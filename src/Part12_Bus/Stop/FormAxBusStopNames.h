/*
 * FormAxBusStopNames.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPNAMES_H_
#define FORMAXBUSSTOPNAMES_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "DataBusSeoul.h"

class FormAxBusStopNames
	: public FormAxBus
	, public IHttpFirstResponder
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxBusStopNames();
	virtual ~FormAxBusStopNames();

	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const 	static 	int 		ID_CONTEXT_FAVORATE = 121;
	const 	static 	int 		ID_CONTEXT_MAP = 122;
	const 	static 	int 		ID_CONTEXT_STOP_ARRV = 123;

	ListView * 					__pListStops;
	ArrayList * 				__pArrBusStops;
	HttpBusSeoul * 				__pHttpSeoulBus;

	ListContextItem *  			__pContextMenu;

private:
	XmlBusSeoulParseStopName 	__parser;

protected:
	void	init();
	void 	invokeQuery();
	//void 	gotoBusStop(int index);
	void 	doContextMenu(int command, BusSeoul_StopInfo* pstop);
	void 	saveDb(BusSeoul_StopInfo* pstop);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

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

#endif /* FORMAXBUSSTOPNAMES_H_ */
