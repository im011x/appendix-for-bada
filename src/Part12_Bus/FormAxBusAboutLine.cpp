/*
 * FormAxBusAboutLine.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusAboutLine.h"
#include "FactoryListItem.h"
#include "DatabaseBus.h"

ArrayList * 		FormAxBusAboutLine::__pArrBusLinesTotal = null;

FormAxBusAboutLine::FormAxBusAboutLine() :
		__pSearchEdit(null), __pAllContentsListView(null), __pListViewLines(null),
		__pHttpSeoulBus(null), __pArrActive(null), __pTypedGroupMap(null), __pTypedGroupArr(null), __pContextMenu(null)
{
}

FormAxBusAboutLine::~FormAxBusAboutLine()
{
}

bool FormAxBusAboutLine::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );
	SetTitle("노선 검색");
	SetPanelStype(BOTTOM_PANEL);
	return true;
}

result FormAxBusAboutLine::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int 	nScreenWidth  = dimBottomSize.width;
	//int 	nScreenHeight = dimBottomSize.height;  // offset
	int x = 10;
	int y = 10;

	__pSearchEdit = makeEditField(Rectangle(x, y, nScreenWidth-120, 60),  EDIT_FIELD_STYLE_NUMBER, "버스노선 번호를 입력하세요.", bottomPanel,
			this, 7, ID_SEARCH_CANCEL, ID_SEARCH_DO, "취소", "확인");  // EDIT_FIELD_STYLE_NORMAL
	//__pSearchEdit->SetCurrentInputModeCategory(EDIT_INPUTMODE_NUMERIC);
	__pSearchEdit->AddTextEventListener(*this);

	MyButton * pbtn =  makeButton(Rectangle(nScreenWidth-100, y, 90, 60),  "appendix/bt_eraser.png", "appendix/bt_eraser_p.png", "", bottomPanel, ID_SEARCH_CLEAR, this);
	pbtn->SetSpeechText("검색어를 지웁니다.");
	y += 70;

	/*
	static int nMeetCount = 0;
	if (nMeetCount == 0) {
		makeLabelText(Rectangle(10, y, nScreenWidth-20, 30), "특정항목을 옆으로 살짝 밀어보세요 ...",
				bottomPanel, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_LEFT);
		y += 40;
	}
	nMeetCount++;
	 */

	// 나중에 돋보기 이미지를 배경에 깔고, 그 만큼 왼쪽 마진을 주는 작업을 하도록 하자~
	//Bitmap * pbmp = loadBitmapN("test/will_mag.png");
	//__pSearchEdit->SetBackgroundBitmap(EDIT_STATUS_NORMAL, *pbmp);
	//__pSearchEdit->SetMargin(EDIT_MARGIN_TYPE_HORIZONTAL, 50);

	////
	// 그룹 리스트 - 결과를 그룹핑하여 보여준다.
	__pAllContentsListView = makeGroupedListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), this, this, this, true);
	__pAllContentsListView->AddFastScrollListener(*this);
	__pAllContentsListView->SetFastScrollIndex("공간지순광인경", false);
	//__pAllContentsListView->SetFastScrollIndex("공항/간선/지선/순환/광역/인천/경기", false);

	//
	__pListViewLines = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	__pListViewLines->SetShowState(false);

	init();  //

	invokeQuery();  //

	InitIntent();  // 2nd level 메뉴에서는 화면 생성 단계에서 폼 파라미터 초기화 한다.

	return r;
}

result FormAxBusAboutLine::OnTerminating(void)
{
	//AppLogDebug("deleting __pArrActive");
	if (__pArrActive != null) {
		__pArrActive->RemoveAll(false);
		delete __pArrActive;
	}

	//AppLogDebug("deleting __pTypedGroupMap");
	if (__pTypedGroupMap != null) {
		__pTypedGroupMap->RemoveAll(false);
		delete __pTypedGroupMap;
	}

	//AppLogDebug("deleting __pTypedGroupArr");
	if (__pTypedGroupArr != null) {
		__pTypedGroupArr->RemoveAll(false);
		delete __pTypedGroupArr;
	}

	//AppLogDebug("deleting __httpSeoulBus");
	if (__pHttpSeoulBus) {
		delete __pHttpSeoulBus;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusAboutLine::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		//GotoBack();
		break;
	case ID_SEARCH_DO:
		__pSearchEdit->HideKeypad();
		break;
	case ID_SEARCH_CANCEL:
	case ID_SEARCH_CLEAR:
		__pSearchEdit->HideKeypad();
		__pSearchEdit->SetText("");
		__pSearchEdit->RequestRedraw();

		showSearching();
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

bool FormAxBusAboutLine::AtWakeup()
{
	AppLog("AtWakeup()");
	InitIntent();
	return true;
}

void FormAxBusAboutLine::init()
{
	// 일단 초기화 ...
	if (__pArrActive == null) {
		__pArrActive = new ArrayList();
		__pArrActive->Construct();
	}

	if (__pTypedGroupMap == null) {
		__pTypedGroupMap = new HashMap();
		__pTypedGroupMap->Construct();
	}

	if (__pTypedGroupArr == null) {
		__pTypedGroupArr = new ArrayList();
		__pTypedGroupArr->Construct();
	}

	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("자주쓰는", ID_CONTEXT_FREQ);
	BcContextItem * p2 = new BcContextItem("노선정보", ID_CONTEXT_LINEINFO);
	BcContextItem * p3 = new BcContextItem("노선도", ID_CONTEXT_LINESTOPS);
	//BcContextItem * p4 = new BcContextItem("노선지도", ID_CONTEXT_LINEMAP);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);
	pArrMenu->Add(*p3);
	//pArrMenu->Add(*p4);

	//
	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;
}

void FormAxBusAboutLine::invokeQuery()
{
	// 쿼리 할 필요가 있나?
	if (__pArrBusLinesTotal != null && __pArrBusLinesTotal->GetCount() > 0) {
		AppLogDebug("전체 노선 정보 이미 있습니다... 갯수는  = %d", __pArrBusLinesTotal->GetCount());
		makeGroup();
		return;
	}
	else {
		if (__pArrBusLinesTotal == null) {
			__pArrBusLinesTotal = new ArrayList();
			__pArrBusLinesTotal->Construct();
		}
	}

	AppLogDebug("서비스 중인 전체 노선 정보를 가져옵니다...");

	if (__pHttpSeoulBus == null) {
		__pHttpSeoulBus = new HttpBusSeoul(this);
	}

	if (__pHttpSeoulBus->GetLines())
		commShow();
}

GroupBusLine * FormAxBusAboutLine::GetBusLineGroup(const String & key)
{
	//AppLogDebug("@@@ addr of __pTypedGroupMap = 0x%x and searching key = %S", __pTypedGroupMap, key.GetPointer());

	bool bIsIn = false;
	__pTypedGroupMap->ContainsKey(key, bIsIn);

	if (bIsIn) {  // exist already!
		return static_cast<GroupBusLine*> (__pTypedGroupMap->GetValue(key));
	}

	// make an instance of the key and make new one instance
	String * pKey = new String(key);
	GroupBusLine * pOneGroup = new GroupBusLine();
	pOneGroup->SetName(key);

	__pTypedGroupMap->Add(*pKey, *pOneGroup);
	__pTypedGroupArr->Add(*pOneGroup);

	return pOneGroup;
}

void FormAxBusAboutLine::makeGroup()
{
	if (__pArrBusLinesTotal == null) {
		AppLogDebug("전체 노선 정보 없습니다.");
		return;
	}

	for (int i=0; i<__pArrBusLinesTotal->GetCount(); i++) {
		BusSeoul_Line* pLine = static_cast<BusSeoul_Line*> (__pArrBusLinesTotal->GetAt(i));
		GroupBusLine * pGroup = GetBusLineGroup(pLine->__routeType);
		pGroup->pArr->Add(*pLine);
	}
}

void FormAxBusAboutLine::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	bool bOK = __parser.ParseXml(pBuffer, __pArrBusLinesTotal);

	commHide();

	if (bOK) {
		CompareBusSeoul_Line * pCompare = new CompareBusSeoul_Line();
		__pArrBusLinesTotal->Sort(*pCompare);
		delete pCompare;

		makeGroup();

		__pAllContentsListView->UpdateList();
	}
	else {
		UtilMessageBox::ConfirmWithCallback("", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}

	this->RequestRedraw();
}

void FormAxBusAboutLine::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	__pAllContentsListView->SetTextOfEmptyList("조회 실패, 재시도 바랍니다.");
	__pAllContentsListView->UpdateList();

	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
	this->RequestRedraw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusAboutLine::GetItemCount(void)
{
	if (__pArrActive == null)
		return 0;

	return __pArrActive->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxBusAboutLine::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	if (__pArrActive) {
		//return FactoryListItem::makeListItem_BusLine(index, itemWidth,  static_cast<BusSeoul_Line*> (__pArrActive->GetAt(index)), true);
		CustomItem * pItem = FactoryListItem::makeListItem_BusLine(index, itemWidth,  static_cast<BusSeoul_Line*> (__pArrActive->GetAt(index)), true);
		pItem->SetContextItem(__pContextMenu);
		return pItem;
	}

	return null;
#endif
}

bool FormAxBusAboutLine::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	CustomItem * pTheItem = (CustomItem*) pItem;
	pTheItem->RemoveAllElements();

	delete pItem;
	pItem = null;
	return true;
}

void FormAxBusAboutLine::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("filtered List element = %d and item = %d", elementId, index);

	BusSeoul_Line* pSelectedLine = static_cast<BusSeoul_Line*> (__pArrActive->GetAt(index));
	doContextMenu(elementId, pSelectedLine);
}

void FormAxBusAboutLine::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	// 키보드가 열린 상태일수 있으므로 일단 닫는다 ...
	__pSearchEdit->HideKeypad();

	BusSeoul_Line* pSelectedLine = static_cast<BusSeoul_Line*> (__pArrActive->GetAt(index));

	doContextMenu(ID_CONTEXT_LINESTOPS, pSelectedLine);
}

void FormAxBusAboutLine::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusAboutLine::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
	doTTS(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxBusAboutLine::GetGroupCount(void)
{
	if (__pArrBusLinesTotal == null)
		return 0;

	if (__pTypedGroupArr == null)
		return 0;

	return __pTypedGroupArr->GetCount();
}

int FormAxBusAboutLine::GetItemCount(int groupIndex)
{
	if (__pArrBusLinesTotal == null)
		return 0;

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));
	//AppLogDebug("group[%d]'s number of items = %d", groupIndex, pGroup->pArr->GetCount());
	return pGroup->pArr->GetCount();

}

Osp::Ui::Controls::GroupItem* FormAxBusAboutLine::CreateGroupItem(int groupIndex, int itemWidth)
{
	GroupItem* pItem = new GroupItem();
	pItem->Construct(Dimension(itemWidth, 50));

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));

	String nameGroup;
	nameGroup.Format(255, L"%S (%d 대)", pGroup->sName.GetPointer(), pGroup->pArr->GetCount());
	pItem->SetElement(nameGroup, null);

	return pItem;
}

bool FormAxBusAboutLine::DeleteGroupItem(int groupIndex, Osp::Ui::Controls::GroupItem* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

Osp::Ui::Controls::ListItemBase* FormAxBusAboutLine::CreateItem(int groupIndex, int itemIndex, int itemWidth)
{
	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));
	BusSeoul_Line* p = static_cast<BusSeoul_Line*> (pGroup->pArr->GetAt(itemIndex));

	CustomItem * pItem = FactoryListItem::makeListItem_BusLine(itemIndex, itemWidth,  p, false);
	pItem->SetContextItem(__pContextMenu);

	return pItem;
}

bool FormAxBusAboutLine::DeleteItem(int groupIndex, int itemIndex, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}


void FormAxBusAboutLine::OnGroupedListViewContextItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("Grouped List element = %d and group/item = %d/%d", elementId, groupIndex, itemIndex);

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));
	BusSeoul_Line* pSelectedLine = static_cast<BusSeoul_Line*> (pGroup->pArr->GetAt(itemIndex));

	doContextMenu(elementId, pSelectedLine);
}

void FormAxBusAboutLine::OnGroupedListViewItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListItemStatus state)
{
	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));
	BusSeoul_Line* pSelectedLine = static_cast<BusSeoul_Line*> (pGroup->pArr->GetAt(itemIndex));
	doContextMenu(ID_CONTEXT_LINESTOPS, pSelectedLine);
}

void FormAxBusAboutLine::OnGroupedListViewItemSwept(Osp::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxBusAboutLine::OnGroupedListViewItemLongPressed(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
	doTTS(groupIndex, itemIndex);
}


void FormAxBusAboutLine::OnFastScrollIndexSelected(Osp::Ui::Control& source, Osp::Base::String& index)
{
	AppLogDebug("fast scroll index = %S", index.GetPointer());

	if (index.CompareTo("공") == 0) {
		__pAllContentsListView->ScrollToItem(0, -1);
	}
	else if (index.CompareTo("간") == 0) {
		__pAllContentsListView->ScrollToItem(1, -1);
	}
	else if (index.CompareTo("지") == 0) {
		__pAllContentsListView->ScrollToItem(2, -1);
	}
	else if (index.CompareTo("순") == 0) {
		__pAllContentsListView->ScrollToItem(3, -1);
	}
	else if (index.CompareTo("광") == 0) {
		__pAllContentsListView->ScrollToItem(4, -1);
	}
	else if (index.CompareTo("인") == 0) {
		__pAllContentsListView->ScrollToItem(5, -1);
	}
	else if (index.CompareTo("경") == 0) {
		__pAllContentsListView->ScrollToItem(6, -1);
	}

	__pAllContentsListView->RequestRedraw();
}

void FormAxBusAboutLine::OnTextValueChanged(const Osp::Ui::Control& source)
{
	showSearching();
}

void FormAxBusAboutLine::OnTextValueChangeCanceled(const Osp::Ui::Control& source)
{
	AppLogDebug("");
}

void FormAxBusAboutLine::showSearching()
{
	String inKey = __pSearchEdit->GetText();

	AppLogDebug("searching key = %S", inKey.GetPointer());

	if (inKey.IsEmpty()) {  // 원래 것을 보여준다...
		__pArrActive->RemoveAll(false);

		__pListViewLines->SetShowState(false);
		__pAllContentsListView->SetShowState(true);
		__pAllContentsListView->RequestRedraw();
	}
	else {
		__pArrActive->RemoveAll(false);

		for (int i=0; i<__pArrBusLinesTotal->GetCount(); i++) {
			BusSeoul_Line* pLine = static_cast<BusSeoul_Line*> (__pArrBusLinesTotal->GetAt(i));
			if (pLine->__busRouteNm.StartsWith(inKey, 0)) {
				__pArrActive->Add(*pLine);
			}
		}

		__pAllContentsListView->SetShowState(false);
		__pListViewLines->SetShowState(true);
		__pListViewLines->UpdateList();
		__pListViewLines->RequestRedraw();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusAboutLine::doContextMenu(int command, BusSeoul_Line* pline)
{
	if (pline) {
		SetKeyValue("노선정보_검색키", pline->__busRouteId);
		SetKeyValue("노선정보_번호이름", pline->__busRouteNm);
	}

	switch (command) {
	case ID_CONTEXT_FREQ: // 자주쓰는
		AppLogDebug("자주쓰는 항목으로 등록합니다.");
		saveDb(pline);
		break;
	case ID_CONTEXT_LINEINFO: // 노선정보
		GotoForm(FORM_ID_BUS_LINEINFO);
		break;
	case ID_CONTEXT_LINESTOPS: // 노선도
		GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	case ID_CONTEXT_LINEMAP: // 노선지도
		GotoForm(FORM_ID_BUS_LINEMAP);
		break;
	default:
		AppAssert(0);
		break;
	}

}

void FormAxBusAboutLine::saveDb(BusSeoul_Line* pline)
{
	DatabaseBus db;

	bool bOK = db.InsertLine(pline->__busRouteId, pline->__busRouteNm, pline->__routeType, pline->__term, pline->__stStationNm, pline->__edStationNm);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}

void FormAxBusAboutLine::doTTS(int groupIndex, int itemIndex)
{
	if (groupIndex < 0 || itemIndex < 0) {
		AppLogDebug("그룹 항목이 눌린듯 합니다.");
		return;
	}

	GroupBusLine * pGroup = static_cast<GroupBusLine*> (__pTypedGroupArr->GetAt(groupIndex));
	BusSeoul_Line* pline = static_cast<BusSeoul_Line*> (pGroup->pArr->GetAt(itemIndex));

	String ttsContents = pGroup->sName;
	ttsContents.Append(", ");
	ttsContents.Append(pline->__busRouteNm);
	ttsContents.Append(" 번 정보 입니다.");

	TTS(ttsContents);
}

void FormAxBusAboutLine::doTTS(int index)
{
	BusSeoul_Line* pline = static_cast<BusSeoul_Line*> (__pArrActive->GetAt(index));

	String ttsContents;
	ttsContents.Append(pline->__busRouteNm);
	ttsContents.Append(" 번 정보 입니다.");

	TTS(ttsContents);
}
