/*
 * ThreadMybus.h
 *
 *  Created on: 2012. 8. 16.
 *      Author: benny
 */

#ifndef THREADMYBUS_H_
#define THREADMYBUS_H_

#include "HandleTimer.h"
#include "HandleMybus.h"

static const int 		EVENT_HANDLER_MYBUS_THREAD_COMM_SHOW 		= 201;
static const int 		EVENT_HANDLER_MYBUS_THREAD_COMM_HIDE 		= 202;

class ThreadMybus
	: public Thread
	, public ITimerDelegate
{
public:
	ThreadMybus(ArrayList * pArrIn, ArrayList * pArrOut, Osp::Ui::Controls::Form* pEventHolder);
	virtual ~ThreadMybus();

protected:
	ArrayList 			* 			__pArrIn; // 외부 클래스에서 생성된 이 클래스에서
	ArrayList 			* 			__pArrOut; // 마찬가지 ... 외부에서 생성된 update 해야 할 메모리 공간!
	Osp::Ui::Controls::Form	*		__pEventHandler;

	HandleMybus 		* 			__pMybusUpdator;
	HandleTimer  		* 			__pTimerHandler;
	int 							__timerInterval;

	int 							__timerCount;

public:
	bool 	Initialize(int intervalSeconds);
	void 	Terminalize();
	bool 	UpdateForce(bool lazy);

	// Thread
	virtual bool OnStart(void);
	virtual void OnStop(void);

	// ITimerDelegate
	virtual void 	AtTimer(HandleTimer * pTimerHandler);

	//
	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);
};

#endif /* THREADMYBUS_H_ */
