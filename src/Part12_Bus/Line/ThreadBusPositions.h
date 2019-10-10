/*
 * ThreadBusPositions.h
 *
 *  Created on: 2012. 8. 11.
 *      Author: benny
 */

#ifndef THREADBUSPOSITIONS_H_
#define THREADBUSPOSITIONS_H_

#include "HandleTimer.h"
#include "HandleBusPositions.h"


static const int 		EVENT_BUSPOS_THREAD_UPDATED = 101;
static const int 		EVENT_BUSPOS_THREAD_COMM_SHOW  = 111;
static const int 		EVENT_BUSPOS_THREAD_COMM_HIDE  = 112;

class ThreadBusPositions
	: public Thread
	, public ITimerDelegate
{
public:
	ThreadBusPositions(ArrayList * pArr, Osp::Ui::Controls::Form* pEventHolder);
	virtual ~ThreadBusPositions();

protected:
	ArrayList 			* 			__pArr; // 외부 클래스에서 생성된 이 클래스에서 update 해야 할 메모리 공간!
	Osp::Ui::Controls::Form	*		__pEventHandler; // 화면 갱신용 ...

	HandleBusPositions 	* 			__pBusPositionUpdator;
	HandleTimer  		* 			__pTimerHandler;

	int 							__timerInterval;
	String 							__queryKey;
	int 							__countRequest;

public:
	bool 	Initialize(const String & key, int intervalSeconds);
	bool 	UpdateForce(bool lazy);
	void 	Terminalize();

	// Thread
	virtual bool OnStart(void);
	virtual void OnStop(void);

	// ITimerDelegate
	virtual void 	AtTimer(HandleTimer * pTimerHandler);

	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);

};

#endif /* THREADBUSPOSITIONS_H_ */
