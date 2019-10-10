/*
 * FormAxBusStopLines.h
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPSEARCHLIST_H_
#define FORMAXBUSSTOPSEARCHLIST_H_

#include "FormAxBus.h"
#include "HandleThread.h"
#include "HandleBusStopLines.h"
#include "ThreadBusStopLines.h"
#include "UtilRegistry.h"

class FormAxBusStopLines
	: public FormAxBus
	, public HandleThread // 정류소 도착 노선 정보를 주기적으로 얻기 위해 사용
	, public UtilRegistry
	, public Osp::Ui::Controls::IGroupedListViewItemProvider
	, public Osp::Ui::Controls::IGroupedListViewItemEventListener
{
public:
	FormAxBusStopLines();
	virtual ~FormAxBusStopLines();
	virtual bool AtInitialize();
	virtual void AtTimer(HandleTimer * pTimerHandler);

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const static 	int 		ID_UPDATE_STOP_LINES = 110;
	const static 	int 		ID_COMMAND_STOP_TIMETABLE = 111;
	//const static 	int 		ID_COMMAND_ADD_FREQ_STOP = 112;
	//const static 	int 		ID_COMMAND_MAP_THE_STOP = 113;

	const 	static 	int 		ID_CONTEXT_MYBUS = 121;
	//const 	static 	int 		ID_CONTEXT_LINEINFO = 122;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 123;
	//const 	static 	int 		ID_CONTEXT_LINEMAP = 124;
	const 	static 	int 		ID_CONTEXT_FREQ_STOP = 125;
	const 	static 	int 		ID_CONTEXT_FREQ_LINE = 126;

	GroupedListView* 			__pListLines;
	ArrayList * 				__pArrGroups;
	ArrayList * 				__pArrBusLines;

	HandleBusStopLines 	* 		__pBusStopLinesInvoker;
	ListContextItem *  			__pContextMenu;
	String 						__keyParam;
	MyLabel * 					__pLabelStopName;
	ThreadBusStopLines 	* 		__pUpdateThread;

	int 						__nTapStoplineGoto;

protected:
	void 	init();
	void 	invokeQuery();

	void 	updateStopName();
	void 	createUpdateTherad();
	void 	destroyUpdateTherad();

	void 	doContextMenu(int command, BusSeoul_ArrivingInfo* pline);
	void 	saveMybus(BusSeoul_ArrivingInfo* pstop);
	void 	saveFreqStop(BusSeoul_ArrivingInfo* pstop);
	void 	saveFreqLine(BusSeoul_ArrivingInfo* pstop);
	void 	doTTS(int groupIndex, int itemIndex);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	virtual bool AtSleep();
	virtual bool AtWakeup();

	// IHttpFirstResponder
	//virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	//virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);
	//
	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);

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

#endif /* FORMAXBUSSTOPSEARCHLIST_H_ */
