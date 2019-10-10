/*
 * FormAxBusLineStops.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusLineStops.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"

FormAxBusLineStops::FormAxBusLineStops() : __httpSeoulBus(null), __pArrStops(null), __pArrPositions(null), __pListViewStops(null), __pContextMenu(null), __pUpdateThread(null)
{
}

FormAxBusLineStops::~FormAxBusLineStops()
{
}

bool FormAxBusLineStops::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	String title = "노선도 - ";

	String * pName = GetStringByKey("노선정보_번호이름");
	if (pName) {
		title.Append(*pName);
	}

	SetTitle(title);

	SetPanelStype(BOTTOM_PANEL);
	//SetPush(NO_PUSH);
	SetRefreshButton(ID_UPDATE_LINE_STOPS);

	return true;
}

result FormAxBusLineStops::OnInitializing(void)
{
	result r = E_SUCCESS;

	String * pKey = GetStringByKey("노선정보_검색키");
	if (pKey) {
		__keyParam = *pKey;
	}

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	Panel * panel = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	createAds(Rectangle(0, y, dimBottomSize.width, 80), panel);
	y += 80;

	/*
	Panel * pPanelBG = makePanel(Rectangle(0,  y, dimBottomSize.width, 80), bottomPanel, "bg/bg_434x170_w.png");
	y += 80;
	makeLabelText(Rectangle(10, 5, dimBottomSize.width-20, 30), "노선 정류소 정보", pPanelBG, 25);
	 */

	//static int nMeetCount = 0;
	//if (nMeetCount == 0) {
	//	makeLabelText(Rectangle(10, 10, dimBottomSize.width-20, 30), "특정항목을 옆으로 살짝 밀어보세요 ...",
	//			pPanelBG, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_LEFT);
	//}
	//nMeetCount++;

	__pListViewStops  = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, false);

/*
	if (__pAd) {
		RequestId reqId  = INVALID_REQUEST_ID;
		__pAd->RequestAd(reqId);
	}
*/

	init();
	invokeQuery();

	return r;
}

result FormAxBusLineStops::OnTerminating(void)
{
	destroyUpdateTherad();

	if (__httpSeoulBus) {
		delete __httpSeoulBus;
	}

	if (__pArrStops) {
		__pArrStops->RemoveAll(true);
		delete __pArrStops;
	}

	if (__pArrPositions) {
		AppLogDebug("number of items = %d", __pArrPositions->GetCount());
		__pArrPositions->RemoveAll(true);
		delete __pArrPositions;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusLineStops::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_UPDATE_LINE_STOPS:
		AppLogDebug("강제로 버스 위치 정보를 업데이트 합니다.");
		__pUpdateThread->UpdateForce(false);
		break;
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_KEYWORD:
		GotoBack();
		break;
	case ID_MESSAGEBOX_NO_DATA:
		GotoBack();
		break;
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	case ID_MESSAGEBOX_NO_STOP:
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

bool FormAxBusLineStops::AtSleep()
{
	AppLogDebug("thread 종료");
	destroyUpdateTherad();
	return FormAxBus::AtSleep();
}

bool FormAxBusLineStops::AtWakeup()
{
	AppLogDebug("thread 생성");
	createUpdateTherad();
	//__pUpdateThread->UpdateForce(true);

	return FormAxBus::AtWakeup();
}

void FormAxBusLineStops::init()
{
	if (__pArrStops == null) {
		__pArrStops = new ArrayList();
		__pArrStops->Construct();
	}

	if (__pArrPositions == null) {
		__pArrPositions = new ArrayList();
		__pArrPositions->Construct();
	}

	if (__httpSeoulBus == null) {
		__httpSeoulBus = new HttpBusSeoul(this);
	}

	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("내 버스",   ID_CONTEXT_MYBUS);
	//BcContextItem * p2 = new BcContextItem("지도위치",   ID_CONTEXT_STOP_MAP);
	//BcContextItem * p3 = new BcContextItem("정류소정보", ID_CONTEXT_STOP_INFO);
	//BcContextItem * p2 = new BcContextItem("자주쓰는 정류소", ID_CONTEXT_FREQ_STOP);
	//BcContextItem * p3 = new BcContextItem("자주쓰는 버스", ID_CONTEXT_FREQ_LINE);
	BcContextItem * p4 = new BcContextItem("정류소 도착버스", ID_CONTEXT_LINESTOPS);
	BcContextItem * p5 = new BcContextItem("지도보기", ID_CONTEXT_MAP);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	//pArrMenu->Add(*p2);
	//pArrMenu->Add(*p3);
	pArrMenu->Add(*p4);
	pArrMenu->Add(*p5);

	//
	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;
}

void FormAxBusLineStops::invokeQuery()
{
	if (__keyParam.IsEmpty()) {
		UtilMessageBox::ConfirmWithCallback("", "노선정보 검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}
	AppLogDebug("노선 정보 검색합니다. 키 = [%S]", __keyParam.GetPointer());

	if (__httpSeoulBus->GetStopsByLine(__keyParam))
		commShow();
}

void FormAxBusLineStops::createUpdateTherad()
{
	if (__pUpdateThread == null) {
		AppLogDebug("@@@ update thread 생성 !!!");

		__pUpdateThread = new ThreadBusPositions(__pArrPositions, this);
		__pUpdateThread->Initialize(__keyParam, 20);

		Fork(__pUpdateThread);
	}
}

void FormAxBusLineStops::destroyUpdateTherad()
{
	if (__pUpdateThread) {
		AppLogDebug("@@@ update thread 파괴 !!!");

		Kill(__pUpdateThread);
		delete __pUpdateThread;
		__pUpdateThread = null;
	}

}

void FormAxBusLineStops::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	bool bOK = __parser.ParseXml(pBuffer, __pArrStops);

	commHide();

	if (bOK) {

		__pListViewStops->UpdateList();

		String * pOrder = GetStringByKey("노선정보_현재정류소");
		if (pOrder) {
			int nOrder = 0;
			Integer::Parse(*pOrder, nOrder);
			nOrder--;

			if (nOrder >= 0)
				__pListViewStops->ScrollToItem(nOrder);
		}

		this->RequestRedraw();

		// 이후에 버스 노선 위치 정보를 조회해야 ...
		createUpdateTherad();
	}
	else {
		UtilMessageBox::ConfirmWithCallback("수신 에러", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}
}

void FormAxBusLineStops::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	if (pHttp == __httpSeoulBus->GetHandler()) {
		commHide();
		UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusLineStops::GetItemCount(void)
{
	if (__pArrStops == null)
		return 0;

	AppLogDebug("number of items = %d", __pArrStops->GetCount());
	return __pArrStops->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxBusLineStops::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	if (__pArrStops) {

		BusSeoul_StopByLine * pCurrStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(index));

		Bitmap * pbmp = loadBitmapPool("appendix/line.png");
		//Bitmap * pbmpBus = loadBitmapPool("appendix/bus_blue.png");
		Bitmap * pbmpBus = GetBitmapBus(pCurrStop->__routeType);

		CustomItem * pItem = FactoryListItem::makeListItem_BusStopByLine(index, itemWidth, pCurrStop, pbmp, pbmpBus);
		pItem->SetContextItem(__pContextMenu);
		return pItem;
	}

	return null;
#endif
}

bool FormAxBusLineStops::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	CustomItem * pTheItem = (CustomItem*) pItem;
	pTheItem->RemoveAllElements();

	delete pItem;
	pItem = null;
	return true;
}

void FormAxBusLineStops::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

	BusSeoul_StopByLine* pSelectedStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(index));
	doContextMenu(elementId, pSelectedStop);
}

void FormAxBusLineStops::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	BusSeoul_StopByLine* pSelectedStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(index));
	doContextMenu(ID_CONTEXT_LINESTOPS, pSelectedStop);
}

void FormAxBusLineStops::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusLineStops::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	BusSeoul_StopByLine* pStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(index));

	String ttsContents;
	ttsContents.Append(pStop->__stationNm);
	ttsContents.Append(" 정류소 입니다.");

	TTS(ttsContents);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusLineStops::doContextMenu(int command, BusSeoul_StopByLine* pStop)
{
	if (pStop == null) {
		AppAssert(0);
		return;
	}

	if (pStop->__stationNo.GetLength() < 2) {
		UtilMessageBox::ConfirmWithCallback("", "정류소 정보가 없습니다.", this, ID_MESSAGEBOX_NO_STOP);
		return;
	}

	AppLogDebug("정류소 코드 = %S", pStop->__stationNo.GetPointer());

	switch (command) {
	case ID_CONTEXT_MYBUS: // 자주쓰는
		AppLogDebug("내 버스로 등록합니다.");
		saveDb(pStop);
		break;
	case ID_CONTEXT_FREQ_STOP:
		saveFreqStop(pStop);
		break;
	case ID_CONTEXT_FREQ_LINE:
		saveFreqLine(pStop);
		break;
	case ID_CONTEXT_MAP:
		SetKeyValue("위치_lat", pStop->__gpsY);
		SetKeyValue("위치_lng", pStop->__gpsX);
		SetKeyValue("위치_info", pStop->__stationNm);
		SetKeyValue("위치_title", pStop->__stationNm);

		GotoForm(FORM_ID_MAP_CENTER);
		break;
	case ID_CONTEXT_LINESTOPS: // 정류소
		SetKeyValue("정류소검색_키", pStop->__stationNo);
		GotoForm(FORM_ID_BUS_STOPLINES);
		break;
	}

}

void FormAxBusLineStops::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	switch (requestId) {
	case EVENT_BUSPOS_THREAD_COMM_SHOW:
		commShow(true);
		return;
	case EVENT_BUSPOS_THREAD_COMM_HIDE:
		commHide(true);
		return;
	case EVENT_BUSPOS_THREAD_UPDATED:
		break;
	default:
		return;
	}

	//if (requestId != EVENT_BUSPOS_THREAD_UPDATED) {
	//	return;
	//}

	// clear the association, first!
	for (int i=0; i<__pArrStops->GetCount(); i++) {
		BusSeoul_StopByLine * pStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(i));
		//pStop->__pPositionInfo = null;
		pStop->__pArrPositionInfo->RemoveAll(false);
	}

	AppLogDebug("new data arrived! associate two array data and refresh !");
	int countRunning = 0;

	for (int i=0; i<__pArrPositions->GetCount(); i++) {
		BusSeoul_BusPosition * pNow = static_cast<BusSeoul_BusPosition*>(__pArrPositions->GetAt(i));

		if (pNow->__isrunyn == "0") {
			continue;
		}

		int index = -1;
		Integer::Parse(pNow->__sectOrd, index);
		index--;

		if (index < 0) {
			AppLogDebug("############이러면 안 되는데 ... ############");
			continue;
		}

		countRunning++;

		BusSeoul_StopByLine * pTheStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(index));
		//pTheStop->__pPositionInfo = pNow;
		pTheStop->__pArrPositionInfo->Add(*pNow);
	}

	AppLogDebug("new data arrived! %d bus(es) are running!", countRunning);

	__pListViewStops->UpdateList();

	this->RequestRedraw();
}

void FormAxBusLineStops::saveDb(BusSeoul_StopByLine* pstop)
{
	DatabaseBus db;

	bool bOK = db.InsertMyBus(pstop->__station, pstop->__seq, pstop->__stationNm, pstop->__stationNo, pstop->__busRouteId, pstop->__busRouteNm);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}

void FormAxBusLineStops::saveFreqStop(BusSeoul_StopByLine* pstop)
{
	/*
	DatabaseBus db;
	bool bOK = db.InsertStop(parrv->__arsId, parrv->__stId, parrv->__stNm, parrv->__stationTp);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "정류소정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "정류소정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
*/

}

void FormAxBusLineStops::saveFreqLine(BusSeoul_StopByLine* pstop)
{
	DatabaseBus db;
	bool bOK = db.InsertLine(pstop->__busRouteId, pstop->__busRouteNm, pstop->__routeType, "", "", "");

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}

