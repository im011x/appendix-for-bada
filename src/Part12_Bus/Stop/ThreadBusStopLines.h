/*
 * ThreadBusStopLines.h
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#ifndef THREADBUSSTOPLINES_H_
#define THREADBUSSTOPLINES_H_

#include "HandleBusStopLines.h"
#include "HandleTimer.h"
//#include "BCanAccssibility.h"

static const int 		EVENT_BUSSTOP_THREAD_COMM_SHOW  = 111;
static const int 		EVENT_BUSSTOP_THREAD_COMM_HIDE  = 112;

class ThreadBusStopLines
	: public Thread
	, public ITimerDelegate
	//, public BCanAccssibility
{
public:
	ThreadBusStopLines(ArrayList * pArr, ArrayList * pArrG);
	virtual ~ThreadBusStopLines();

protected:
	ArrayList 			* 			__pArr; // 외부 클래스에서 생성된 이 클래스에서 update 해야 할 메모리 공간!
	ArrayList 			* 			__pArrGroups; // 마찬가지 ... 외부에서 생성된
	HandleBusStopLines 	* 			__pBusStopLinesUpdator;
	HandleTimer  		* 			__pTimerHandler;

	int 							__timerInterval;
	String 							__queryKey;
	Osp::Ui::Controls::Form	*		__pEventHandler;

public:
	bool 	Initialize(const String & key, int intervalSeconds, Osp::Ui::Controls::Form* pEventHolder = null);
	bool 	UpdateForce(bool lazy);
	void 	Terminalize();

	// Thread
	virtual bool OnStart(void);
	virtual void OnStop(void);

	// ITimerDelegate
	virtual void 	AtTimer(HandleTimer * pTimerHandler);

	//
	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);

};

#endif /* THREADBUSSTOPLINES_H_ */
