/*
 * ThreadBusPositions.cpp
 *
 *  Created on: 2012. 8. 11.
 *      Author: benny
 */

#include "ThreadBusPositions.h"

ThreadBusPositions::ThreadBusPositions(ArrayList * pArr, Osp::Ui::Controls::Form* pEventHolder) : __pArr(pArr), __pEventHandler(pEventHolder)
{
	__timerInterval = 0;
	__countRequest = 0;
}

ThreadBusPositions::~ThreadBusPositions()
{
	Terminalize();
}

bool ThreadBusPositions::Initialize(const String & key, int intervalSeconds)
{
	__queryKey = key;
	__timerInterval = intervalSeconds;

	result r = Thread::Construct(THREAD_TYPE_EVENT_DRIVEN);

	AppLogDebug("inner of Get Bus Position thread 생성 !");

	return (r == E_SUCCESS);
}

void ThreadBusPositions::Terminalize()
{
	if (__pTimerHandler) {
		delete __pTimerHandler;
		__pTimerHandler = null;
	}

	if (__pBusPositionUpdator) {
		delete __pBusPositionUpdator;
		__pBusPositionUpdator = null;
	}
}

bool ThreadBusPositions::UpdateForce(bool lazy)
{
	if (__pTimerHandler) {
		__pTimerHandler->Stop();

		//int nsleep = (lazy ? 1500 : 200);
		__pTimerHandler->Start(100);
	}

	return true;
}

bool ThreadBusPositions::OnStart(void)
{
	if (__pBusPositionUpdator == null) {
		__pBusPositionUpdator = new HandleBusPositions(__pArr, this);
	}

	if (__pTimerHandler == null) {
		__pTimerHandler = new HandleTimer();
		__pTimerHandler->Create(this);
		//__pTimerHandler->Start(1000 * __timerInterval); // 일단 Start 하지 않는다~
		__pTimerHandler->SetForever(false);
	}

	AppLogDebug("@@@ 쓰레드가 시작됩니다... 일단 요청합니다. ");

	/*
	if (__countRequest == 0) {
		__pBusPositionUpdator->Invoke(__queryKey);
	}
	else {
		__pBusPositionUpdator->Update(__queryKey);
	}

	__countRequest++;
	 */

	__pTimerHandler->Start(100);

	return true;
}

void ThreadBusPositions::OnStop(void)
{
	Terminalize();
}

void ThreadBusPositions::AtTimer(HandleTimer * pTimerHandler)
{
	AppLogDebug("timer 에서 요청합니다~");

	__pEventHandler->SendUserEvent(EVENT_BUSPOS_THREAD_COMM_SHOW, null);
	__pBusPositionUpdator->Update(__queryKey);
}

void ThreadBusPositions::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	AppLogDebug("received an event (%d)", requestId);

	switch (requestId) {
	case EVENT_BUS_POSITION_SENT_GOOD:
		break;
	case EVENT_BUS_POSITION_RECV_NO:
	case EVENT_BUS_POSITION_RECV_GOOD:
		// data를 associate 시키고 ... 화면을 갱신하도록 한다~
		__pEventHandler->SendUserEvent(EVENT_BUSPOS_THREAD_UPDATED, null);
		//
		if (__pTimerHandler) {
			__pTimerHandler->Start(1000 * __timerInterval);
		}
		break;
	case EVENT_BUS_POSITION_SENT_BAD:
		break;
	case EVENT_BUS_POSITION_RECV_BAD:
		break;
	default:
		break;
	}

	if (__pEventHandler) {
		switch (requestId) {
		case EVENT_BUS_POSITION_SENT_GOOD:
			break;
		case EVENT_BUS_POSITION_RECV_NO:
		case EVENT_BUS_POSITION_RECV_GOOD:
		case EVENT_BUS_POSITION_SENT_BAD:
		case EVENT_BUS_POSITION_RECV_BAD:
			__pEventHandler->SendUserEvent(EVENT_BUSPOS_THREAD_COMM_HIDE, null);
			break;
		default:
			break;
		}

	}
}

