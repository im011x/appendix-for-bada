/*
 * FormAxBusAboutMybus.cpp
 *
 *  Created on: 2012. 8. 9.
 *      Author: benny
 */

#include "FormAxBusAboutMybus.h"
#include "FactoryListItem.h"
#include "UtilApp.h"

FormAxBusAboutMybus::FormAxBusAboutMybus() : __pDbBus(null), __pList(null), __pArrCandidates(null), __pArrAnswer(null), __pArrShowGroup(null),
	__pThreadMybus(null), __pContextMenu(null), __pTimerRefresh(null)
{
}

FormAxBusAboutMybus::~FormAxBusAboutMybus()
{
}

bool FormAxBusAboutMybus::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("내 버스");
	SetPanelStype(BOTTOM_PANEL);
	SetRefreshButton(ID_UPDATE_MYBUS);

	return true;
}

result FormAxBusAboutMybus::OnInitializing(void)
{
	result r = E_SUCCESS;

	//
	init();

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	int y = 0;
	Panel * panel = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	createAds(Rectangle(0, y, dimBottomSize.width, 80), panel);
	y += 80;

	__pList = makeGroupedListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), this, this, this, true);
	__pList->SetTextOfEmptyList("등록된 내버스 정보가 없습니다.");


	RegistryGet("TapOnMybus", __nTapMybusGoto);
	invokeQuery();
	__pTimerRefresh = CreateTimer(5, 0);

	return r;
}

void FormAxBusAboutMybus::init()
{
	if (__pArrCandidates == null) {
		__pArrCandidates = new ArrayList();
		__pArrCandidates->Construct();
	}

	if (__pArrAnswer == null) {
		__pArrAnswer = new ArrayList();
		__pArrAnswer->Construct();
	}

	if (__pDbBus == null) {
		__pDbBus = new DatabaseBus();
	}
	__pDbBus->GetUinqueStopsMyBusN(__pArrCandidates);

	if (__pArrShowGroup == null) {
		__pArrShowGroup = new ArrayList();
		__pArrShowGroup->Construct();

		for (int i=0; i<__pArrCandidates->GetCount(); i++) {  // copy~
			MybusGroup * pBusStop = new MybusGroup();
			{
				GroupStop * pStop = static_cast<GroupStop*> (__pArrCandidates->GetAt(i));
				pBusStop->__arsId = pStop->sId2;
				pBusStop->__stationId = pStop->sId;
				pBusStop->__stationNm = pStop->sName;
			}
			__pArrShowGroup->Add(*pBusStop);
		}
	}

	if (__pContextMenu == null) {
		// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
		BcContextItem * p1 = new BcContextItem("삭제",   ID_CONTEXT_MYBUS_DELETE);
		BcContextItem * p2 = new BcContextItem("노선도", ID_CONTEXT_LINESTOPS);
		BcContextItem * p3 = new BcContextItem("정류소", ID_CONTEXT_STOP_INFO);

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
}

void FormAxBusAboutMybus::invokeQuery()
{
	createMybusTherad();
}

void FormAxBusAboutMybus::createMybusTherad()
{
	if (__pThreadMybus == null) {
		__pThreadMybus = new ThreadMybus(__pArrCandidates, __pArrAnswer, this);
		__pThreadMybus->Initialize(20);
		Fork(__pThreadMybus);
	}
}

void FormAxBusAboutMybus::destroyMybusTherad()
{
	if (__pThreadMybus) {
		Kill(__pThreadMybus);
		delete __pThreadMybus;
		__pThreadMybus = null;
	}
}


bool FormAxBusAboutMybus::AtSleep()
{
	AppLog("AtSleep()");
	destroyMybusTherad();

	return FormAxBus::AtSleep();
}

bool FormAxBusAboutMybus::AtWakeup()
{
	AppLog("AtWakeup()");
	InitIntent();

	createMybusTherad();
	__pThreadMybus->UpdateForce(true);

	return FormAxBus::AtWakeup();
}

result FormAxBusAboutMybus::OnTerminating(void)
{
	AppLogDebug("");

	destroyMybusTherad();

	if (__pArrCandidates) {
		__pArrCandidates->RemoveAll(true);
		delete __pArrCandidates;
		__pArrCandidates = null;
	}

	if (__pArrAnswer) {
		__pArrAnswer->RemoveAll(true);
		delete __pArrAnswer;
		__pArrAnswer = null;
	}

	if (__pArrShowGroup) {
		__pArrShowGroup->RemoveAll(true);
		delete __pArrShowGroup;
		__pArrShowGroup = null;
	}

	if (__pDbBus) {
		delete __pDbBus;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusAboutMybus::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
	case ID_UPDATE_MYBUS:
		if (__pThreadMybus) {
			AppLogDebug("강제로 업데이트 합니다.");
			__pThreadMybus->UpdateForce(false);
		}
		break;
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_DATA:
		break;
	case ID_MESSAGEBOX_UPDATE:
		SetPush(NO_PUSH);  // 다시 로드하기 전에 스택에 쌓지 않도록 하고 ...
		GotoForm(GetFormID()); // 자시 자신의 폼을 다시 로드한다~
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

MybusGroup * FormAxBusAboutMybus::getGroup(BusSeoul_ArrivingByLine * pArriv)
{
	for (int i=0; i<__pArrShowGroup->GetCount(); i++) {
		MybusGroup * pThe = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(i));

		if (pThe->__arsId == pArriv->__arsId) {
			return pThe;
		}
	}

	return null;
}

bool FormAxBusAboutMybus::addGroup(BusSeoul_ArrivingByLine * pArriv, int position)
{
	MybusGroup * pGroup = getGroup(pArriv);

	if (pGroup == null) {
		AppLogDebug("이러면 안되는데 ...");
		return false;
	}

	ArrayList * pArrPos = pGroup->__pArr;

	//AppLogDebug("addGroup 전, 정류소그룹이 (%S) 가진 버스 항목 갯수 = %d/ 현재 index = %d", pArriv->__arsId.GetPointer(), pArrPos->GetCount(), position);

	bool bExist = false;

	for (int i=0; i<pArrPos->GetCount(); i++) {
		MybusGroupIndex * pBusIndexer = static_cast<MybusGroupIndex*> (pArrPos->GetAt(i));
		if (pBusIndexer->__indexArrivingArr.ToInt() == position) {
			bExist = true;
			break;
		}
	}

	if (bExist == false) {
		MybusGroupIndex * pIndexer = new MybusGroupIndex();
		pIndexer->__indexArrivingArr = position;
		pArrPos->Add(*pIndexer);
	}

	//AppLogDebug("addGroup 후, 정류소그룹이 (%S) 가진 버스 항목 갯수 = %d/ 현재 index = %d", pArriv->__arsId.GetPointer(), pArrPos->GetCount(), position);

	return true;
}

void FormAxBusAboutMybus::OnAnswer()
{
	for (int i=0; i<__pArrAnswer->GetCount(); i++) {
		BusSeoul_ArrivingByLine * pNewArrivInfo = static_cast<BusSeoul_ArrivingByLine*> (__pArrAnswer->GetAt(i));
		addGroup(pNewArrivInfo, i);
	}

	AppLogDebug("update list and refresh entire ... ");
	__pList->UpdateList();
	this->RequestRedraw(); // 뺑뺑이가 돌았으면 화면 전체를 다시 그린다~
}

void FormAxBusAboutMybus::OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs)
{
	switch (requestId) {
	case EVENT_HANDLER_MYBUS_RECV_GOOD_ONE:
		OnAnswer();
		break;
	case EVENT_HANDLER_MYBUS_THREAD_COMM_SHOW:
		commShow(true);
		break;
	case EVENT_HANDLER_MYBUS_THREAD_COMM_HIDE:
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

int FormAxBusAboutMybus::GetGroupCount(void)
{
	if (__pArrShowGroup == null)
		return 0;

	return __pArrShowGroup->GetCount();
}

int FormAxBusAboutMybus::GetItemCount(int groupIndex)
{
	if (__pArrShowGroup == null)
		return 0;

	MybusGroup * pGroup = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(groupIndex));
	return pGroup->__pArr->GetCount();
}

Osp::Ui::Controls::GroupItem* FormAxBusAboutMybus::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new GroupItem();
	pItem->Construct(Dimension(itemWidth, 50));

	MybusGroup * pGroup = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(groupIndex));

	String ars = pGroup->__arsId;
	if (ars.GetLength() > 2) {
		ars.Insert('-', 2);
	}

	String nameGroup;
	nameGroup.Format(255, L"[%S] %S (%d 대)", ars.GetPointer(), pGroup->__stationNm.GetPointer(), pGroup->__pArr->GetCount());
	//nameGroup.Format(255, L"%S [%S]", pGroup->__stationNm.GetPointer(), ars.GetPointer());
	pItem->SetElement(nameGroup, null);

	return pItem;
}

bool FormAxBusAboutMybus::DeleteGroupItem(int groupIndex, Osp::Ui::Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

Osp::Ui::Controls::ListItemBase* FormAxBusAboutMybus::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	MybusGroup * pGroup = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(groupIndex));
	MybusGroupIndex * pIndex = static_cast<MybusGroupIndex*> (pGroup->__pArr->GetAt(itemIndex));
	BusSeoul_ArrivingByLine * pArrive = static_cast<BusSeoul_ArrivingByLine*> (__pArrAnswer->GetAt(pIndex->__indexArrivingArr.ToInt()));

#if 0
	String a;
	a.Format(100, L"%S", pArrive->__rtNm.GetPointer());
	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);
#else
	Bitmap * pbmpBus = GetBitmapBus(pArrive->__routeType);
	CustomItem * pItem = FactoryListItem::makeListItem_LineArriveInfo(itemIndex, itemWidth,  pArrive, pbmpBus);
	pItem->SetContextItem(__pContextMenu);
#endif
	return pItem;
}

bool FormAxBusAboutMybus::DeleteItem(int groupIndex, int itemIndex, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}


void FormAxBusAboutMybus::OnGroupedListViewContextItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("Grouped List element = %d and group/item = %d/%d", elementId, groupIndex, itemIndex);

	doContextMenu(elementId, groupIndex, itemIndex);
}

void FormAxBusAboutMybus::OnGroupedListViewItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListItemStatus state)
{
	switch (__nTapMybusGoto) {
	case 0: // 이동없음
		doTTS(groupIndex, itemIndex);
		break;
	case 1: // 정류소 도착버스로 이동
		doContextMenu(ID_CONTEXT_STOP_INFO, groupIndex, itemIndex);
		break;
	case 2: // 노선도 - 위치정보로 이동
		doContextMenu(ID_CONTEXT_LINESTOPS, groupIndex, itemIndex);
		break;
	default:
		break;
	}
}

void FormAxBusAboutMybus::OnGroupedListViewItemSwept(Osp::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusAboutMybus::OnGroupedListViewItemLongPressed(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
	doTTS(groupIndex, itemIndex);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusAboutMybus::AtTimer(HandleTimer * pTimerHandler)
{
	if (__pTimerRefresh == pTimerHandler) {
		__pList->UpdateList();
		this->RequestRedraw();
	}

	FormAxBus::AtTimer(pTimerHandler);
}

void FormAxBusAboutMybus::doContextMenu(int command, int groupIndex, int itemIndex)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	MybusGroup * pGroup = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(groupIndex));
	MybusGroupIndex * pIndex = static_cast<MybusGroupIndex*> (pGroup->__pArr->GetAt(itemIndex));
	BusSeoul_ArrivingByLine * pArrive = static_cast<BusSeoul_ArrivingByLine*> (__pArrAnswer->GetAt(pIndex->__indexArrivingArr.ToInt()));

	switch (command) {
	case ID_CONTEXT_MYBUS_DELETE:
		deleteMybus(pArrive);
		break;
	case ID_CONTEXT_LINESTOPS:
		SetKeyValue("노선정보_검색키", pArrive->__busRouteId);
		SetKeyValue("노선정보_번호이름", pArrive->__rtNm);
		SetKeyValue("노선정보_현재정류소", pArrive->__staOrd);
		GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	case ID_CONTEXT_STOP_INFO:
		SetKeyValue("정류소검색_키", pArrive->__arsId);
		GotoForm(FORM_ID_BUS_STOPLINES);
		break;
	default:
		break;
	}
}

void FormAxBusAboutMybus::doTTS(int groupIndex, int itemIndex)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	MybusGroup * pGroup = static_cast<MybusGroup*> (__pArrShowGroup->GetAt(groupIndex));
	MybusGroupIndex * pIndex = static_cast<MybusGroupIndex*> (pGroup->__pArr->GetAt(itemIndex));
	BusSeoul_ArrivingByLine * pArv = static_cast<BusSeoul_ArrivingByLine*> (__pArrAnswer->GetAt(pIndex->__indexArrivingArr.ToInt()));

	TimeSpan tmSpan1 = pArv->__datetime1.GetTime();
	TimeSpan tmSpanNow = UtilApp::GetNow().GetTime();
	tmSpan1 = tmSpan1 - tmSpanNow;
	int nTime1 = tmSpan1.GetTicks() / 1000;

	String ttsContents = "버스 ";
	ttsContents.Append(pArv->__rtNm);
	ttsContents.Append(" 번이 ");

	ttsContents.Append(pGroup->__stationNm);
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

void FormAxBusAboutMybus::deleteMybus(BusSeoul_ArrivingByLine * parrivs)
{
	DatabaseBus db;

	bool bOK = db.DeleteMyBus(parrivs->__stId, parrivs->__busRouteId);

	if (bOK) {
		UtilMessageBox::ConfirmWithCallback("", "내 버스 삭제 성공", this, ID_MESSAGEBOX_UPDATE);
	}
	else
		UtilMessageBox::ConfirmWithCallback("", "내 버스 삭제 실패", this, ID_MESSAGEBOX_CONFIRM);

}

