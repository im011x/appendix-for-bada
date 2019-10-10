/*
 * PanelAxBusMyLine.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "PanelAxBusMyLine.h"
#include "FactoryListItem.h"
//#include "FormAxBus.h"
#include "FormAxBusAboutFreq.h"

using namespace Osp::Graphics;

PanelAxBusMyLine::PanelAxBusMyLine(FormAxBus * pHandler) : __pFormParent(pHandler), __pDbBus(null), __pArr(null), __pList(null), __pActionListener(null), __pContextMenu(null)
{
}

PanelAxBusMyLine::~PanelAxBusMyLine()
{
	if (__pArr) {
		__pArr->RemoveAll(true);
		delete __pArr;
		__pArr = null;
	}

	if (__pDbBus) {
		delete __pDbBus;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}
}

void PanelAxBusMyLine::init()
{
	if (__pArr == null) {
		__pArr = new ArrayList();
		__pArr->Construct();
	}

	__pDbBus = new DatabaseBus();
	__pDbBus->GetFreqLineN(__pArr);
}

bool PanelAxBusMyLine::Initialize(Rectangle rect, Container * pContainer, Osp::Ui::IActionEventListener* pActionListener)
{
	this->Construct(rect);
	pContainer->AddControl(*this);
	__pActionListener = pActionListener;

	init();

	Dimension sz = this->GetSize();
	__pList = makeListView(Rectangle(0, 0, sz.width, sz.height), this, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	__pList->SetTextOfEmptyList("등록된 자주쓰는 노선정보가 없습니다.");

	{
	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("삭제", ID_CONTEXT_LINEDEL);
	BcContextItem * p2 = new BcContextItem("노선정보", ID_CONTEXT_LINEINFO);
	BcContextItem * p3 = new BcContextItem("노선도", ID_CONTEXT_LINESTOPS);
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

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int PanelAxBusMyLine::GetItemCount(void)
{
	if (__pArr == null)
		return 0;

	return __pArr->GetCount();
}

Osp::Ui::Controls::ListItemBase* PanelAxBusMyLine::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_BusLine(index, itemWidth,  static_cast<BusSeoul_Line*> (__pArr->GetAt(index)), true);
	pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool PanelAxBusMyLine::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void PanelAxBusMyLine::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

	doContextMenu(elementId, index);
}

void PanelAxBusMyLine::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	doContextMenu(ID_CONTEXT_LINESTOPS, index);
}

void PanelAxBusMyLine::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void PanelAxBusMyLine::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
}

void PanelAxBusMyLine::doContextMenu(int elementId, int index)
{
	BusSeoul_Line * pline = static_cast<BusSeoul_Line*> (__pArr->GetAt(index));

	switch (elementId) {
	case ID_CONTEXT_LINEDEL:
		deleteFreqLine(pline);
		break;
	case ID_CONTEXT_LINEINFO:
		break;
	case ID_CONTEXT_LINESTOPS:
		__pFormParent->SetKeyValue("노선정보_검색키", pline->__busRouteId);
		__pFormParent->SetKeyValue("노선정보_번호이름", pline->__busRouteNm);
		__pFormParent->GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	case ID_CONTEXT_LINEMAP:
		break;
	}

	//__pActionListener->OnActionPerformed(*this, elementId);
}

void PanelAxBusMyLine::deleteFreqLine(BusSeoul_Line * parrivs)
{
	DatabaseBus db;

	bool bOK = db.DeleteLine(parrivs->__busRouteId);

	if (bOK) {
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 노선 삭제 성공", __pActionListener, FormAxBusAboutFreq::ID_MESSAGEBOX_UPDATE);
	}
	else
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 노선 삭제 실패", __pActionListener, FormAxBusAboutFreq::ID_MESSAGEBOX_CONFIRM);

}
