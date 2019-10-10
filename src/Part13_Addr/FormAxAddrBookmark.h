/*
 * FormAxAddrBookmark.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef FORMAXADDRBOOKMARK_H_
#define FORMAXADDRBOOKMARK_H_

#include "FormAxAddr.h"
#include "DatabaseAddress.h"

class FormAxAddrBookmark
	: public FormAxAddr
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxAddrBookmark();
	virtual ~FormAxAddrBookmark();
	virtual bool AtInitialize();

// Implementation
protected:
	DatabaseAddress 		* 		__pDbAddress;
	ArrayList 				* 		__pArr;

	ListView 				* 		__pListAddress;
	TextBox 				* 		__pTextBoxNoAddress;

protected:
	bool 		init();
	void 		invokeQuery();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

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

#endif /* FORMAXADDRBOOKMARK_H_ */
