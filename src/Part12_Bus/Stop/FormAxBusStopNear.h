/*
 * FormAxBusStopNear.h
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPNEAR_H_
#define FORMAXBUSSTOPNEAR_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DataBusSeoul.h"

class FormAxBusStopNear
	: public FormAxBus
	, public IHttpFirstResponder
	, public Osp::Ui::IAdjustmentEventListener
	, public Osp::Ui::Controls::ISliderEventListener
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxBusStopNear();
	virtual ~FormAxBusStopNear();
	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const static 	int 		ID_UPDATE_NEAR_STOP = 110;
	const static 	int 		ID_COMMAND_STOP_DETAIL = 111;

	const 	static 	int 		ID_CONTEXT_FAVORATE = 121;
	const 	static 	int 		ID_CONTEXT_MAP = 122;
	const 	static 	int 		ID_CONTEXT_STOP_ARRV = 123;

	MyLabel * 					__pLabelRadius;
	MyLabel * 					__pLabelTotal;
	Slider * 					__pSliderDistance;
	ListView * 					__pListStops;
	ArrayList * 				__pArrStops;
	HttpBusSeoul * 				__pHttpSeoulBus;

	XmlBusSeoulParseNearStops 	__parser;

	String 						__currLat;
	String 						__currLng;
	String 						__sDistanceNow;

	ListContextItem *  			__pContextMenu;

protected:
	void 	init();
	void 	invokeQuery(bool update=false);
	void 	computeDistance();
	//void	gotoBusStop(int index);
	void 	doContextMenu(int command, BusSeoul_StopInfo* pstop);
	void 	saveDb(BusSeoul_StopInfo* pstop);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

	// IAdjustmentEventListener
	virtual void OnAdjustmentValueChanged(const Osp::Ui::Control& source, int adjustment);
	// ISliderEventListener
	virtual void OnSliderBarMoved(Osp::Ui::Controls::Slider& source, int value);

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

#endif /* FORMAXBUSSTOPNEAR_H_ */
