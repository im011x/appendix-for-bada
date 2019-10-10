/*
 * FormAxBusStopNames.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusStopNames.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"

FormAxBusStopNames::FormAxBusStopNames() : __pListStops(null), __pArrBusStops(null), __pHttpSeoulBus(null), __pContextMenu(null)
{
}

FormAxBusStopNames::~FormAxBusStopNames()
{
}

bool FormAxBusStopNames::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	String title = "정류소";

	String * pKey = GetStringByKey("정류소검색_키");
	if (pKey) {
		title.Append(" - ");
		title.Append(*pKey);
	}
	else {
		title.Append(" 이름");
	}
	SetTitle(title);

	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusStopNames::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// create list view
	__pListStops = makeListView(Rectangle(0, 0, dimBottomSize.width, dimBottomSize.height), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);

	// invoke query
	init();
	invokeQuery();

	return r;
}

void FormAxBusStopNames::init()
{
	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}

	if (__pArrBusStops == null) {
		__pArrBusStops = new ArrayList();
		__pArrBusStops->Construct();
	}

	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("자주쓰는", ID_CONTEXT_FAVORATE);
	BcContextItem * p2 = new BcContextItem("지도보기", ID_CONTEXT_MAP);
	BcContextItem * p3 = new BcContextItem("도착정보", ID_CONTEXT_STOP_ARRV);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);
	pArrMenu->Add(*p3);

	//
	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;
}

void FormAxBusStopNames::invokeQuery()
{
	String * pKey = GetStringByKey("정류소검색_키");
	if (pKey == null) {
		UtilMessageBox::ConfirmWithCallback("", "검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}

	if (__pHttpSeoulBus->GetStopsByName(*pKey))
		commShow();
}

result FormAxBusStopNames::OnTerminating(void)
{
	if (__pHttpSeoulBus) {
		delete __pHttpSeoulBus;
	}

	if (__pArrBusStops) {
		__pArrBusStops->RemoveAll(true);
		delete __pArrBusStops;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusStopNames::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_NO_KEYWORD:
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxBusStopNames::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	//String invokedUri = pHttp->GetRequestedUrl();
	//AppLogDebug("@@@ invoked Url = [%S]", invokedUri.GetPointer());

	bool bOK = __parser.ParseXml(pBuffer, __pArrBusStops);

	commHide();

	if (bOK) {
		CompareBusSeoul_StopName * pCompare = new CompareBusSeoul_StopName();
		__pArrBusStops->Sort(*pCompare);
		delete pCompare;

		__pListStops->UpdateList();
		this->RequestRedraw();
	}
	else {
		UtilMessageBox::ConfirmWithCallback("", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}

}

void FormAxBusStopNames::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

int FormAxBusStopNames::GetItemCount(void)
{
	if (__pArrBusStops == null)
		return 0;

	return __pArrBusStops->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxBusStopNames::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	if (__pArrBusStops) {
		CustomItem * pItem = FactoryListItem::makeListItem_StopNear(index, itemWidth,  static_cast<BusSeoul_StopInfo*> (__pArrBusStops->GetAt(index)), false);
		pItem->SetContextItem(__pContextMenu);
		return pItem;
	}

	return null;
#endif
}

bool FormAxBusStopNames::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	CustomItem * pTheItem = (CustomItem*) pItem;
	pTheItem->RemoveAllElements();

	delete pItem;
	pItem = null;
	return true;
}

void FormAxBusStopNames::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("index = %d", index);

	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrBusStops->GetAt(index));
	doContextMenu(elementId, pStop);
}

void FormAxBusStopNames::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrBusStops->GetAt(index));
	doContextMenu(ID_CONTEXT_STOP_ARRV, pStop);
}

void FormAxBusStopNames::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusStopNames::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrBusStops->GetAt(index));

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

//void FormAxBusStopNames::gotoBusStop(int index)
//{
//	BusSeoul_StopSimple * pStop = static_cast<BusSeoul_StopSimple*> (__pArrBusStops->GetAt(index));
//
//	SetKeyValue("정류소검색_키", pStop->__arsId);
//	GotoForm(FORM_ID_BUS_STOPLINES);
//}

void FormAxBusStopNames::doContextMenu(int command, BusSeoul_StopInfo* pstop)
{
	switch (command) {
	case ID_CONTEXT_FAVORATE:
		saveDb(pstop);
		break;
	case ID_CONTEXT_MAP:
		SetKeyValue("위치_lat", pstop->__gpsY);
		SetKeyValue("위치_lng", pstop->__gpsX);
		SetKeyValue("위치_info", pstop->__stationNm);
		SetKeyValue("위치_title", pstop->__stationNm);

		GotoForm(FORM_ID_MAP_CENTER);
		break;
	case ID_CONTEXT_STOP_ARRV:
		SetKeyValue("정류소검색_키", pstop->__arsId);
		GotoForm(FORM_ID_BUS_STOPLINES);
		break;
	}
}

void FormAxBusStopNames::saveDb(BusSeoul_StopInfo* pstop)
{
	DatabaseBus db;

	bool bOK = db.InsertStop(pstop->__arsId, pstop->__stationId, pstop->__stationNm, pstop->__stationTp);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}
