#pragma once

#include <map>
#include <string>
#include <ace/Process.h>
#include "TimerHandler.h"

class LinuxCgroup;
class ResourceLimitation;
//////////////////////////////////////////////////////////////////////////
/// Process Object
//////////////////////////////////////////////////////////////////////////
class AppProcess :public ACE_Process, public TimerHandler
{
public:
	explicit AppProcess(int cacheOutputLines = 256);
	virtual ~AppProcess();

	void attach(int pid);
	void detach();
	virtual pid_t getpid(void) const;
	virtual void killgroup(int timerId = 0);
	virtual void setCgroup(std::shared_ptr<ResourceLimitation>& limit);
	const std::string getuuid() const;
	void regKillTimer(std::size_t timeoutSec, const std::string from);
	virtual std::string containerId() { return std::string(); };
	virtual void containerId(std::string containerId) {};

	std::tuple<std::string, std::string> extractCommand(const std::string& cmd);

	virtual int spawnProcess(std::string cmd, std::string user, std::string workDir,
		std::map<std::string, std::string> envMap, std::shared_ptr<ResourceLimitation> limit,
		std::string stdoutFile);

	virtual std::string getOutputMsg();
	virtual std::string fetchOutputMsg();
	virtual bool complete() { return true; }

protected:
	const int m_cacheOutputLines;
	std::shared_ptr<int> m_returnCode;

private:
	std::unique_ptr<LinuxCgroup> m_cgroup;
	int m_killTimerId;
	ACE_HANDLE m_stdoutHandler;
	std::string m_uuid;
};
