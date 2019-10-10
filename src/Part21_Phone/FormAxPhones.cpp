/*
 * FormAxPhones.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxPhones.h"
#include "FactoryListItem.h"

FormAxPhones::FormAxPhones() : __pDbPhones(null), __pArr(null), __pListPhones(null)
{
}

FormAxPhones::~FormAxPhones()
{
}

bool FormAxPhones::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("주요 전화번호");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxPhones::OnInitializing(void)
{
	result r = E_SUCCESS;

	// 일단 데이터를 구한다.
	init();

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

	return r;
}

void FormAxPhones::init()
{
	__pDbPhones = new DatabasePhonebook();
	__pArr = __pDbPhones->GetCategories();
}

result FormAxPhones::OnTerminating(void)
{
	if (__pDbPhones) {
		delete __pDbPhones;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxPhones::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
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

int FormAxPhones::GetItemCount(void)
{
	if (__pArr == null)
		return 0;

	return __pArr->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxPhones::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_PublicPhoneCategory(index, itemWidth,  static_cast<String*> (__pArr->GetAt(index)));
	//pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool FormAxPhones::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxPhones::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

}

void FormAxPhones::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	String * pCategory = static_cast<String*> (__pArr->GetAt(index));
	SetKeyValue("공공전화_카테고리", *pCategory);

	GotoForm(FORM_ID_PHONES2);
}

void FormAxPhones::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxPhones::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	String * pCategory = static_cast<String*> (__pArr->GetAt(index));

	String ttsContents = "주요전화 카테고리는 ";
	ttsContents.Append(*pCategory);
	ttsContents.Append(" 입니다.");

	TTS(ttsContents);
}
