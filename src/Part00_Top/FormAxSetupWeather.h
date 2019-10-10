/*
 * FormAxSetupWeather.h
 *
 *  Created on: 2012. 9. 27.
 *      Author: benny
 */

#ifndef FORMAXSETUPWEATHER_H_
#define FORMAXSETUPWEATHER_H_

#include "FormAxSetup.h"
#include "UtilRegistry.h"

class CompareString :  public Osp::Base::Collection::IComparer
{
public:
	CompareString() {};
	virtual ~CompareString() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};


class FormAxSetupWeather
	: public FormAxSetup
	, public UtilRegistry
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxSetupWeather();
	virtual ~FormAxSetupWeather();
	virtual bool AtInitialize();

// Implementation
protected:
	ListView 	* 				__pList;

protected:
	void 		init();
	void 		save();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);

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

#endif /* FORMAXSETUPWEATHER_H_ */
