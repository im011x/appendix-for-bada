/*
 * HandleMybus.cpp
 *
 *  Created on: 2012. 8. 16.
 *      Author: benny
 */

#include "HandleMybus.h"

HandleMybus::HandleMybus(ArrayList * pArrIn, ArrayList * pArrOut, Osp::Base::Runtime::Thread * pEventHolder)
	: __pArrIn(pArrIn), __pArrOut(pArrOut), __pThreadHandler(pEventHolder), __pHttpSeoulBus(null)
{
	__countInvoke = 0;
}

HandleMybus::~HandleMybus()
{
	fini();
}

bool HandleMybus::init()
{
	if (__pArr == null) {
		__pArr = new ArrayList();
		__pArr->Construct();
	}

	for (int groupIndex=0; groupIndex<__pArrIn->GetCount(); groupIndex++) {
		GroupStop * pGroup = static_cast<GroupStop*> (__pArrIn->GetAt(groupIndex));
		for (int itemIndex=0; itemIndex<pGroup->pArr->GetCount(); itemIndex++) {
			MybusItem * pMybus = static_cast<MybusItem*> (pGroup->pArr->GetAt(itemIndex));
			//
			__pArr->Add(*pMybus);
		}
	}

	return true;
}

void HandleMybus::fini()
{
	if (__pArr) {
		__pArr->RemoveAll(false);
		delete __pArr;
		__pArr = null;
	}

	if (__pHttpSeoulBus) {
		delete __pHttpSeoulBus;
		__pHttpSeoulBus = null;
	}
}

void HandleMybus::Initialize()
{
	init();

	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}
}

bool HandleMybus::Invoke()
{
	if (__pArr == null || __pArr->GetCount() == 0) {
		return false;
	}

	int nth = __countInvoke % __pArr->GetCount();

	MybusItem * pMybus = static_cast<MybusItem*> (__pArr->GetAt(nth));

	AppLogDebug("invoke HTTP - GetArrivingBy (%d-th)", nth);

	if (pMybus) {
		__pHttpSeoulBus->GetArrivingBy(pMybus->__stationId, pMybus->__stationOrd, pMybus->__busId);
	}

	__countInvoke++;

	return true;
}

bool HandleMybus::IsOneSet()
{
	return (__countInvoke % __pArr->GetCount() == 0);
}

void HandleMybus::sendEvent(int event, const String & message)
{
	if (__pThreadHandler)
		__pThreadHandler->SendUserEvent(event, null);
}

void HandleMybus::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	//AppLogDebug("%s", pBuffer->GetPointer());

	__parser.ParseXml(pBuffer, __pArrOut);

	sendEvent(EVENT_HANDLER_MYBUS_RECV_GOOD_ONE);

}

void HandleMybus::OnBadResponse(HandleHttp * pHttp, const String & message)
{

}
