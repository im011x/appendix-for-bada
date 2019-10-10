/*
 * FormAxCurrencyDetail.cpp
 *
 *  Created on: 2012. 8. 15.
 *      Author: benny
 */

#include "FormAxCurrencyDetail.h"
#include "DataCurrency.h"
#include "FactoryListItem.h"

FormAxCurrencyDetail::FormAxCurrencyDetail() : __pList(null)
{
}

FormAxCurrencyDetail::~FormAxCurrencyDetail()
{
}

bool FormAxCurrencyDetail::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("환율 상세");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxCurrencyDetail::OnInitializing(void)
{
	result r = E_SUCCESS;

	DataCurrency* pCurrCurrency = static_cast<DataCurrency*> (GetObjectByKey("환율상세_오브젝트"));

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	// 일단 각 영역을 판넬로 할당한다.
	Panel * pPanelTop = makePanel(Rectangle(0, 0, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	makeLabelText(Rectangle( 10,  0, 400, 80), pCurrCurrency->__currencyName, pPanelTop, 40, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	//makeLabelText(Rectangle( 10, 80, 400, 50), "환율계산에 사용할 기준 상태를 선택하세요", pPanelTop, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	//y += (130);
	y += (80);

	/*
	int x = 23;
	y += (90+30);
	Panel * pPanelL = makePanel(Rectangle(x, y, 434/2, 350), bottomPanel, "bg/bg_434x190_blue.png", true);
	Panel * pPanelR = makePanel(Rectangle(x+434/2, y, 434/2, 350), bottomPanel, "bg/bg_434x190_white.png", true);
	y += (350 + 30);


	// 각 판넬을 채운다
	int w = 434/2 - 30;
	makeLabelText(Rectangle(20,   0, w, 50), "매매기준율",  pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20,  50, w, 50), "현찰 (살때)", pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20, 100, w, 50), "현찰 (팔때)", pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20, 150, w, 50), "송금 (보낼때)", pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20, 200, w, 50), "송금 (받을때)", pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20, 250, w, 50), "환가료율",     pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));
	makeLabelText(Rectangle(20, 300, w, 50), "미화환산율",    pPanelL, 25, Color::COLOR_WHITE, Color(0,0,0,0));

	w = 434/2 - 30;
	makeLabelText(Rectangle(10,   0, w, 50), pCurrCurrency->__currencyRate[0] + " 원", pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10,  50, w, 50), pCurrCurrency->__currencyRate[1] + " 원", pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 100, w, 50), pCurrCurrency->__currencyRate[2] + " 원", pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 150, w, 50), pCurrCurrency->__currencyRate[3] + " 원", pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 200, w, 50), pCurrCurrency->__currencyRate[4] + " 원", pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 250, w, 50), pCurrCurrency->__currencyRate[5], pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 300, w, 50), pCurrCurrency->__currencyRate[6], pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	 */

	//__pList = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, false);
	__pList = makeListView(Rectangle(0, y, dimBottomSize.width, 420), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, false);
	__pList->SetTextOfEmptyList("");


	return r;
}

result FormAxCurrencyDetail::OnTerminating(void)
{

	return AppBaseForm::OnTerminating();
}

void FormAxCurrencyDetail::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
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

int FormAxCurrencyDetail::GetItemCount(void)
{
	return 7;
}

Osp::Ui::Controls::ListItemBase* FormAxCurrencyDetail::CreateItem(int index, int itemWidth)
{
	String sCurrencyKinds[] = {
			"매매기준율",
			"현찰 (살때)",
			"현찰 (팔때)",
			"송금 (보낼때)",
			"송금 (받을때)",
			"환가료율",
			"미화환산율"
	};

	/*
	switch (index) {
	default:
		break;
	}
	 */

	DataCurrency* pCurrCurrency = static_cast<DataCurrency*> (GetObjectByKey("환율상세_오브젝트"));

	String sValue = pCurrCurrency->__currencyRate[index];
	if (index < 5) {
		sValue += " 원";
	}

	CustomItem * pItem = FactoryListItem::makeListItem_String2(index, itemWidth, sCurrencyKinds[index],
			sValue,
			LIST_ANNEX_STYLE_NORMAL);
			//LIST_ANNEX_STYLE_MARK);

	if (index == 0) {
		__pList->SetItemChecked(index, true);
	}

	return pItem;
}

bool FormAxCurrencyDetail::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxCurrencyDetail::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);
}

void FormAxCurrencyDetail::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	for (int i=0; i<__pList->GetItemCount(); i++) {
		__pList->SetItemChecked(i, false);
	}

	__pList->SetItemChecked(index, true);
}

void FormAxCurrencyDetail::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxCurrencyDetail::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
}


