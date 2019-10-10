/*
 * FormAxCurrencyDetail.h
 *
 *  Created on: 2012. 8. 15.
 *      Author: benny
 */

#ifndef FORMAXCURRENCYDETAIL_H_
#define FORMAXCURRENCYDETAIL_H_

#include "AppBaseForm.h"

class FormAxCurrencyDetail
	: public AppBaseForm
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxCurrencyDetail();
	virtual ~FormAxCurrencyDetail();
	virtual bool AtInitialize();

// Implementation
protected:
	ListView 	* 				__pList;

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

#endif /* FORMAXCURRENCYDETAIL_H_ */
