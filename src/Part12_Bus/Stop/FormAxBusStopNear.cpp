/*
 * FormAxBusStopNear.cpp
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#include "FormAxBusStopNear.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"

FormAxBusStopNear::FormAxBusStopNear() : __pLabelRadius(null), __pLabelTotal(null), __pSliderDistance(null), __pListStops(null),
	__pArrStops(null), __pHttpSeoulBus(null), __pContextMenu(null)
{
	__currLat = "";
	__currLng = "";
	__sDistanceNow = "500";
}

FormAxBusStopNear::~FormAxBusStopNear()
{
}

bool FormAxBusStopNear::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("주변 정류소");
	SetPanelStype(BOTTOM_PANEL);

	SetRefreshButton(ID_UPDATE_NEAR_STOP);

	return true;
}

result FormAxBusStopNear::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();
	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	// 범위를 설정할 수 있는 컨트롤 추가 한다.
	Panel * panel = makePanel(Rectangle(0, 0, dimBottomSize.width, 140), bottomPanel, "appendix/bg_white.png", true);

	int unit = panel->GetWidth()/5;
	int x = 10;

	makeLabelText(Rectangle(x, y, unit-10, 100), "반경 :", panel);
	x += (unit-10) + 10;
	__pSliderDistance = makeSlider(Rectangle(x, y, unit*3-20, 100), panel, 1, 15, this, this);
	x += (unit*3) - 10;
	__pLabelRadius = makeLabelText(Rectangle(x, y, unit, 100), (__sDistanceNow + " m"), panel);

	y += 100;
	__pLabelTotal = makeLabelText(Rectangle(10, y, unit*5-20, 30), "검색결과를 알려드립니다..." , panel);
	__pLabelTotal->SetTextHorizontalAlignment(ALIGNMENT_LEFT);

	// fix the y
	y = 140;

	__pListStops = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);

	//
	__pSliderDistance->SetValue(5);

	init();
	invokeQuery();

	return r;
}

result FormAxBusStopNear::OnTerminating(void)
{
	if (__pArrStops) {
		__pArrStops->RemoveAll(true);
		delete __pArrStops;
	}

	if (__pHttpSeoulBus) {
		delete __pHttpSeoulBus;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusStopNear::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_UPDATE_NEAR_STOP:
		__currLat = __currPosLatitude;
		__currLng = __currPosLongitude;
		invokeQuery(true);
		break;
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_KEYWORD:
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	case ID_MESSAGEBOX_NO_DATA:
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxBusStopNear::init()
{
	//
	String * pLat = GetStringByKey("정류소검색_위치_lat");
	String * pLng = GetStringByKey("정류소검색_위치_lng");

	if (pLat == null || pLng == null) {
		UtilMessageBox::ConfirmWithCallback("", "파라미터 - 위치 정보가 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}

	__currLat = *pLat;
	__currLng = *pLng;

	AppLogDebug("현재 검색할 위도/경도 = %S/%S", __currLat.GetPointer(), __currLng.GetPointer());

	if (__pArrStops == null) {
		__pArrStops = new ArrayList();
		__pArrStops->Construct();
	}

	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
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

void FormAxBusStopNear::invokeQuery(bool update)
{
	if (__currLat.IsEmpty() || __currLng.IsEmpty()) {
		return;
	}

	if (__pArrStops && update) {
		__pArrStops->RemoveAll(true);
	}

	if (__pHttpSeoulBus->GetStopsByPosition(__currLat, __currLng, __sDistanceNow)) {
		commShow();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusStopNear::computeDistance()
{
	float fCurrLat, fCurrLng;
	float fNowLat, fNowLng;
	float fCalcLat, fCalcLng;

	Float::Parse(AppBaseForm::__currPosLatitude, fCurrLat);
	Float::Parse(AppBaseForm::__currPosLongitude, fCurrLng);

	//AppLogDebug("compute the distance - curr lat/lng = %.6f/%.6f", fCurrLat, fCurrLng);

	for (int i=0; i<__pArrStops->GetCount(); i++) {
		BusSeoul_StopInfo * pStop = static_cast<BusSeoul_StopInfo*> (__pArrStops->GetAt(i));

		Float::Parse(pStop->__gpsY, fNowLat);
		Float::Parse(pStop->__gpsX, fNowLng);

		//AppLogDebug("  > compute the distance - stop[%d]'s lat/lng = %.6f/%.6f", i, fNowLat, fNowLng);

		fCalcLat = fNowLat - fCurrLat;
		fCalcLng = fNowLng - fCurrLng;

		fCalcLat *= 10000;
		fCalcLng *= 10000;
		//AppLogDebug("  > compute the distance - the difference = %.6f/%.6f", fCalcLat, fCalcLng);

		fCalcLat *= fCalcLat;
		fCalcLng *= fCalcLng;

		//AppLogDebug("  > compute the distance - the difference square = %.6f/%.6f", fCalcLat, fCalcLng);

		pStop->__distance = fCalcLat + fCalcLng;
		pStop->__distance = Osp::Base::Utility::Math::Sqrt(pStop->__distance);
		//AppLogDebug("  > compute the distance [%S] - %.6f", pStop->__arsId.GetPointer(), pStop->__distance);
	}

}

void FormAxBusStopNear::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	bool bOK = __parser.ParseXml(pBuffer, __pArrStops);
	commHide();
	computeDistance();

	if (bOK) {
		CompareBusSeoul_StopNear * pCompare = new CompareBusSeoul_StopNear();
		__pArrStops->Sort(*pCompare);
		delete pCompare;

		String sCount;
		sCount.Format(255, L"전체 %d 개의 정류소가 검색되었습니다.", __pArrStops->GetCount());
		__pLabelTotal->SetText(sCount);
	}
	else {
		UtilMessageBox::ConfirmWithCallback("", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}

	__pListStops->UpdateList();

	this->RequestRedraw();
}

void FormAxBusStopNear::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();

	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusStopNear::GetItemCount(void)
{
	if (__pArrStops == null)
		return 0;

	return __pArrStops->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxBusStopNear::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_StopNear(index, itemWidth,  static_cast<BusSeoul_StopInfo*> (__pArrStops->GetAt(index)));
	pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool FormAxBusStopNear::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusStopNear::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrStops->GetAt(index));
	doContextMenu(elementId, pStop);
}

void FormAxBusStopNear::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrStops->GetAt(index));
	doContextMenu(ID_CONTEXT_STOP_ARRV, pStop);
}

void FormAxBusStopNear::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusStopNear::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	BusSeoul_StopInfo* pStop= static_cast<BusSeoul_StopInfo*> (__pArrStops->GetAt(index));

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

void FormAxBusStopNear::OnAdjustmentValueChanged(const Osp::Ui::Control& source, int adjustment)
{
	AppLogDebug("adjusted value = %d", adjustment);

	__sDistanceNow.Format(64, L"%d", adjustment * 100);

	__pLabelRadius->SetText(__sDistanceNow + " m");
	__pLabelRadius->RequestRedraw();

	invokeQuery(true);
}

void FormAxBusStopNear::OnSliderBarMoved(Osp::Ui::Controls::Slider& source, int value)
{
	AppLogDebug("moved value = %d", value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

//void FormAxBusStopNear::gotoBusStop(int index)
//{
//	BusSeoul_StopSimple * pStop = static_cast<BusSeoul_StopSimple*> (__pArrStops->GetAt(index));
//
//	SetKeyValue("정류소검색_키", pStop->__arsId);
//	GotoForm(FORM_ID_BUS_STOPLINES);
//}

void FormAxBusStopNear::doContextMenu(int command, BusSeoul_StopInfo* pstop)
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

void FormAxBusStopNear::saveDb(BusSeoul_StopInfo* pstop)
{
	DatabaseBus db;

	bool bOK = db.InsertStop(pstop->__arsId, pstop->__stationId, pstop->__stationNm, pstop->__stationTp);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}
