/*
 * FormAxBusAboutMybus.h
 *
 *  Created on: 2012. 8. 9.
 *      Author: benny
 */

#ifndef FORMAXBUSABOUTMYBUS_H_
#define FORMAXBUSABOUTMYBUS_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DatabaseBus.h"
#include "HandleThread.h"
#include "ThreadMybus.h"
#include "UtilRegistry.h"

class FormAxBusAboutMybus
	: public FormAxBus
	, public HandleThread // 정류소 도착 노선 정보를 주기적으로 얻기 위해 사용
	, public UtilRegistry
	, public Osp::Ui::Controls::IGroupedListViewItemProvider
	, public Osp::Ui::Controls::IGroupedListViewItemEventListener
{
public:
	FormAxBusAboutMybus();
	virtual ~FormAxBusAboutMybus();
	virtual bool AtInitialize();
	virtual void AtTimer(HandleTimer * pTimerHandler);
	virtual bool AtSleep();
	virtual bool AtWakeup();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;
	const static 	int 		ID_MESSAGEBOX_UPDATE = 104;
	const static 	int 		ID_UPDATE_MYBUS = 111;

	const static 	int 		ID_CONTEXT_MYBUS_DELETE = 201;
	const static 	int 		ID_CONTEXT_LINESTOPS = 202;
	const static 	int 		ID_CONTEXT_STOP_INFO = 203;


	DatabaseBus 			* 		__pDbBus;
	GroupedListView 		* 		__pList;
	ArrayList 				* 		__pArrCandidates;
	ArrayList 				* 		__pArrAnswer;
	ArrayList 				* 		__pArrShowGroup;
	ThreadMybus 			* 		__pThreadMybus;
	ListContextItem 		*  		__pContextMenu;

	int 							__nTapMybusGoto;
	HandleTimer 			* 		__pTimerRefresh;

protected:
	void 		init();
	void 		invokeQuery();

	void 		createMybusTherad();
	void 		destroyMybusTherad();

	MybusGroup * 	getGroup(BusSeoul_ArrivingByLine * pArriv);
	bool 			addGroup(BusSeoul_ArrivingByLine * pArriv, int position);
	void 			OnAnswer();

	void 			doContextMenu(int command, int idxG, int idx);
	void 			deleteMybus(BusSeoul_ArrivingByLine * pArrive);
	void 			doTTS(int idxG, int idx);


public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);

public:
	// IGroupedListViewItemProvider
	virtual int GetGroupCount(void);
	virtual int GetItemCount(int groupIndex);
	virtual Osp::Ui::Controls::GroupItem* CreateGroupItem(int groupIndex, int itemWidth);
	virtual bool DeleteGroupItem(int groupIndex, Osp::Ui::Controls::GroupItem* pItem, int itemWidth);
	virtual Osp::Ui::Controls::ListItemBase* CreateItem(int groupIndex, int itemIndex, int itemWidth);
	virtual bool DeleteItem(int groupIndex, int itemIndex, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth);

	// IGroupedListViewItemEventListener
	virtual void OnGroupedListViewContextItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListContextItemStatus state);
	virtual void OnGroupedListViewItemStateChanged(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, Osp::Ui::Controls::ListItemStatus state);
	virtual void OnGroupedListViewItemSwept(Osp::Ui::Controls::GroupedListView &listView, int groupIndex,  int itemIndex, Osp::Ui::Controls::SweepDirection direction);
	virtual void OnGroupedListViewItemLongPressed(Osp::Ui::Controls::GroupedListView &listView, int groupIndex, int itemIndex, int elementId, bool& invokeListViewItemCallback);

};

#endif /* FORMAXBUSABOUTMYBUS_H_ */
