/*
 * HandleBusPositions.h
 *
 *  Created on: 2012. 8. 11.
 *      Author: benny
 */

#ifndef HANDLEBUSPOSITIONS_H_
#define HANDLEBUSPOSITIONS_H_

#include "HandleHttp.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DataBusSeoul.h"

typedef enum {
	BUSPOSITION_REQUEST_CREATOR,
	BUSPOSITION_REQUEST_UPDATOR
} TYPE_BUSPOSITION_REQUEST;

static const int 		EVENT_BUS_POSITION_SENT_GOOD = 101;
static const int 		EVENT_BUS_POSITION_SENT_BAD  = 102;
static const int 		EVENT_BUS_POSITION_RECV_GOOD = 103;
static const int 		EVENT_BUS_POSITION_RECV_NO   = 104;
static const int 		EVENT_BUS_POSITION_RECV_BAD  = 105;

class HandleBusPositions
	: public IHttpFirstResponder
{
public:
	HandleBusPositions(ArrayList * pArr, Osp::Base::Runtime::Thread * pThread);
	virtual ~HandleBusPositions();

protected:
	ArrayList 					* 	__pArr; // 외부 클래스에서 생성된 이 클래스에서 update 해야 할 메모리 공간!
	Osp::Base::Runtime::Thread 	* 	__pEventHandler;

	TYPE_BUSPOSITION_REQUEST		__reqType;
	HttpBusSeoul 		* 			__pHttpSeoulBus;
	XmlBusSeoulParsePositionInfo 	__parser;

protected:
	bool 				init();
	void 				fini();

	void 				sendEvent(int event, const String & message = "");

public:
	bool 			Invoke(const String & value);
	bool 			Update(const String & value);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

};

#endif /* HANDLEBUSPOSITIONS_H_ */
