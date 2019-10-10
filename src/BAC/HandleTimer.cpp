/*
 * HandleTimer.cpp
 *
 *  Created on: 2012. 1. 13.
 *      Author: SONG
 */

#include "HandleTimer.h"

HandleTimer::HandleTimer() : __pTimer(null), __pDelegate(null), __forever(false), __bRunning(false)
{
	__interval = 0;
}

HandleTimer::~HandleTimer()
{
	if (__pTimer) {
		if (__bRunning)
			__pTimer->Cancel();

		delete __pTimer;
		__pTimer = null;
	}
}

bool HandleTimer::Create(ITimerDelegate * pDelegate, int intervals)
{
	__pTimer = new Osp::Base::Runtime::Timer();
	__pTimer->Construct(*this);

	if (pDelegate) {
		SetDelegate(pDelegate);
	}

	if (intervals > 0) {
		__pTimer->Start(intervals);
	}
	__interval = intervals;

	return true;
}

bool HandleTimer::Start(int intervals)
{
	if (__pTimer == null)
		return false;

	__pTimer->Start(intervals);
	__interval = intervals;

	__bRunning = true;

	return true;
}

void HandleTimer::Stop()
{
	if (__pTimer) {
		__pTimer->Cancel();
	}

	__bRunning = false;
}

void HandleTimer::OnTimerExpired(Timer& timer)
{
	if (__pTimer == null)
		return;

	if (__forever) {
		__pTimer->Start(__interval);
	}
	else {
		Stop();
	}

	if (__pDelegate) {
		__pDelegate->AtTimer(this);
	}

}

