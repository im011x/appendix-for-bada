/*
 * HandleBusStopLines.cpp
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#include "HandleBusStopLines.h"

HandleBusStopLines::HandleBusStopLines(ArrayList * pArr, ArrayList * pArrG, Osp::Ui::Control* pEventReceiver) : __pArr(pArr), __pArrGroups(pArrG)
	, __pEventHandler(pEventReceiver), __pMapGroups(null), __pHttpSeoulBus(null)
{
	__pThreadHandler = null;

	__reqType = BUSARRIVE_REQUEST_CREATOR;

	init();
}

HandleBusStopLines::HandleBusStopLines(ArrayList * pArr, ArrayList * pArrG, Osp::Base::Runtime::Thread * pThread) : __pArr(pArr), __pArrGroups(pArrG)
	, __pThreadHandler(pThread), __pMapGroups(null), __pHttpSeoulBus(null)
{
	__pEventHandler = null;

	__reqType = BUSARRIVE_REQUEST_CREATOR;

	init();
}


HandleBusStopLines::~HandleBusStopLines()
{
	fini();
}

bool HandleBusStopLines::init()
{
	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}

	if (__pMapGroups == null) {
		__pMapGroups = new HashMap();
		__pMapGroups->Construct();
	}

	return true;
}

void HandleBusStopLines::fini()
{
	if (__pHttpSeoulBus)
		delete __pHttpSeoulBus;

	if (__pMapGroups) {
		__pMapGroups->RemoveAll(false);
		delete __pMapGroups;
	}
}

bool HandleBusStopLines::Invoke(const String & value)
{
	__reqType = BUSARRIVE_REQUEST_CREATOR;

	if (__pHttpSeoulBus->GetArrivingByStop(value)) {
		sendEvent(EVENT_BUSSTOP_ARRIVE_SENT_GOOD);
	}
	else {
		sendEvent(EVENT_BUSSTOP_ARRIVE_SENT_BAD, "전송실패");
	}

	return true;
}

bool HandleBusStopLines::Update(const String & value)
{
	__reqType = BUSARRIVE_REQUEST_UPDATOR;

	bool bOK = __pHttpSeoulBus->GetArrivingByStop(value);

	return bOK;
}

void HandleBusStopLines::makeGroups()
{
	if (__pArr == null) {
		AppAssert(0);
		return;
	}

	for (int i=0; i<__pArr->GetCount(); i++) {
		BusSeoul_ArrivingInfo* pArrv = static_cast<BusSeoul_ArrivingInfo*> (__pArr->GetAt(i));
		GroupBusLine * pGroup = buildGroup(pArrv->__routeType);
		pGroup->pArr->Add(*pArrv);
	}

}

GroupBusLine * HandleBusStopLines::buildGroup(const String & key)
{
	bool bIsIn = false;
	__pMapGroups->ContainsKey(key, bIsIn);

	if (bIsIn) {  // exist already!
		return static_cast<GroupBusLine*> (__pMapGroups->GetValue(key));
	}

	// make an instance of the key and make new one instance
	String * pKey = new String(key);
	GroupBusLine * pOneGroup = new GroupBusLine();
	pOneGroup->SetName(key);

	__pMapGroups->Add(*pKey, *pOneGroup);
	__pArrGroups->Add(*pOneGroup);

	return pOneGroup;
}

void HandleBusStopLines::sendEvent(int event, const String & message)
{
	if (__pEventHandler) {
		ArrayList * parr = null;

		if (!message.IsEmpty()) {
			parr = new ArrayList();
			parr->Construct();
			parr->Add(*(new String(message)));
		}

		__pEventHandler->SendUserEvent(event, parr);
		// event 받는 쪽에서 parr 메모리 삭제한다!
	}

	if (__pThreadHandler) {
		__pThreadHandler->SendUserEvent(event, null);
	}

}


void HandleBusStopLines::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	sendEvent(EVENT_BUSSTOP_ARRIVE_RECV_BAD, message);
}

void HandleBusStopLines::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	__parser.ParseXml(pBuffer, __pArr);  // __reqType 도 파라미터로 넘겨, 생성 여부를 제어하도록 한다.

	if (__pArr->GetCount() <= 0) {
		//sendEvent(EVENT_BUSSTOP_ARRIVE_RECV_NO, __parser.GetMessage());  //"결과가 없는 경우 마을버스 정류소일 수 있습니다."
		sendEvent(EVENT_BUSSTOP_ARRIVE_RECV_NO, __parser.GetMessage() + "\n\n마을버스 정류소의 버스도착 정보는 알 수 없습니다." );
	}
	else {
		if (__reqType == BUSARRIVE_REQUEST_CREATOR) {  // it's 1st fetching ...
			CompareBusSeoul_Arrive * pCompare = new CompareBusSeoul_Arrive();
			__pArr->Sort(*pCompare);
			delete pCompare;

			makeGroups();
		}
		sendEvent(EVENT_BUSSTOP_ARRIVE_RECV_GOOD);
	}
}

