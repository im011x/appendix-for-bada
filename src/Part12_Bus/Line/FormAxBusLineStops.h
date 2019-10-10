/*
 * FormAxBusLineStops.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSLINESTOPS_H_
#define FORMAXBUSLINESTOPS_H_

#include "FormAxBus.h"
#include "HandleThread.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DataBusSeoul.h"
#include "ThreadBusPositions.h"

class FormAxBusLineStops
	: public FormAxBus
	, public HandleThread // 버스위치 정보를 주기적으로 얻기 위해 사용
	, public IHttpFirstResponder
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxBusLineStops();
	virtual ~FormAxBusLineStops();

	virtual bool AtInitialize();

// Implementation
protected:
	const 	static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const 	static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const 	static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const 	static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;
	const 	static 	int 		ID_MESSAGEBOX_NO_STOP = 104;

	const 	static 	int 		ID_UPDATE_LINE_STOPS = 110;

	const 	static 	int 		ID_CONTEXT_MYBUS = 121;
	//const 	static 	int 		ID_CONTEXT_STOP_MAP = 122;
	//const 	static 	int 		ID_CONTEXT_STOP_INFO = 123;
	const 	static 	int 		ID_CONTEXT_FREQ_STOP = 124;
	const 	static 	int 		ID_CONTEXT_FREQ_LINE = 125;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 126;
	const 	static 	int 		ID_CONTEXT_MAP = 127;

	HttpBusSeoul * 						__httpSeoulBus;
	ArrayList * 						__pArrStops;
	ArrayList * 						__pArrPositions;
	ListView * 							__pListViewStops;
	XmlBusSeoulParseLineStops 			__parser;

	ListContextItem *  					__pContextMenu;

	ThreadBusPositions 	* 				__pUpdateThread;
	String 								__keyParam;

	void 		init();
	void 		invokeQuery();

	void 		createUpdateTherad();
	void 		destroyUpdateTherad();

	void 		doContextMenu(int command, BusSeoul_StopByLine* pStop);
	void 		saveDb(BusSeoul_StopByLine* pstop);
	void 		saveFreqStop(BusSeoul_StopByLine* pstop);
	void 		saveFreqLine(BusSeoul_StopByLine* pstop);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	virtual bool AtSleep();
	virtual bool AtWakeup();
	virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

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

#endif /* FORMAXBUSLINESTOPS_H_ */
