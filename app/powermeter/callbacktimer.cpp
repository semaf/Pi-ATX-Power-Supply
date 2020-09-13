/*
 * CallBackTimer.cpp
 * August 202
 */

#include <future>
#include <cstdio>
#include "callbacktimer.h"

CallBackTimer::CallBackTimer(std::function<void(void)> func)
	:_running(false), callBackFunction(func)
{}

CallBackTimer::~CallBackTimer() {
	if( _running.load(std::memory_order_acquire) ) {
		StopThread();
	};
}

void CallBackTimer::StopThread()
{
	_running.store(false, std::memory_order_release);
	if( _worker_thread.joinable() )
		_worker_thread.join();
}

void CallBackTimer::StartThread(int interval, std::function<void(void)> callBackFunction)
{
	if( _running.load(std::memory_order_acquire) ) {
		StopThread();
	};
	_running.store(true, std::memory_order_release);
	_worker_thread = std::thread([this, interval, callBackFunction]()
			{
			while (_running.load(std::memory_order_acquire)) {
			callBackFunction();                   
			std::this_thread::sleep_for(
					std::chrono::milliseconds(interval));
			}
			});
}

bool CallBackTimer::isRunning() const {
	return ( _running.load(std::memory_order_acquire) && 
			_worker_thread.joinable() );
}

