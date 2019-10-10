/*
 * PanelAxBusMyStop.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "PanelAxBusMyStop.h"
#include "FactoryListItem.h"
//#include "FormAxBus.h"
#include "FormAxBusAboutFreq.h"

using namespace Osp::Graphics;

PanelAxBusMyStop::PanelAxBusMyStop(FormAxBus * pHandler) : __pFormParent(pHandler), __pDbBus(null), __pArr(null), __pList(null), __pActionListener(null), __pContextMenu(null)
{
}

PanelAxBusMyStop::~PanelAxBusMyStop()
{
	if (__pArr) {
		__pArr->RemoveAll(true);
		delete __pArr;
		__pArr = null;
	}

	if (__pDbBus) {
		delete __pDbBus;
	}
}

void PanelAxBusMyStop::init()
{
	if (__pArr == null) {
		__pArr = new ArrayList();
		__pArr->Construct();
	}

	__pDbBus = new DatabaseBus();
	__pDbBus->GetFreqStopN(__pArr);
}

bool PanelAxBusMyStop::Initialize(Rectangle rect, Container * pContainer, Osp::Ui::IActionEventListener* pActionListener)
{
	this->Construct(rect);
	pContainer->AddControl(*this);
	__pActionListener = pActionListener;

	init();

	Dimension sz = this->GetSize();
	__pList = makeListView(Rectangle(0, 0, sz.width, sz.height), this, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	__pList->SetTextOfEmptyList("등록된 자주쓰는 정류소정보가 없습니다.");


	{
	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("삭제", ID_CONTEXT_STOP_DEL);
	BcContextItem * p2 = new BcContextItem("도착정보", ID_CONTEXT_STOP_ARRV);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);

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

int PanelAxBusMyStop::GetItemCount(void)
{
	if (__pArr == null)
		return 0;

	return __pArr->GetCount();
}

Osp::Ui::Controls::ListItemBase* PanelAxBusMyStop::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	CustomItem * pItem = FactoryListItem::makeListItem_StopInfo(index, itemWidth,  static_cast<BusSeoul_StopInfo*> (__pArr->GetAt(index)));
	pItem->SetContextItem(__pContextMenu);
	return pItem;
#endif
}

bool PanelAxBusMyStop::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void PanelAxBusMyStop::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);
	doContextMenu(elementId, index);
}

void PanelAxBusMyStop::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);
	doContextMenu(ID_CONTEXT_STOP_ARRV, index);
}

void PanelAxBusMyStop::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void PanelAxBusMyStop::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");
}

void PanelAxBusMyStop::doContextMenu(int elementId, int index)
{
	BusSeoul_StopInfo * pinfo = static_cast<BusSeoul_StopInfo*> (__pArr->GetAt(index));

	switch (elementId) {
	case ID_CONTEXT_STOP_DEL:
		deleteFreqStop(pinfo);
		break;
	case ID_CONTEXT_STOP_ARRV:
		__pFormParent->SetKeyValue("정류소검색_키", pinfo->__arsId);
		__pFormParent->GotoForm(FORM_ID_BUS_STOPLINES);
		break;
	default:
		break;
	}
}

void PanelAxBusMyStop::deleteFreqStop(BusSeoul_StopInfo * pStop)
{
	DatabaseBus db;

	bool bOK = db.DeleteStop(pStop->__arsId);

	if (bOK) {
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 삭제 성공", __pActionListener, FormAxBusAboutFreq::ID_MESSAGEBOX_UPDATE);
	}
	else
		UtilMessageBox::ConfirmWithCallback("", "자주쓰는 정류소 삭제 실패", __pActionListener, FormAxBusAboutFreq::ID_MESSAGEBOX_CONFIRM);
}
