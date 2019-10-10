/*
 * HandleTimer.h
 *
 *  Created on: 2012. 1. 13.
 *      Author: SONG
 */

#ifndef HANDLETIMER_H_
#define HANDLETIMER_H_

#include <FBase.h>
#include <FSystem.h>

using namespace Osp::Base;
using namespace Osp::Base::Runtime;
using namespace Osp::System;

class HandleTimer;

class ITimerDelegate {
public:
	ITimerDelegate() {};
	virtual ~ITimerDelegate() {};

public:
	virtual void 	AtTimer(HandleTimer * pTimerHandler) = 0;
};

class HandleTimer
	: public Osp::Base::Object
	, public Osp::Base::Runtime::ITimerEventListener
{
public:
	HandleTimer();
	virtual ~HandleTimer();

protected:
	Osp::Base::Runtime::Timer * 		__pTimer;
	ITimerDelegate * 					__pDelegate;

private:
	int 		__interval;
	bool 		__forever;

	bool 		__bRunning;

public:
	bool 		Create(ITimerDelegate * pDelegate=null, int intervals = 0);
	bool 		Start(int intervals);
	void 		Stop();
	void 		SetDelegate(ITimerDelegate * p) 			{ __pDelegate = p; };
	void 		SetForever(bool b) 							{  __forever = b; };

	virtual void OnTimerExpired(Timer& timer);
};

#endif /* HANDLETIMER_H_ */
