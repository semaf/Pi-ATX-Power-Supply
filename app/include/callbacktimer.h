/*
 * CallBackTimer.h
 * August 2020
 */

#ifndef CALLBACKTIMER_H_
#define CALLBACKTIMER_H_

#include <thread>
#include <functional>
#include <atomic>

class CallBackTimer
{
	public:
		CallBackTimer(std::function<void(void)> func);
		~CallBackTimer();

		void StopThread();
		void StartThread(int interval, std::function<void(void)> func);

		bool isRunning() const;

	private:
		std::atomic<bool> _running;
		std::thread _worker_thread;
		std::function<void(void)> callBackFunction;
};
#endif
