/*
 * ThreadMybus.cpp
 *
 *  Created on: 2012. 8. 16.
 *      Author: benny
 */

#include "ThreadMybus.h"

ThreadMybus::ThreadMybus(ArrayList * pArrIn, ArrayList * pArrOut, Osp::Ui::Controls::Form* pEventHolder)
	: __pArrIn(pArrIn), __pArrOut(pArrOut), __pEventHandler(pEventHolder)
{
	__pMybusUpdator = null;
	__pTimerHandler = null;

	__timerInterval = 1;
	__timerCount = 0;
}

ThreadMybus::~ThreadMybus()
{
	AppLogDebug("@@@ Mybus 쓰레드가 파괴됩니다...");
	Terminalize();
}

bool ThreadMybus::Initialize(int intervalSeconds)
{
	__timerInterval = intervalSeconds;

	if (__timerInterval == 0) {
		__timerInterval = 30; // default value
	}

	result r = Thread::Construct(THREAD_TYPE_EVENT_DRIVEN);

	AppLogDebug("Mybus update thread 생성 !");

	return (r == E_SUCCESS);
}

void ThreadMybus::Terminalize()
{
	if (__pTimerHandler) {
		delete __pTimerHandler;
		__pTimerHandler = null;
	}

	if (__pMybusUpdator) {
		delete __pMybusUpdator;
		__pMybusUpdator = null;
	}

}

bool ThreadMybus::OnStart(void)
{
	if (__pTimerHandler == null) {
		__pTimerHandler = new HandleTimer();
		__pTimerHandler->Create(this);
		//__pTimerHandler->Start(1000 * __timerInterval);
		__pTimerHandler->SetForever(false);
	}

	if (__pMybusUpdator == null) {
		__pMybusUpdator = new HandleMybus(__pArrIn, __pArrOut, this);
		__pMybusUpdator->Initialize();
	}

//	if (__pMybusUpdator->Invoke()) {
//		AppLogDebug("@@@ Mybus 쓰레드에서 요청합니다...");
//		__pEventHandler->SendUserEvent(EVENT_HANDLER_MYBUS_THREAD_COMM_SHOW, null);
//	}
	__pTimerHandler->Start(100);

	AppLogDebug("@@@ Mybus 쓰레드가 시작됩니다...");

	return true;
}

void ThreadMybus::OnStop(void)
{
	AppLogDebug("@@@ Mybus 쓰레드가 종료됩니다...");
	Terminalize();
}

bool ThreadMybus::UpdateForce(bool lazy)
{
	AppLogDebug("@@@ UpdateForce 에서 ~");

	if (__pTimerHandler == null) {
		return false;
	}
	if (__pMybusUpdator == null) {
		return false;
	}

	__pTimerHandler->Stop();

	//int nsleep = (lazy ? 1500 : 200);
	__pTimerHandler->Start(100);

	return true;
}

void ThreadMybus::AtTimer(HandleTimer * pTimerHandler)
{
	AppLogDebug("@@@ timer 에서 ~");

	if (__pMybusUpdator->Invoke())
		__pEventHandler->SendUserEvent(EVENT_HANDLER_MYBUS_THREAD_COMM_SHOW, null);

}

void ThreadMybus::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	switch (requestId) {
	case EVENT_HANDLER_MYBUS_RECV_GOOD_ONE:
		AppLogDebug("one response is safe!");
		if (__pEventHandler) {
			__pEventHandler->SendUserEvent(requestId, null);
		}

		if (__pMybusUpdator->IsOneSet()) {
			__pEventHandler->SendUserEvent(EVENT_HANDLER_MYBUS_THREAD_COMM_HIDE, null);
			__pTimerHandler->Start(1000 * __timerInterval);
		}
		else {
			__pMybusUpdator->Invoke();
		}
		break;
	default:
		break;
	}
}

