#pragma once

#include <map>
#include <string>
#include <fstream>
#include <ace/Process.h>
#include "../TimerHandler.h"

class LinuxCgroup;
class ResourceLimitation;
//////////////////////////////////////////////////////////////////////////
/// Process Object
//////////////////////////////////////////////////////////////////////////
class AppProcess : public ACE_Process, public TimerHandler
{
public:
	AppProcess();
	virtual ~AppProcess();

	void attach(int pid);
	void detach();
	virtual pid_t getpid(void) const;
	virtual void killgroup(int timerId = 0);
	virtual void setCgroup(std::shared_ptr<ResourceLimitation> &limit);
	const std::string getuuid() const;
	void regKillTimer(std::size_t timeoutSec, const std::string from);
	virtual std::string containerId() { return std::string(); };
	virtual void containerId(std::string containerId){};

	std::tuple<std::string, std::string> extractCommand(const std::string &cmd);

	virtual int spawnProcess(std::string cmd, std::string user, std::string workDir,
							 std::map<std::string, std::string> envMap, std::shared_ptr<ResourceLimitation> limit,
							 const std::string &stdoutFile = "", const std::string &stdinFileContent = "");

	virtual std::string fetchOutputMsg();
	virtual std::string fetchLine();
	virtual bool complete() { return true; }
	// error
	void startError(const std::string &err) { m_startError = err; };
	const std::string startError() const { return m_startError; }

protected:
	std::shared_ptr<int> m_returnCode;
	std::string m_stdoutFileName;

private:
	std::unique_ptr<LinuxCgroup> m_cgroup;
	int m_killTimerId;
	ACE_HANDLE m_stdinHandler;
	ACE_HANDLE m_stdoutHandler;
	std::string m_stdinFileName;
	std::string m_uuid;
	std::string m_startError;
	mutable std::recursive_mutex m_outFileMutex;
	std::shared_ptr<std::ifstream> m_stdoutReadStream;
};
