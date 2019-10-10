/*
 * FormAxBusStopInfo.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusStopInfo.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"

FormAxBusStopInfo::FormAxBusStopInfo() : __pListLines(null), __pArrBusLines(null), __pHttpSeoulBus(null), __pHttpSeoulBusGetTime(null),
	__pMapGroups(null), __pArrGroups(null), __pContextMenu(null)
{
}

FormAxBusStopInfo::~FormAxBusStopInfo()
{
}

bool FormAxBusStopInfo::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("정류소 정보");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusStopInfo::OnInitializing(void)
{
	result r = E_SUCCESS;

	//
	String stopCode;
	String * pKey = GetStringByKey("정류소검색_키");
	if (pKey) {
		stopCode = *pKey;
		if (stopCode.GetLength()>2) {
			stopCode.Insert('-', 2);
			stopCode.Insert('[', 0);
			stopCode.Append(']');
		}
	}
	AppLogDebug("정류소 키값 = %S", stopCode.GetPointer());
	//

	String stopName;
	String * pName = GetStringByKey("정류소검색_이름");
	if (pName) {
		stopName = *pName;
	}

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int w = dimBottomSize.width - 20;
	int y = 0;

	Panel * pPanelBG = makePanel(Rectangle(0,  y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png");
	/*
	Panel * pPanelBG = makePanel(Rectangle(0,  y, dimBottomSize.width, 180-40), bottomPanel, "appendix/bg_white.png");
	y = 10;

	makeButton(Rectangle(w/3*0+10,  y, w/3-20, 50), "지도보기", pPanelBG, ID_COMMAND_STOP_MAP, this);
	makeButton(Rectangle(w/3*1+10,  y, w/3-20, 50), "자주쓰는", pPanelBG, ID_COMMAND_STOP_FREQ, this);
	makeButton(Rectangle(w/3*2+10,  y, w/3-20, 50), "도착버스", pPanelBG, ID_COMMAND_STOP_BUSES, this);
	y += 50;
	 */

	makeLabelText(Rectangle(10, y, w-10, 50), stopName, pPanelBG, 40, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 50;

	makeLabelText(Rectangle(10, y, w/2, 30), stopCode, pPanelBG, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 30;

	//makeLabelText(Rectangle(10, y, w, 40), "현재 정류소의 첫차/막차 정보를 확인하려면 해당 노선을 선택하세요.", pPanelBG, 20, Color(64,64,64), Color(0,0,0,0), ALIGNMENT_LEFT);
	//y += 40;

	// create list view
	__pListLines = makeGroupedListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), this, this, this, true);

	init();
	invokeQuery();

	return r;
}

void FormAxBusStopInfo::init()
{
	if (__pArrBusLines == null) {
		__pArrBusLines = new ArrayList();
		__pArrBusLines->Construct();
	}

	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}

	if (__pHttpSeoulBusGetTime == null) {
		__pHttpSeoulBusGetTime = new HttpBusSeoul(this);
	}

	if (__pMapGroups == null) {
		__pMapGroups = new HashMap();
		__pMapGroups->Construct();
	}

	if (__pArrGroups == null) {
		__pArrGroups = new ArrayList();
		__pArrGroups->Construct();
	}

	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("내 버스",   ID_CONTEXT_MYBUS);
	BcContextItem * p2 = new BcContextItem("노선정보", ID_CONTEXT_LINEINFO);
	BcContextItem * p3 = new BcContextItem("노선도", ID_CONTEXT_LINEINFO);
	BcContextItem * p4 = new BcContextItem("노선지도", ID_CONTEXT_LINEMAP);

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

void FormAxBusStopInfo::invokeQuery()
{
	String * pKey = GetStringByKey("정류소검색_키");
	if (pKey == null) {
		UtilMessageBox::ConfirmWithCallback("", "검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}

	if (__pHttpSeoulBus->GetLanesByStop(*pKey))
		commShow();
}

void FormAxBusStopInfo::askTimeTable(const String & lineID)
{
	String * pKey = GetStringByKey("정류소검색_키");

	if (__pHttpSeoulBusGetTime->GetTimesBy(*pKey, lineID))
		commShow();
}

result FormAxBusStopInfo::OnTerminating(void)
{
	if (__pHttpSeoulBus) {
		delete __pHttpSeoulBus;
	}

	if (__pHttpSeoulBusGetTime) {
		delete __pHttpSeoulBusGetTime;
	}

	if (__pArrBusLines) {
		__pArrBusLines->RemoveAll(true);
		delete __pArrBusLines;
	}

	if (__pMapGroups) {
		__pMapGroups->RemoveAll(false);
		delete __pMapGroups;
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

void FormAxBusStopInfo::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	BusSeoul_ArrivingInfo * pOneArrvInfo = null;

	ArrayList * pArrStopArriving = static_cast<ArrayList*> (GetObjectByKey("정류소정보_Array"));
	if (pArrStopArriving) {
		pOneArrvInfo = static_cast<BusSeoul_ArrivingInfo*> (pArrStopArriving->GetAt(0));
	}

	switch(actionId)
	{
	case ID_COMMAND_STOP_MAP:
		AppLogDebug("정류소 위치를 지도로 확인합니다.");
		break;
	case ID_COMMAND_STOP_FREQ:
		AppLogDebug("자주쓰는 정류소로 등록합니다.");
		saveFreqStop(pOneArrvInfo);
		break;
	case ID_COMMAND_STOP_BUSES:
		AppLogDebug("정류소 도착예정 버스를 확인합니다.");
		GotoBack();
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

void FormAxBusStopInfo::makeGroups()
{
	if (__pArrBusLines == null) {
		AppAssert(0);
		return;
	}

	for (int i=0; i<__pArrBusLines->GetCount(); i++) {
		BusSeoul_LineDetail* pArrv = static_cast<BusSeoul_LineDetail*> (__pArrBusLines->GetAt(i));
		GroupBusLine * pGroup = buildGroup(pArrv->__routeType);
		pGroup->pArr->Add(*pArrv);
	}

}

GroupBusLine * FormAxBusStopInfo::buildGroup(const String & key)
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

void FormAxBusStopInfo::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	if (__pHttpSeoulBus->GetHandler() == pHttp) {
		bool bOK = __parser.ParseXml(pBuffer, __pArrBusLines);
		commHide();

		if (__pArrBusLines->GetCount() <= 0) {
			bOK = false;
		}

		if (bOK) {
			CompareBusSeoul_Line * pCompare = new CompareBusSeoul_Line();
			__pArrBusLines->Sort(*pCompare);
			delete pCompare;

			makeGroups();

			__pListLines->UpdateList();
			__pListLines->ScrollToItem(0, -1);  // 왜 뜨자마자 마지막으로 가지 ?
		}
		else {
			UtilMessageBox::ConfirmWithCallback("", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
		}
	}
	else if (pHttp == __pHttpSeoulBusGetTime->GetHandler()) {
		__parserTime.ParseXml(pBuffer, __pArrBusLines);
		commHide();
		__pListLines->UpdateList();
	}

	this->RequestRedraw();
}

void FormAxBusStopInfo::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusStopInfo::GetGroupCount(void)
{
	if (__pArrBusLines == null)
		return 0;

	if (__pArrGroups == null)
		return 0;

	return __pArrGroups->GetCount();
}

int FormAxBusStopInfo::GetItemCount(int groupIndex)
{
	if (__pArrBusLines == null)
		return 0;

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	return pGroup->pArr->GetCount();

}

Osp::Ui::Controls::GroupItem* FormAxBusStopInfo::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new GroupItem();
	pItem->Construct(Dimension(itemWidth, 50));

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));

	String nameGroup;
	nameGroup.Format(255, L"%S (%d 대)", pGroup->sName.GetPointer(), pGroup->pArr->GetCount());
	pItem->SetElement(nameGroup, null);

	return pItem;
}

bool FormAxBusStopInfo::DeleteGroupItem(int groupIndex, Osp::Ui::Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

Osp::Ui::Controls::ListItemBase* FormAxBusStopInfo::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_LineDetail* p = static_cast<BusSeoul_LineDetail*> (pGroup->pArr->GetAt(itemIndex));

	CustomItem * pItem = FactoryListItem::makeListItem_StopLinesInfo(itemIndex, itemWidth,  p);
	pItem->SetContextItem(__pContextMenu);
	return pItem;
}

bool FormAxBusStopInfo::DeleteItem(int groupIndex, int itemIndex, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

void FormAxBusStopInfo::OnGroupedListViewContextItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("Grouped List element = %d and group/item = %d/%d", elementId, groupIndex, itemIndex);

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_LineDetail* pSelectedLine = static_cast<BusSeoul_LineDetail*> (pGroup->pArr->GetAt(itemIndex));

	doContextMenu(elementId, pSelectedLine);
}

void FormAxBusStopInfo::OnGroupedListViewItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListItemStatus state)
{
	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_LineDetail* pSelectedLine = static_cast<BusSeoul_LineDetail*> (pGroup->pArr->GetAt(itemIndex));
	//doContextMenu(ID_CONTEXT_LINESTOPS, pSelectedLine);
	askTimeTable(pSelectedLine->__busRouteId);
}

void FormAxBusStopInfo::OnGroupedListViewItemSwept(Osp::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusStopInfo::OnGroupedListViewItemLongPressed(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
	doTTS(groupIndex, itemIndex);
}

void FormAxBusStopInfo::doTTS(int groupIndex, int itemIndex)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	bool bOK = false;

	String * pName = GetStringByKey("정류소검색_이름");
	String ttsContents = *pName;
	ttsContents.Append(" 정류소 ");

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pArrGroups->GetAt(groupIndex));
	BusSeoul_LineDetail* pinfo = static_cast<BusSeoul_LineDetail*> (pGroup->pArr->GetAt(itemIndex));

	ttsContents.Append(pinfo->__busRouteNm);
	ttsContents.Append(" 번 경우, ");

	if (!pinfo->__firstBusTmCurrTTS.IsEmpty()) {
		ttsContents.Append(pinfo->__firstBusTmCurrTTS);
		bOK = true;
	}
	if (!pinfo->__lastBusTmCurrTTS.IsEmpty()) {
		ttsContents.Append(pinfo->__lastBusTmCurrTTS);
		bOK = true;
	}

	if (!bOK) {
		ttsContents.Append("첫차, 막차 정보를 확인하려면 이곳을 누르세요");
	}

	TTS(ttsContents);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusStopInfo::doContextMenu(int command, BusSeoul_LineDetail* pline)
{


	switch (command) {
	case ID_CONTEXT_MYBUS:
		saveDb(pline);
		break;
	case ID_CONTEXT_LINEINFO:
		break;
	case ID_CONTEXT_LINESTOPS:
		SetKeyValue("노선정보_검색키", pline->__busRouteId);
		SetKeyValue("노선정보_번호이름", pline->__busRouteNm);
		{
			ArrayList * pArrStopArriving = static_cast<ArrayList*> (GetObjectByKey("정류소정보_Array"));
			if (pArrStopArriving) {
				BusSeoul_ArrivingInfo * pOneArrvInfo = static_cast<BusSeoul_ArrivingInfo*> (pArrStopArriving->GetAt(0));
				SetKeyValue("노선정보_현재정류소", pOneArrvInfo->__staOrd);
			}
		}
		GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	case ID_CONTEXT_LINEMAP:
		break;
	}
}

void FormAxBusStopInfo::saveDb(BusSeoul_LineDetail* pline)
{
	BusSeoul_ArrivingInfo* pTheArriv = null;
	ArrayList * pArrStopArriving = static_cast<ArrayList*> (GetObjectByKey("정류소정보_Array"));

	for (int i=0; i<pArrStopArriving->GetCount(); i++) {
		BusSeoul_ArrivingInfo* pArriv = static_cast <BusSeoul_ArrivingInfo *> (pArrStopArriving->GetAt(i));

		if (pArriv->__busRouteId == pline->__busRouteId) {
			pTheArriv = pArriv;
			break;
		}
	}

	if (pTheArriv == null) {
		return;
	}

	DatabaseBus db;

	bool bOK = db.InsertMyBus(pTheArriv->__stId, pTheArriv->__staOrd, pTheArriv->__stNm, pTheArriv->__arsId, pTheArriv->__busRouteId, pTheArriv->__rtNm);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "내 버스 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}

void FormAxBusStopInfo::saveFreqStop(BusSeoul_ArrivingInfo * pArrv)
{
	if (pArrv == null) {
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소정보 파라미터가 없음", this, ID_MESSAGEBOX_CONFIRM);
		return;
	}

	DatabaseBus db;
	bool bOK = db.InsertStop(pArrv->__arsId, pArrv->__stId, pArrv->__stNm, pArrv->__stationTp);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소정보 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소정보 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}

