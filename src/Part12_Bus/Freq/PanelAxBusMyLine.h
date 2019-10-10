/*
 * PanelAxBusMyLine.h
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#ifndef PANELAXBUSMYLINE_H_
#define PANELAXBUSMYLINE_H_

#include <FUi.h>
#include "ControlMaker.h"
#include "DatabaseBus.h"

class FormAxBus;

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;

class PanelAxBusMyLine
	: public Osp::Ui::Controls::Panel
	, public ControlMaker
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	PanelAxBusMyLine(FormAxBus * pHandler);
	virtual ~PanelAxBusMyLine();

// Implementation
protected:
	const 	static 	int 		ID_CONTEXT_LINEDEL = 121;
	const 	static 	int 		ID_CONTEXT_LINEINFO = 122;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 123;
	const 	static 	int 		ID_CONTEXT_LINEMAP = 124;

	FormAxBus 				* 		__pFormParent;
	DatabaseBus 			* 		__pDbBus;
	ArrayList 				* 		__pArr;
	ListView 				* 		__pList;

	Osp::Ui::IActionEventListener	* 	__pActionListener;
	ListContextItem 		*  			__pContextMenu;

protected:
	void 		init();
	void 		doContextMenu(int elementId, int index);
	void 		deleteFreqLine(BusSeoul_Line * parrivs);

public:
	bool 		Initialize(Rectangle rect, Container * pContainer, Osp::Ui::IActionEventListener* pActionListener);

public:
	//IListViewItemProvider
	virtual int GetItemCount(void);
	virtual Osp::Ui::Controls::ListItemBase* CreateItem(int index, int itemWidth);
	virtual bool DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth);

	// IListViewItemEventListener
	virtual void OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state);
	virtual void OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status);
	virtual void OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction);
	virtual void OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback);
};

#endif /* PANELAXBUSMYLINE_H_ */
