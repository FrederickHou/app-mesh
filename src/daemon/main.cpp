#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <ace/Init_ACE.h>
#include "RestHandler.h"
#include "PrometheusRest.h"
#include "../common/Utility.h"
#include "Application.h"
#include "Configuration.h"
#include "ResourceCollection.h"
#include "TimerHandler.h"
#include "HealthCheckTask.h"

int main(int argc, char* argv[])
{
	const static char fname[] = "main() ";
	PRINT_VERSION();

	try
	{
		ACE::init();
		// set working dir
		ACE_OS::chdir(Utility::getSelfDir().c_str());

		// init log
		Utility::initLogging();

		LOG_INF << fname << "entered with working dir: " << getcwd(NULL, 0);
		Configuration::handleReloadSignal();

		// Resource init
		ResourceCollection::instance()->getHostResource();
		ResourceCollection::instance()->dump();

		// get configuration
		auto config = Configuration::FromJson(Configuration::readConfiguration());
		Configuration::instance(config);

		// set log level
		Utility::setLogLevel(config->getLogLevel());
		Configuration::instance()->dump();

		std::shared_ptr<RestHandler> httpServerIp4;
		std::shared_ptr<RestHandler> httpServerIp6;
		if (config->getRestEnabled())
		{
			// Init REST
			if (!config->getRestListenAddress().empty())
			{
				// just enable for specified address
				httpServerIp4 = std::make_shared<RestHandler>(config->getRestListenAddress(), config->getRestListenPort());
			}
			else
			{
				// enable for both ipv6 and ipv4
				httpServerIp4 = std::make_shared<RestHandler>("0.0.0.0", config->getRestListenPort());
				try
				{
					httpServerIp6 = std::make_shared<RestHandler>(ResourceCollection::instance()->getHostName(), config->getRestListenPort());
				}
				catch (const std::exception& e)
				{
					LOG_ERR << fname << e.what();
				}
				catch (...)
				{
					LOG_ERR << fname << "unknown exception";
				}
			}
			LOG_INF << fname << "initialize_with_threads:" << config->getThreadPoolSize();
		}

		// HA attach process to App
		auto apps = config->getApps();
		std::map<std::string, int> process;
		AppProcess::getSysProcessList(process, nullptr);
		std::for_each(apps.begin(), apps.end(), [&process](std::vector<std::shared_ptr<Application>>::reference p) { p->attach(process); });

		// start one thread for timers
		auto timerThread = std::make_unique<std::thread>(std::bind(&TimerHandler::runTimerThread));

		// health check share main thread for now
		// // start one thread for health check
		// HealthCheckTask::instance()->open();
		auto healthCheckTime = std::chrono::system_clock::now();

		// monitor applications
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(Configuration::instance()->getScheduleInterval()));

			// monitor application
			auto apps = Configuration::instance()->getApps();
			for (const auto& app : apps)
			{
				app->invoke();
			}
			
			// health check
			if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - healthCheckTime).count() >= DEFAULT_HEALTH_CHECK_INTERVAL)
			{
				HealthCheckTask::instance()->healthCheckAllApp();
				healthCheckTime = std::chrono::system_clock::now();
			}
		}
	}
	catch (const std::exception & e)
	{
		LOG_ERR << fname << e.what();
	}
	catch (...)
	{
		LOG_ERR << fname << "unknown exception";
	}
	LOG_ERR << fname << "ERROR exited";
	ACE::fini();
	_exit(0);
	return 0;
}
