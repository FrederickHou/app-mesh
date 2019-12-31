#pragma once

#include <memory>
#include <functional>
#include "TimerHandler.h"
#include <cpprest/http_listener.h> // HTTP server 
#include "../common/HttpRequest.h"

class CounterPtr;
class PrometheusRest;
class Application;
class HttpRequest;
//////////////////////////////////////////////////////////////////////////
/// REST service
//////////////////////////////////////////////////////////////////////////
class RestHandler : public TimerHandler
{
public:
	explicit RestHandler(std::string ipaddress, int port);
	virtual ~RestHandler();

	void initMetrics(std::shared_ptr<PrometheusRest> prom);

protected:
	void open();
	void close();

private:
	void handleRest(const http_request& message, std::map<std::string, std::function<void(const HttpRequest&)>>& restFunctions);
	void bindRestMethod(web::http::method method, std::string path, std::function< void(const HttpRequest&)> func);
	void handle_get(const HttpRequest& message);
	void handle_put(const HttpRequest& message);
	void handle_post(const HttpRequest& message);
	void handle_delete(const HttpRequest& message);
	void handle_options(const HttpRequest& message);
	void handle_error(pplx::task<void>& t);

	std::string verifyToken(const HttpRequest& message);
	std::string getTokenUser(const HttpRequest& message);
	bool permissionCheck(const HttpRequest& message, const std::string& permission);
	std::string getTokenStr(const HttpRequest& message);
	std::string createToken(const std::string& uname, const std::string& passwd, int timeoutSeconds);
	void cleanTempApp(int timerId = 0);
	void cleanTempAppByName(std::string appNameStr);
	int getHttpQueryValue(const HttpRequest& message, const std::string key, int defaultValue, int min, int max) const;

	void apiLogin(const HttpRequest& message);
	void apiAuth(const HttpRequest& message);
	void apiGetApp(const HttpRequest& message);
	std::shared_ptr<Application> apiRunParseApp(const HttpRequest& message);
	void apiRunAsync(const HttpRequest& message);
	void apiRunSync(const HttpRequest& message);
	void apiRunAsyncOut(const HttpRequest& message);
	void apiGetAppOutput(const HttpRequest& message);
	void apiGetApps(const HttpRequest& message);
	void apiGetResources(const HttpRequest& message);
	void apiRegApp(const HttpRequest& message);
	void apiEnableApp(const HttpRequest& message);
	void apiDisableApp(const HttpRequest& message);
	void apiDeleteApp(const HttpRequest& message);
	void apiFileDownload(const HttpRequest& message);
	void apiFileUpload(const HttpRequest& message);
	void apiGetLabels(const HttpRequest& message);
	void apiAddLabel(const HttpRequest& message);
	void apiDeleteLabel(const HttpRequest& message);
	void apiGetPermissions(const HttpRequest& message);
	void apiGetBasicConfig(const HttpRequest& message);
	void apiSetBasicConfig(const HttpRequest& message);
	void apiChangePassword(const HttpRequest& message);
	void apiLockUser(const HttpRequest& message);
	void apiUnLockUser(const HttpRequest& message);
	void apiHealth(const HttpRequest& message);

	http_response requestHttp(const web::http::method& mtd, const std::string& path, std::map<std::string, std::string> query, std::map<std::string, std::string> header, web::json::value* body, const std::string& token);

private:
	std::string m_listenAddress;
	std::unique_ptr<web::http::experimental::listener::http_listener> m_listener;
	// API functions
	std::map<std::string, std::function<void(const HttpRequest&)>> m_restGetFunctions;
	std::map<std::string, std::function<void(const HttpRequest&)>> m_restPutFunctions;
	std::map<std::string, std::function<void(const HttpRequest&)>> m_restPstFunctions;
	std::map<std::string, std::function<void(const HttpRequest&)>> m_restDelFunctions;

	std::recursive_mutex m_mutex;
	// key: timerId, value: appName
	std::map<int, std::string> m_tempAppsForClean;

	// prometheus
	std::shared_ptr<CounterPtr> m_promScrapeCounter;
	std::shared_ptr<CounterPtr> m_restGetCounter;
	std::shared_ptr<CounterPtr> m_restPutCounter;
	std::shared_ptr<CounterPtr> m_restDelCounter;
	std::shared_ptr<CounterPtr> m_restPostCounter;
};
