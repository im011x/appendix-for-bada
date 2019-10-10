/*
 * HandleBusPositions.cpp
 *
 *  Created on: 2012. 8. 11.
 *      Author: benny
 */

#include "HandleBusPositions.h"

HandleBusPositions::HandleBusPositions(ArrayList * pArr, Osp::Base::Runtime::Thread * pThread) : __pArr(pArr), __pEventHandler(pThread)
{
	__reqType = BUSPOSITION_REQUEST_CREATOR;

	init();
}

HandleBusPositions::~HandleBusPositions()
{
	fini();
}


bool HandleBusPositions::init()
{
	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}

	return true;
}

void HandleBusPositions::fini()
{
	if (__pHttpSeoulBus)
		delete __pHttpSeoulBus;

}

void HandleBusPositions::sendEvent(int event, const String & message)
{
	if (__pEventHandler) {
		ArrayList * parr = null;

		/*
		if (!message.IsEmpty()) {
			parr = new ArrayList();
			parr->Construct();
			parr->Add(*(new String(message)));
		}
		 */

		__pEventHandler->SendUserEvent(event, parr);
		// event 받는 쪽에서 parr 메모리 삭제한다!
	}

}

bool HandleBusPositions::Invoke(const String & value)
{
	__reqType = BUSPOSITION_REQUEST_CREATOR;

	if (__pHttpSeoulBus->GetPositionsByLine(value)) {
		sendEvent(EVENT_BUS_POSITION_SENT_GOOD);
	}
	else {
		sendEvent(EVENT_BUS_POSITION_SENT_BAD, "전송실패");
	}

	return true;
}

bool HandleBusPositions::Update(const String & value)
{
	__reqType = BUSPOSITION_REQUEST_UPDATOR;

	bool bOK = __pHttpSeoulBus->GetPositionsByLine(value);

	return bOK;
}


void HandleBusPositions::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	sendEvent(EVENT_BUS_POSITION_RECV_BAD, message);
}

void HandleBusPositions::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	__parser.ParseXml(pBuffer, __pArr, (__reqType == BUSPOSITION_REQUEST_UPDATOR));  // __reqType 도 파라미터로 넘겨, 생성 여부를 제어하도록 한다.

	if (__pArr->GetCount() <= 0) {
		sendEvent(EVENT_BUS_POSITION_RECV_NO, __parser.GetMessage());
	}
	else {
		sendEvent(EVENT_BUS_POSITION_RECV_GOOD);
	}
}


