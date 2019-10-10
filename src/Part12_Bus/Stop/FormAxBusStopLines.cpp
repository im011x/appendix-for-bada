/*
 * FormAxBusStopLines.cpp
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#include "FormAxBusStopLines.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"
#include "UtilApp.h"

FormAxBusStopLines::FormAxBusStopLines() : __pListLines(null), __pArrGroups(null), __pArrBusLines(null)
	, __pBusStopLinesInvoker(null), __pContextMenu(null)
{
	__pLabelStopName = null;
	__pUpdateThread = null;
}

FormAxBusStopLines::~FormAxBusStopLines()
{
}

bool FormAxBusStopLines::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("정류소 도착버스");
	SetPanelStype(BOTTOM_PANEL);

	SetRefreshButton(ID_UPDATE_STOP_LINES);

	return true;
}

result FormAxBusStopLines::OnInitializing(void)
{
	result r = E_SUCCESS;

	String stopCode = "정류소 코드";
	String * pKey = GetStringByKey("정류소검색_키");
	if (pKey) {
		stopCode = *pKey;
		if (stopCode.GetLength()>2) {
			stopCode.Insert('-', 2);
			stopCode.Insert('[', 0);
			stopCode.Append(']');
		}

		__keyParam = *pKey;
	}

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int w = dimBottomSize.width;
	int y = 0;

	Panel * pPanelBG = makePanel(Rectangle(0,  0, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white_arrow.png");

//	makeButton(Rectangle(w/3*0+10,  y, w/3-20, 50), "지도보기", pPanelBG, ID_COMMAND_MAP_THE_STOP, this);
//	makeButton(Rectangle(w/3*1+10,  y, w/3-20, 50), "자주쓰는", pPanelBG, ID_COMMAND_ADD_FREQ_STOP, this);
//	makeButton(Rectangle(w/3*2+10,  y, w/3-20, 50), "시간표",  pPanelBG, ID_COMMAND_STOP_TIMETABLE, this);
//	y += 60;

	// 나중에 overwrite !
//	makeLabelText(Rectangle(10, y, 100, 50), stopCode, pPanelBG, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);

	__pLabelStopName = makeLabelText(Rectangle(10, y, w-20, 50), "정류소 명칭", pPanelBG, 40, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 50;

	makeLabelText(Rectangle(10, y, w/2, 30), stopCode, pPanelBG, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 30;

	makeButtonPool(Rectangle(0,  0, dimBottomSize.width, 80), "appendix/transparent.png", "", "", pPanelBG, ID_COMMAND_STOP_TIMETABLE, this);  // 투명 버튼!

	// create list view
	__pListLines = makeGroupedListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), this, this, this, true);

	RegistryGet("TapOnStopline", __nTapStoplineGoto);
	init();
	invokeQuery();

	return r;
}

void FormAxBusStopLines::init()
{
	if (__pArrBusLines == null) {
		__pArrBusLines = new ArrayList();
		__pArrBusLines->Construct();
	}

	if (__pArrGroups == null) {
		__pArrGroups = new ArrayList();
		__pArrGroups->Construct();
	}

	if (__pBusStopLinesInvoker == null) {
		__pBusStopLinesInvoker = new HandleBusStopLines(__pArrBusLines, __pArrGroups, this);
	}

	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("내 버스",   ID_CONTEXT_MYBUS);
	//BcContextItem * p2 = new BcContextItem("노선정보", ID_CONTEXT_LINEINFO);
	//ID_CONTEXT_FREQ_STOP, ID_CONTEXT_FREQ_LINE
	//BcContextItem * p4 = new BcContextItem("노선지도", ID_CONTEXT_LINEMAP);

	BcContextItem * p2 = new BcContextItem("자주쓰는 정류소", ID_CONTEXT_FREQ_STOP);
	BcContextItem * p3 = new BcContextItem("자주쓰는 버스", ID_CONTEXT_FREQ_LINE);
	BcContextItem * p4 = new BcContextItem("노선도", ID_CONTEXT_LINESTOPS);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);
	pArrMenu->Add(*p3);
	pArrMenu->Add(*p4);

	//
	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;

}

void FormAxBusStopLines::invokeQuery()
{
	if (__keyParam.IsEmpty()) {
		UtilMessageBox::ConfirmWithCallback("", "검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}

	__pBusStopLinesInvoker->Invoke(__keyParam);
}

result FormAxBusStopLines::OnTerminating(void)
{
	destroyUpdateTherad();

	if (__pBusStopLinesInvoker) {
		delete __pBusStopLinesInvoker;
		__pBusStopLinesInvoker = null;
	}

	if (__pArrBusLines) {
		__pArrBusLines->RemoveAll(true);
		delete __pArrBusLines;
	}

	if (__pArrGroups) {
		__pArrGroups->RemoveAll(false);
		delete __pArrGroups;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusStopLines::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_UPDATE_STOP_LINES:
		if (__pUpdateThread) {
			AppLogDebug("강제로 업데이트 합니다.");
			__pUpdateThread->UpdateForce(false);
		}
		break;
	case ID_COMMAND_STOP_TIMETABLE:
	{
		SetKeyValue("정류소검색_이름", __pLabelStopName->GetText());
		SetKeyObject("정류소정보_Array", __pArrBusLines);
		GotoForm(FORM_ID_BUS_STOPINFO);
	}
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
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

/*

void FormAxBusStopLines::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
}

void FormAxBusStopLines::OnBadResponse(HandleHttp * pHttp, const String & message)
{
}
*/

bool FormAxBusStopLines::AtSleep()
{
	destroyUpdateTherad();

	return FormAxBus::AtSleep();
}

bool FormAxBusStopLines::AtWakeup()
{
	createUpdateTherad();
	__pUpdateThread->UpdateForce(true);

	return FormAxBus::AtWakeup();
}

void FormAxBusStopLines::updateStopName()
{
	BusSeoul_ArrivingInfo* pArrv0 = static_cast<BusSeoul_ArrivingInfo*> (__pArrBusLines->GetAt(0));
	__pLabelStopName->SetText(pArrv0->__stNm);
}

void FormAxBusStopLines::createUpdateTherad()
{
	if (__pUpdateThread == null) {
		AppLogDebug("@@@ update thread 생성 !!!");

		__pUpdateThread = new ThreadBusStopLines(__pArrBusLines, __pArrGroups);
		__pUpdateThread->Initialize(__keyParam, 20, this);
		Fork(__pUpdateThread);
	}
}

void FormAxBusStopLines::destroyUpdateTherad()
{
	if (__pUpdateThread) {
		AppLogDebug("@@@ update thread 파괴 !!!");

		Kill(__pUpdateThread);
		delete __pUpdateThread;
		__pUpdateThread = null;
	}
}

void FormAxBusStopLines::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	String message;

	if (pArgs) {
		message = *(static_cast<String *> (pArgs->GetAt(0)));
		pArgs->RemoveAll(true);
		delete pArgs;
	}

	AppLogDebug("received an event (%d) with message = [%S]", requestId, message.GetPointer());

	switch (requestId) {
	case EVENT_BUSSTOP_ARRIVE_SENT_GOOD:
		commShow();
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_GOOD:
		commHide();
		updateStopName();
		__pListLines->UpdateList();
		__pListLines->ScrollToItem(0, -1);  // 왜 뜨자마자 마지막으로 가지 ?

		createUpdateTherad();

		CreateTimer(5, 0);
		this->RequestRedraw();
		break;
	case EVENT_BUSSTOP_ARRIVE_SENT_BAD:
		UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_NO:
		UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_NO_DATA);
		break;
	case EVENT_BUSSTOP_ARRIVE_RECV_BAD:
		UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
		break;
	case EVENT_BUSSTOP_THREAD_COMM_SHOW:
		commShow(true);
		break;
	case EVENT_BUSSTOP_THREAD_COMM_HIDE:
		commHide(true);
		break;
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusStopLines::GetGroupCount(void)
{
	if (__pArrBusLines == null)
		return 0;

	if (__pArrGroups == null)
		return 0;

	return __pArrGroups->GetCount();
}

int FormAxBusStopLines::GetItemCount(int groupIndex)
{
	if (__pArrBusLines == null)
		return 0;

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	return pGroup->pArr->GetCount();

}

Osp::Ui::Controls::GroupItem* FormAxBusStopLines::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new GroupItem();
	pItem->Construct(Dimension(itemWidth, 50));

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));

	String nameGroup;
	nameGroup.Format(255, L"%S (%d 대)", pGroup->sName.GetPointer(), pGroup->pArr->GetCount());
	pItem->SetElement(nameGroup, null);

	return pItem;
}

bool FormAxBusStopLines::DeleteGroupItem(int groupIndex, Osp::Ui::Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

Osp::Ui::Controls::ListItemBase* FormAxBusStopLines::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_ArrivingInfo* p = static_cast<BusSeoul_ArrivingInfo*> (pGroup->pArr->GetAt(itemIndex));

	Bitmap * pbmpBus = GetBitmapBus(p->__routeType);

	CustomItem * pItem = FactoryListItem::makeListItem_StopArriveInfo(itemIndex, itemWidth,  p, pbmpBus);
	pItem->SetContextItem(__pContextMenu);
	return pItem;
}

bool FormAxBusStopLines::DeleteItem(int groupIndex, int itemIndex, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}


void FormAxBusStopLines::OnGroupedListViewContextItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("Grouped List element = %d and group/item = %d/%d", elementId, groupIndex, itemIndex);

	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_ArrivingInfo* pSelectedLine = static_cast<BusSeoul_ArrivingInfo*> (pGroup->pArr->GetAt(itemIndex));

	doContextMenu(elementId, pSelectedLine);
}

void FormAxBusStopLines::OnGroupedListViewItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListItemStatus state)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_ArrivingInfo* pSelectedLine = static_cast<BusSeoul_ArrivingInfo*> (pGroup->pArr->GetAt(itemIndex));

	switch (__nTapStoplineGoto) {  // setup 설정 내용에 따라 이동
	case 0: // 이동없음
		doTTS(groupIndex, itemIndex);
		break;
	case 1: // 노선도 - 위치정보로 이동
	default:
		doContextMenu(ID_CONTEXT_LINESTOPS, pSelectedLine);
		break;
	}

}

void FormAxBusStopLines::OnGroupedListViewItemSwept(Osp::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusStopLines::OnGroupedListViewItemLongPressed(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
	doTTS(groupIndex, itemIndex);
}

void FormAxBusStopLines::doTTS(int groupIndex, int itemIndex)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_ArrivingInfo* pArv = static_cast<BusSeoul_ArrivingInfo*> (pGroup->pArr->GetAt(itemIndex));

	TimeSpan tmSpan1 = pArv->__datetime1.GetTime();
	TimeSpan tmSpanNow = UtilApp::GetNow().GetTime();
	tmSpan1 = tmSpan1 - tmSpanNow;
	int nTime1 = tmSpan1.GetTicks() / 1000;

	String ttsContents = "버스 ";
	ttsContents.Append(pArv->__rtNm);
	ttsContents.Append(" 번이 ");

	ttsContents.Append(pArv->__stNm);
	ttsContents.Append(" 정류소에 ");

	if (pArv->__stationNm1.IsEmpty() || pArv->__vehId1 == "0") {
		ttsContents.Append("도착예정 버스 없습니다.");
	}
	else if (nTime1 < 0) {
		ttsContents.Append("곧 도착하거나 이미 지나갔습니다");
	}
	else if (nTime1 < 60) {
		ttsContents.Append(nTime1%60);
		ttsContents.Append(" 초 후에 도착 예정입니다.");
	}
	else {
		String s;
		s.Format(80, L"%d 분 %d 초 후 도착 예정입니다.", nTime1/60, nTime1%60);
		ttsContents.Append(s);
	}

	//AppLogDebug("%S", ttsContents.GetPointer());
	TTS(ttsContents);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusStopLines::AtTimer(HandleTimer * pTimerHandler)
{
	__pListLines->UpdateList();
	__pListLines->RequestRedraw();
}

void FormAxBusStopLines::doContextMenu(int command, BusSeoul_ArrivingInfo* pline)
{
	switch (command) {
	case ID_CONTEXT_MYBUS:
		saveMybus(pline);
		break;
	//case ID_CONTEXT_LINEINFO:
	//	break;
	case ID_CONTEXT_FREQ_STOP:
		saveFreqStop(pline);
		break;
	case ID_CONTEXT_FREQ_LINE:
		saveFreqLine(pline);
		break;
	case ID_CONTEXT_LINESTOPS:
		SetKeyValue("노선정보_검색키", pline->__busRouteId);
		SetKeyValue("노선정보_번호이름", pline->__rtNm);
		SetKeyValue("노선정보_현재정류소", pline->__staOrd);
		GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	//case ID_CONTEXT_LINEMAP:
	//	break;
	}
}

void FormAxBusStopLines::saveMybus(BusSeoul_ArrivingInfo* parrivs)
{
	DatabaseBus db;

	bool bOK = db.InsertMyBus(parrivs->__stId, parrivs->__staOrd, parrivs->__stNm, parrivs->__arsId, parrivs->__busRouteId, parrivs->__rtNm);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}

void FormAxBusStopLines::saveFreqStop(BusSeoul_ArrivingInfo* parrv)
{
	DatabaseBus db;
	bool bOK = db.InsertStop(parrv->__arsId, parrv->__stId, parrv->__stNm, parrv->__stationTp);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "정류소정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "정류소정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}

void FormAxBusStopLines::saveFreqLine(BusSeoul_ArrivingInfo* pstop)
{
	DatabaseBus db;
	bool bOK = db.InsertLine(pstop->__busRouteId, pstop->__rtNm, pstop->__routeType, pstop->__term, "", "");

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}
