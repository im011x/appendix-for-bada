/*
 * HandleBusStopLines.h
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#ifndef HANDLEBUSSTOPLINES_H_
#define HANDLEBUSSTOPLINES_H_

#include "HandleHttp.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "GroupBusLine.h"
#include "DataBusSeoul.h"

typedef enum {
	BUSARRIVE_REQUEST_CREATOR,
	BUSARRIVE_REQUEST_UPDATOR
} TYPE_BUSARRIVE_REQUEST;

static const int 		EVENT_BUSSTOP_ARRIVE_SENT_GOOD = 101;
static const int 		EVENT_BUSSTOP_ARRIVE_SENT_BAD  = 102;
static const int 		EVENT_BUSSTOP_ARRIVE_RECV_GOOD = 103;
static const int 		EVENT_BUSSTOP_ARRIVE_RECV_NO   = 104;
static const int 		EVENT_BUSSTOP_ARRIVE_RECV_BAD  = 105;

class HandleBusStopLines
	: public IHttpFirstResponder
{
public:
	HandleBusStopLines(ArrayList * pArr, ArrayList * pArrG, Osp::Ui::Control* pEventReceiver = null);
	HandleBusStopLines(ArrayList * pArr, ArrayList * pArrG, Osp::Base::Runtime::Thread * pThread);
	virtual ~HandleBusStopLines();

protected:
	ArrayList 			* 			__pArr; // 외부 클래스에서 생성된 이 클래스에서 update 해야 할 메모리 공간!
	ArrayList 			* 			__pArrGroups; // 마찬가지 ... 외부에서 생성된

	Osp::Ui::Control 	* 			__pEventHandler;
	Osp::Base::Runtime::Thread 	* 	__pThreadHandler;

	HashMap 			*			__pMapGroups;

	TYPE_BUSARRIVE_REQUEST			__reqType;
	HttpBusSeoul 		* 			__pHttpSeoulBus;
	XmlBusSeoulParseArrivesByStop 	__parser;

protected:
	bool 				init();
	void 				fini();
	void 				makeGroups();
	GroupBusLine * 		buildGroup(const String & key);

	void 				sendEvent(int event, const String & message = "");

public:
	bool 			Invoke(const String & value);
	bool 			Update(const String & value);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

};

#endif /* HANDLEBUSSTOPLINES_H_ */
