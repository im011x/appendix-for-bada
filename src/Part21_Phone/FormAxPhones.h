/*
 * FormAxPhones.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXPHONES_H_
#define FORMAXPHONES_H_

#include "AppBaseForm.h"
#include "DatabasePhonebook.h"


class FormAxPhones
	: public AppBaseForm
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxPhones();
	virtual ~FormAxPhones();
	virtual bool AtInitialize();

// Implementation
protected:
	DatabasePhonebook 		* 		__pDbPhones;
	ArrayList 				* 		__pArr;
	ListView 				* 		__pListPhones;

protected:
	void 		init();

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

#endif /* FORMAXPHONES_H_ */
