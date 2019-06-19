#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H
#include <functional>
#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>

//////////////////////////////////////////////////////////////////////////
// Timer Event base class 
// The class which use timer event should implement from this class.
//////////////////////////////////////////////////////////////////////////
class TimerHandler : public ACE_Event_Handler, public std::enable_shared_from_this<TimerHandler>
{
private:
	struct TimerDefinition
	{
		TimerDefinition(const int* timerId, std::function<void(int)> handler, const std::shared_ptr<TimerHandler> object, bool callOnce);
		~TimerDefinition();
		const int* m_timerId;
		std::function<void(int)> m_handler;
		const std::shared_ptr<TimerHandler> m_timerObject;
		const bool m_callOnce;
	};

	// Timer expire call back function, override from ACE
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act = 0) override final;
public:
	TimerHandler();
	virtual ~TimerHandler();

	/// <summary>
	/// Register a timer to this object
	/// </summary>
	/// <param name="delaySeconds">Timer will start after delay seconds.</param>
	/// <param name="intervalSeconds">Interval for the Timer, the value 0 means the timer will only triggered once.</param>
	/// <param name="handler">Function point to this object.</param>
	/// <return>Timer unique ID.</return>
	int registerTimer(size_t delaySeconds, size_t intervalSeconds, const std::function<void(int)> &handler, const std::string from);
	bool cancleTimer(int timerId);

	static void runEventLoop();
	static int endEventLoop();
private:
	// key: timer ID point, must unique, value: function point
	std::map<const int*, std::shared_ptr<TimerDefinition>> m_timers;
	std::recursive_mutex m_mutex;
};

#endif


