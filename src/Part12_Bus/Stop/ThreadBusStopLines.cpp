/*
 * ThreadBusStopLines.cpp
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#include "ThreadBusStopLines.h"

ThreadBusStopLines::ThreadBusStopLines(ArrayList * pArr, ArrayList * pArrG) : __pArr(pArr), __pArrGroups(pArrG),
	__pBusStopLinesUpdator(null), __pTimerHandler(null)
{
	__timerInterval = 0;
	__pEventHandler = null;
}

ThreadBusStopLines::~ThreadBusStopLines()
{
	Terminalize();
}

bool ThreadBusStopLines::Initialize(const String & key, int intervalSeconds, Osp::Ui::Controls::Form* pEventHolder)
{
	__queryKey = key;
	__timerInterval = intervalSeconds;
	__pEventHandler = pEventHolder;

	result r = Thread::Construct(THREAD_TYPE_EVENT_DRIVEN);

	AppLogDebug("inner of update thread 생성 !");

	return (r == E_SUCCESS);
}

void ThreadBusStopLines::Terminalize()
{
	if (__pTimerHandler) {
		delete __pTimerHandler;
		__pTimerHandler = null;
	}

	if (__pBusStopLinesUpdator) {
		delete __pBusStopLinesUpdator;
		__pBusStopLinesUpdator = null;
	}
}

bool ThreadBusStopLines::UpdateForce(bool lazy)
{
	if (__pTimerHandler) {
		__pTimerHandler->Stop();

		//int nsleep = (lazy ? 1500 : 200);
		__pTimerHandler->Start(100);
	}

	return true;
}

bool ThreadBusStopLines::OnStart(void)
{
	if (__pBusStopLinesUpdator == null) {
		__pBusStopLinesUpdator = new HandleBusStopLines(__pArr, __pArrGroups, this);
	}

	if (__pTimerHandler == null) {
		__pTimerHandler = new HandleTimer();
		__pTimerHandler->Create(this);
		__pTimerHandler->Start(1000 * __timerInterval);
		__pTimerHandler->SetForever(false);
	}

	AppLogDebug("@@@ 쓰레드가 시작됩니다...");

	return true;
}

void ThreadBusStopLines::OnStop(void)
{
	Terminalize();
}

void ThreadBusStopLines::AtTimer(HandleTimer * pTimerHandler)
{
	AppLogDebug("@@@ timer 에서 요청합니다~");

	if (__pBusStopLinesUpdator->Update(__queryKey))
		__pEventHandler->SendUserEvent(EVENT_BUSSTOP_THREAD_COMM_SHOW, null);
}

void ThreadBusStopLines::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	AppLogDebug("received an event (%d)", requestId);

	switch (requestId) {
	case EVENT_BUSSTOP_ARRIVE_SENT_GOOD:
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_GOOD:
		if (__pTimerHandler) {
			__pTimerHandler->Start(1000 * __timerInterval);
		}
		break;
	case EVENT_BUSSTOP_ARRIVE_SENT_BAD:
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_NO:
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_BAD:
		break;
	default:
		break;
	}

	//
	if (__pEventHandler) {
		switch (requestId) {
		case EVENT_BUSSTOP_ARRIVE_SENT_GOOD:
			//__pEventHandler->SendUserEvent(EVENT_BUSSTOP_THREAD_COMM_SHOW, null);
			break;
		case EVENT_BUSSTOP_ARRIVE_RECV_GOOD:
		case EVENT_BUSSTOP_ARRIVE_SENT_BAD:
		case EVENT_BUSSTOP_ARRIVE_RECV_NO:
		case EVENT_BUSSTOP_ARRIVE_RECV_BAD:
			__pEventHandler->SendUserEvent(EVENT_BUSSTOP_THREAD_COMM_HIDE, null);
			break;
		default:
			break;
		}
	}

}

