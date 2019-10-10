/*
 * FormAxPhones2.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "FormAxPhones2.h"
#include "FactoryListItem.h"

FormAxPhones2::FormAxPhones2()  : __pDbPhones(null), __pArr(null), __pListPhones(null)
{
}

FormAxPhones2::~FormAxPhones2()
{
}

bool FormAxPhones2::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	String * pCat = GetStringByKey("공공전화_카테고리");
	if (pCat) {
		SetTitle(*pCat);
	}

	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxPhones2::OnInitializing(void)
{
	result r = E_SUCCESS;

	// 일단 데이터를 구한다.
	__pDbPhones = new DatabasePhonebook();
	__pArr = __pDbPhones->GetPhones(*GetStringByKey("공공전화_카테고리"));

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();
	Panel * bottomPanel = GetPanel();

	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	// 광고 공간 확보!
	Panel * panel = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	createAds(Rectangle(0, y, dimBottomSize.width, 80), panel);
	y = 80;

	__pListPhones = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);

	//
/*
	if (__pAd) {
		RequestId reqId  = INVALID_REQUEST_ID;
		__pAd->RequestAd(reqId);
	}
*/

	return r;
}

void FormAxPhones2::init()
{
}

void FormAxPhones2::invokeQuery()
{
}

result FormAxPhones2::OnTerminating(void)
{
	if (__pDbPhones) {
		delete __pDbPhones;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxPhones2::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxPhones2::GetItemCount(void)
{
	if (__pArr == null)
		return 0;

	return __pArr->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxPhones2::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_PublicPhones(index, itemWidth,  static_cast<PhoneItem*> (__pArr->GetAt(index)));
	//pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool FormAxPhones2::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxPhones2::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

}

void FormAxPhones2::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	PhoneItem * pcurr = static_cast<PhoneItem *> (__pArr->GetAt(index));
	SetKeyObject("공공전화_상세", pcurr);

	GotoForm(FORM_ID_PHONE_DETAIL);
}

void FormAxPhones2::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxPhones2::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	PhoneItem * pcurr = static_cast<PhoneItem *> (__pArr->GetAt(index));

	String phoneNum = pcurr->__num_area;
	if (!phoneNum.IsEmpty()) {
		phoneNum.Append("-");
	}
	phoneNum.Append(pcurr->__num_station);
	if (!phoneNum.IsEmpty()) {
		phoneNum.Append("-");
	}
	phoneNum.Append(pcurr->__num_last);


	String ttsContents = "주요전화, ";
	ttsContents.Append(pcurr->__owner_name);
	ttsContents.Append(" 의 전화 번호는 ");
	ttsContents.Append(phoneNum);
	ttsContents.Append(" 입니다.");

	TTS(ttsContents);
}
