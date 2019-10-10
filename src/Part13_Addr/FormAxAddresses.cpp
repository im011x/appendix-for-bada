/*
 * FormAxAddresses.cpp
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#include "FormAxAddresses.h"
#include "FactoryListItem.h"
#include "DatabaseAddress.h"

FormAxAddresses::FormAxAddresses() : __pHttpAddress(null), __pHttpGoogleGeoCode(null), __pHttpDaumGeoCode(null), __pArrAddresses(null), __pListViewAddresses(null), __pContextMenu(null)
{
	__currOption = -1;
	__lastContextMenu = 0;
}

FormAxAddresses::~FormAxAddresses()
{
}

bool FormAxAddresses::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("주소검색 결과");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxAddresses::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxAddr::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	Panel * panel = makePanel(Rectangle(0, 0, dimBottomSize.width, 60), bottomPanel, "appendix/transparent.png", true);
	__pLabelSearchWhat = makeLabelText(Rectangle(10, 0, dimBottomSize.width-20, 60), "검색결과를 보여줍니다.", panel, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	__pLabelSearchWhat->SetWrap();
	y += 60;

	__pListViewAddresses = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);

	//
	init();
	invokeQuery();

	return r;
}
result FormAxAddresses::OnTerminating(void)
{
	if (__pHttpAddress) {
		delete __pHttpAddress;
	}

	if (__pHttpGoogleGeoCode) {
		delete __pHttpGoogleGeoCode;
	}

	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	if (__pArrAddresses) {
		__pArrAddresses->RemoveAll(true);
		delete __pArrAddresses;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return FormAxAddr::OnTerminating();
}

void FormAxAddresses::init()
{
	if (__pHttpAddress == null) {
		__pHttpAddress = new HttpAddress(this);
	}

	if (__pHttpGoogleGeoCode == null) {
		__pHttpGoogleGeoCode = new HttpGoogleGeo(this);
	}

	if (__pHttpDaumGeoCode == null) {
		__pHttpDaumGeoCode = new HttpDaum(this);
	}

	if (__pArrAddresses == null) {
		__pArrAddresses = new ArrayList();
		__pArrAddresses->Construct();
	}

	//
	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p0 = new BcContextItem("북 마크", ID_CONTEXT_BOOK_MARK);
	BcContextItem * p1 = new BcContextItem("지도보기", ID_CONTEXT_GET_POSITION);
	BcContextItem * p2 = new BcContextItem("주변정류소", ID_CONTEXT_GET_NEAR_STOP);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p0);
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);

	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;
	// 여기까지 context menu

}

void FormAxAddresses::invokeQuery()
{
	__currOption = GetValueByKey("주소검색_대상");
	String * pKey = GetStringByKey("주소검색_키");

	bool bOK = false;

	String what = *pKey;
	what.Append("검색 ");

	switch (__currOption) {
	case 0:  // 지번주소 - 동/읍/면,아파트/건물 이름
		what.Append(" (지번-동/읍/면,아파트/건물 이름)");
		bOK = __pHttpAddress->GetAddrByDongName(*pKey);
		//what.Append(" (지번-아파트/건물 이름)");
		//bOK = __pHttpAddress->GetAddrByBuildingName(*pKey);
		break;
	case 1:  // 지번주소 - 시/군/구 이름
		what.Append(" (지번-시/군/구 이름)");
		bOK = __pHttpAddress->GetAddrByCityName(*pKey);
		break;
	case 2:  // 지번주소 - 우편번호
		what.Append(" (지번-우편번호)");
		bOK = __pHttpAddress->GetAddrByZipCode(*pKey);
		break;
	case 3:  // 도로명주소 - 동/읍/면 이름
		what.Append(" (도로명-동/읍/면 이름)");
		bOK = __pHttpAddress->GetNewAddrByDongName(*pKey);
		break;
	case 4:  // 도로명주소 - 도로 이름
		what.Append(" (도로명-도로 이름)");
		bOK = __pHttpAddress->GetNewAddrByRoadName(*pKey);
		break;
	case 5:  // 도로명주소 - 우편번호
		what.Append(" (도로명-우편번호)");
		bOK = __pHttpAddress->GetNewAddrByZipCode(*pKey);
		break;
	default:
		AppLogDebug("check-up the passing parameters ...");
		return;
	}

	__pLabelSearchWhat->SetText(what);

	if (bOK)
		commShow();
}

void FormAxAddresses::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	default:
		FormAxAddr::OnActionPerformed(source, actionId);
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxAddresses::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	if (pHttp == __pHttpAddress->GetHandler()) {
		onHttpAddress(pBuffer);
	}
	else if (pHttp == __pHttpGoogleGeoCode->GetHandler()) {
		onHttpGoogleGeoCode(pBuffer);
	}
	else if (pHttp == __pHttpDaumGeoCode->GetHandler()) {
		onHttpDaumGeoCode(pBuffer);
	}
}

void FormAxAddresses::onHttpAddress(ByteBuffer* pBuffer)
{
	bool bOK = false;

	switch (__currOption) {
	case 0:  // 지번주소 - 동/읍/면 이름, 아파트/건물 이름
	case 1:  // 지번주소 - 우편번호
	case 2:  // 지번주소 - 시/군/구 이름
		bOK = __pHttpAddress->ParseAddr(pBuffer, __pArrAddresses);
		break;
	case 3:  // 도로명주소 - 동/읍/면 이름
	case 4:  // 도로명주소 - 도로 이름
	case 5:  // 도로명주소 - 우편번호
		bOK = __pHttpAddress->ParseNewAddr(pBuffer, __pArrAddresses);
		break;
	default:
		AppLogDebug("check-up the passing parameters ...");
		break;
	}

	commHide();

	if (bOK == false) {
		UtilMessageBox::ConfirmWithCallback("", __pHttpAddress->GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}
	else {
		String s = __pLabelSearchWhat->GetText();
		s.Append(" - 결과 ");
		s.Append(__pArrAddresses->GetCount());
		s.Append(" 건");
		__pLabelSearchWhat->SetText(s);
		__pLabelSearchWhat->RequestRedraw();

		//
		CompareZipAddress * pCompare = new CompareZipAddress();
		__pArrAddresses->Sort(*pCompare);
		delete pCompare;
		//
		__pListViewAddresses->UpdateList();
	}
}


void FormAxAddresses::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxAddresses::GetItemCount(void)
{
	if (__pArrAddresses == null)
		return 0;

	return __pArrAddresses->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxAddresses::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	//return FactoryListItem::makeListItem_Address(index, itemWidth,  static_cast<DataAddr*> (__pArrAddresses->GetAt(index)));
	CustomItem * pItem = FactoryListItem::makeListItem_Address(index, itemWidth,  static_cast<DataAddr*> (__pArrAddresses->GetAt(index)));
	pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool FormAxAddresses::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxAddresses::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

	switch (elementId) {
	case ID_CONTEXT_BOOK_MARK:
		bookMark(index);
		break;
	case ID_CONTEXT_GET_POSITION:
	case ID_CONTEXT_GET_NEAR_STOP:
		invokeGeoCoding(index, ID_CONTEXT_GET_POSITION);
		break;
	}

	__lastContextMenu = elementId;
}

void FormAxAddresses::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	invokeGeoCoding(index, ID_CONTEXT_GET_POSITION);

	__lastContextMenu = ID_CONTEXT_GET_POSITION;
}

void FormAxAddresses::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxAddresses::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	DataAddr * pItem = static_cast<DataAddr*> (__pArrAddresses->GetAt(index));

	String ttsContents = "지번 주소는 ";
	ttsContents.Append(pItem->__address);
	ttsContents.Append(" 입니다.");

	if (!pItem->__roadAddress.IsEmpty()) {
		ttsContents.Append(" 그리고 도로명 주소는 ");
		ttsContents.Append(pItem->__roadAddress);

	}

	ttsContents.Append(" 그리고 우편번호는  ");
	ttsContents.Append(pItem->__zipCode);
	ttsContents.Append(" 입니다.");

	TTS(ttsContents);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxAddresses::invokeGeoCoding(int nth, int whichMethod)
{
	DataAddr * pItem = static_cast<DataAddr*> (__pArrAddresses->GetAt(nth));

	switch (whichMethod) {
	case ID_CONTEXT_GET_POSITION:
		__pHttpGoogleGeoCode->GetPosition(pItem->__address); // 구글은 한개를 돌려 줌
		break;
	case ID_CONTEXT_GET_POSITION_DAUM:
		__pHttpDaumGeoCode->GetPosition(pItem->__address); // 다음은 복수개를 돌려 줌
		break;
	default:
		return;
	}

	commShow();
}

void FormAxAddresses::onHttpGoogleGeoCode(ByteBuffer* pBuffer)
{
	AppLogDebug("%s", pBuffer->GetPointer());

	commHide();
	__pHttpGoogleGeoCode->ParsePosition(pBuffer, null);

#ifdef _DEBUG_XXX
	String smessage = "구글 위치조회 결과\n \n";
	smessage.Append(__pHttpGoogleGeoCode->__formattedAddress);
	smessage.Append("\n");
	smessage.Append(__pHttpGoogleGeoCode->__lat);
	smessage.Append("\n");
	smessage.Append(__pHttpGoogleGeoCode->__lng);
	UtilMessageBox::ConfirmWithCallback("", smessage, this, ID_MESSAGEBOX_CONFIRM);
#endif

	if (__lastContextMenu == ID_CONTEXT_GET_NEAR_STOP) {
		SetKeyValue("정류소검색_위치_lat", __pHttpGoogleGeoCode->__lat);
		SetKeyValue("정류소검색_위치_lng", __pHttpGoogleGeoCode->__lng);

		GotoForm(FORM_ID_BUS_STOPNEAR);
	}
	else if (__lastContextMenu == ID_CONTEXT_GET_POSITION){
		SetKeyValue("위치_lat", __pHttpGoogleGeoCode->__lat);
		SetKeyValue("위치_lng", __pHttpGoogleGeoCode->__lng);
		SetKeyValue("위치_info", __pHttpGoogleGeoCode->__formattedAddress);
		SetKeyValue("위치_title", __pHttpGoogleGeoCode->__formattedAddress);

		GotoForm(FORM_ID_MAP_CENTER);
	}
}

void FormAxAddresses::onHttpDaumGeoCode(ByteBuffer* pBuffer)
{
	AppLogDebug("%s", pBuffer->GetPointer());

	commHide();
	__pHttpDaumGeoCode->ParsePosition(pBuffer, null);

#ifdef _DEBUG_XX
	String smessage = "다음 위치조회 결과\n \n";
	smessage.Append(__pHttpDaumGeoCode->__formattedAddress);
	smessage.Append("\n");
	smessage.Append(__pHttpDaumGeoCode->__lat);
	smessage.Append("\n");
	smessage.Append(__pHttpDaumGeoCode->__lng);
	UtilMessageBox::ConfirmWithCallback("", smessage, this, ID_MESSAGEBOX_CONFIRM);
#endif

	int countAnswer = 0;
	Integer::Parse(__pHttpDaumGeoCode->__totalCount, countAnswer);
	if (countAnswer > 0) {
		SetKeyValue("위치_lat", __pHttpDaumGeoCode->__lat);
		SetKeyValue("위치_lng", __pHttpDaumGeoCode->__lng);
		SetKeyValue("위치_info", __pHttpDaumGeoCode->__formattedAddress);
		SetKeyValue("위치_title", __pHttpDaumGeoCode->__formattedAddress);

		GotoForm(FORM_ID_MAP_CENTER);
	}
	else {
		UtilMessageBox::ConfirmWithCallback("", "위치조회 결과가 없습니다.\n(다음 로컬 API 사용)", this, ID_MESSAGEBOX_CONFIRM);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxAddresses::bookMark(int index)
{
	DataAddr * pItem = static_cast<DataAddr*> (__pArrAddresses->GetAt(index));

	DatabaseAddress 	db;
	bool bOK = db.Insert(pItem->__zipCode, pItem->__address, pItem->__roadAddress);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "북마크 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "북마크 저장 실패", this, ID_MESSAGEBOX_CONFIRM);
}

