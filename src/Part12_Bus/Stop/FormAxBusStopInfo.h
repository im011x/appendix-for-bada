/*
 * FormAxBusStopInfo.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPINFO_H_
#define FORMAXBUSSTOPINFO_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "GroupBusLine.h"
#include "DataBusSeoul.h"

class FormAxBusStopInfo
	: public FormAxBus
	, public IHttpFirstResponder
	, public Osp::Ui::Controls::IGroupedListViewItemProvider
	, public Osp::Ui::Controls::IGroupedListViewItemEventListener
{
public:
	FormAxBusStopInfo();
	virtual ~FormAxBusStopInfo();

	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const static 	int 		ID_COMMAND_STOP_MAP = 111;
	const static 	int 		ID_COMMAND_STOP_FREQ = 112;
	const static 	int 		ID_COMMAND_STOP_BUSES = 113;

	const 	static 	int 		ID_CONTEXT_MYBUS = 121;
	const 	static 	int 		ID_CONTEXT_LINEINFO = 122;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 123;
	const 	static 	int 		ID_CONTEXT_LINEMAP = 124;

	GroupedListView* 			__pListLines;
	ArrayList * 				__pArrBusLines;

	HttpBusSeoul * 						__pHttpSeoulBus;
	XmlBusSeoulParseLinesByStop 		__parser;
	HttpBusSeoul * 						__pHttpSeoulBusGetTime;
	XmlBusSeoulParseTimeByLineByStop  	__parserTime;

	//
	HashMap *					__pMapGroups;
	ArrayList * 				__pArrGroups;
	ListContextItem *  			__pContextMenu;

protected:
	void 	init();
	void 	invokeQuery();
	void 	askTimeTable(const String & lineID);

	void 	makeGroups();
	GroupBusLine * buildGroup(const String & key);

	void 	doContextMenu(int command, BusSeoul_LineDetail* pline);
	void 	saveDb(BusSeoul_LineDetail* pstop);
	void 	saveFreqStop(BusSeoul_ArrivingInfo * pArrv);
	void 	doTTS(int groupIndex, int itemIndex);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

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

#endif /* FORMAXBUSSTOPINFO_H_ */
