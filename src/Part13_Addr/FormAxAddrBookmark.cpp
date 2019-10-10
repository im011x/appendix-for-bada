/*
 * FormAxAddrBookmark.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "FormAxAddrBookmark.h"
#include "FactoryListItem.h"

FormAxAddrBookmark::FormAxAddrBookmark() : __pDbAddress(null), __pArr(null), __pListAddress(null), __pTextBoxNoAddress(null)
{
}

FormAxAddrBookmark::~FormAxAddrBookmark()
{
}


bool FormAxAddrBookmark::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("주소 북마크");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxAddrBookmark::OnInitializing(void)
{
	result r = E_SUCCESS;

	// 일단 데이터를 구한다.
	bool bREADY = init();

	// TODO: Add your initialization code here
	FormAxAddr::OnInitializing();
	Panel * bottomPanel = GetPanel();

	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	// 광고 공간 확보!
	Panel * panel = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	createAds(Rectangle(0, y, dimBottomSize.width, 80), panel);
	y = 80;

	if (bREADY) {
		__pListAddress = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	}
	else {
		String smessage = "저장된 북마크가 없습니다.\n\n북 마크를 저장하시려면\n"
				"1. 주소검색 (지번,도로명)\n"
				"2. 검색결과에서 특정항목 저장\n"
				"  (특정항목을 옆으로 살짝 밀어 \n"
				"   나타나는 [북마크] 버튼을 \n"
				"   통해 저장할 수 있습니다.";

		y += 20;
		__pTextBoxNoAddress = makeTextBox(Rectangle(20, y, dimBottomSize.width-40, dimBottomSize.height-y), smessage, bottomPanel, Color::COLOR_BLACK);
	}

/*
	if (__pAd) {
		RequestId reqId  = INVALID_REQUEST_ID;
		__pAd->RequestAd(reqId);
	}
*/

	return r;
}

bool FormAxAddrBookmark::init()
{
	if (__pArr == null) {
		__pArr = new ArrayList();
		__pArr->Construct();
	}

	__pDbAddress = new DatabaseAddress();
	__pDbAddress->GetFreqN(__pArr);

	return (__pArr->GetCount() > 0);
}

void FormAxAddrBookmark::invokeQuery()
{
}

result FormAxAddrBookmark::OnTerminating(void)
{
	if (__pDbAddress) {
		delete __pDbAddress;
	}

	if (__pArr) {
		__pArr->RemoveAll(true);
		delete __pArr;
	}

	return FormAxAddr::OnTerminating();
}

void FormAxAddrBookmark::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
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

int FormAxAddrBookmark::GetItemCount(void)
{
	if (__pArr == null)
		return 0;

	return __pArr->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxAddrBookmark::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_AddressFreq(index, itemWidth,  static_cast<AddressItem*> (__pArr->GetAt(index)));
	//pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool FormAxAddrBookmark::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxAddrBookmark::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

}

void FormAxAddrBookmark::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

/*
	String * pCategory = static_cast<String*> (__pArr->GetAt(index));
	SetKeyValue("공공전화_카테고리", *pCategory);

	GotoForm(FORM_ID_PHONES2);
*/
}

void FormAxAddrBookmark::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxAddrBookmark::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AddressItem * pItem = static_cast<AddressItem*> (__pArr->GetAt(index));
	//DataAddr * pItem = static_cast<DataAddr*> (__pArrAddresses->GetAt(index));

	String ttsContents = "지번 주소는 ";
	ttsContents.Append(pItem->__address);
	ttsContents.Append(" 입니다.");

	if (!pItem->__addressNew.IsEmpty()) {
		ttsContents.Append(" 그리고 도로명 주소는 ");
		ttsContents.Append(pItem->__addressNew);

	}

	ttsContents.Append(" 그리고 우편번호는  ");
	ttsContents.Append(pItem->__zip);
	ttsContents.Append(" 입니다.");

	TTS(ttsContents);

}
