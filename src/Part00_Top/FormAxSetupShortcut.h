/*
 * FormAxSetupShortcut.h
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#ifndef FORMAXSETUPSHORTCUT_H_
#define FORMAXSETUPSHORTCUT_H_

#include "FormAxSetup.h"
#include "UtilRegistry.h"

class FormAxSetupShortcut
	: public FormAxSetup
	, public UtilRegistry
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxSetupShortcut();
	virtual ~FormAxSetupShortcut();
	virtual bool AtInitialize();

// Implementation
protected:
	const 	static 	int 		ID_SHORTCUT_DO = 201;
	const 	static 	int 		ID_SHORTCUT_DONOT = 202;

	ListView 	* 				__pList;
	MyButton 	* 				__pOptionsBtn[2];

private:
	int 						__shortcutOnStartup;
	int 						__shortcutFormID;

protected:
	void 		init();
	void 		save();

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

#endif /* FORMAXSETUPSHORTCUT_H_ */
