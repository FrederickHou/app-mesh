#ifndef APPLICATION_DEFINITION_SOHORT_RUN_H
#define APPLICATION_DEFINITION_SOHORT_RUN_H

#include <memory>
#include <string>
#include <map>
#include <mutex>
#include "Application.h"

/**
* @class Application
*
* @brief An Short Running Application will start periodly.
*
*/
class ApplicationShortRun : public Application
{
public:
	ApplicationShortRun();
	virtual ~ApplicationShortRun();

	static void FromJson(std::shared_ptr<ApplicationShortRun>& app, const web::json::value& jobj);

	virtual void invoke() override;
	virtual void invokeNow(int timerId) override;
	virtual void enable() override;
	virtual void disable() override;
	virtual web::json::value AsJson(bool returnRuntimeInfo) override;
	void initTimer();
	virtual void refreshPid() override;
	int getStartInterval();
	std::chrono::system_clock::time_point getStartTime();
	virtual bool avialable() override;
	virtual void dump() override;
protected:
	std::chrono::system_clock::time_point m_startTime;
	std::shared_ptr<std::chrono::system_clock::time_point> m_nextLaunchTime;
	int m_startInterval;
	int m_bufferTime;
	int m_timerId;
	std::shared_ptr<AppProcess> m_bufferProcess;
};

#endif 