/*
 * HandleMybus.h
 *
 *  Created on: 2012. 8. 16.
 *      Author: benny
 */

#ifndef HANDLEMYBUS_H_
#define HANDLEMYBUS_H_

#include "HandleHttp.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DatabaseBus.h"

static const int 		EVENT_HANDLER_MYBUS_RECV_GOOD_ONE 		= 101;
//static const int 		EVENT_HANDLER_MYBUS_UPDATE_LIST 		= 102;

class HandleMybus
	: public IHttpFirstResponder
{
public:
	HandleMybus(ArrayList * pArrIn, ArrayList * pArrOut, Osp::Base::Runtime::Thread * pEventHolder);
	virtual ~HandleMybus();

protected:
	ArrayList 			* 			__pArrIn;  // 외부에서 생성된 ...
	ArrayList 			* 			__pArrOut; // 외부 클래스에서 생성된 이 클래스에서 update 해야 할 메모리 공간!
	Osp::Base::Runtime::Thread 	* 	__pThreadHandler;

	ArrayList 			* 			__pArr;

	HttpBusSeoul 		* 			__pHttpSeoulBus;
	XmlBusSeoulParseArrivesByLine 	__parser;

	int 							__countInvoke;

protected:
	bool 				init();
	void 				fini();
	void 				sendEvent(int event, const String & message = "");

public:
	void 			Initialize();
	bool 			Invoke();
	bool 			IsOneSet();

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

};

#endif /* HANDLEMYBUS_H_ */
