/*
 * FormAxBusAboutLine.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSABOUTLINE_H_
#define FORMAXBUSABOUTLINE_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include "GroupBusLine.h"
#include "DataBusSeoul.h"

class FormAxBusAboutLine
		: public FormAxBus
		, public IHttpFirstResponder
		, public Osp::Ui::ITextEventListener
		, public Osp::Ui::Controls::IGroupedListViewItemProvider
		, public Osp::Ui::Controls::IGroupedListViewItemEventListener
		, public Osp::Ui::Controls::IFastScrollListener
		, public Osp::Ui::Controls::IListViewItemProvider
		, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxBusAboutLine();
	virtual ~FormAxBusAboutLine();
	virtual bool AtInitialize();
	virtual bool AtWakeup();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const 	static 	int 		ID_SEARCH_CLEAR = 110;
	const 	static 	int 		ID_SEARCH_DO = 111;
	const 	static 	int 		ID_SEARCH_CANCEL = 112;

	const 	static 	int 		ID_CONTEXT_FREQ = 121;
	const 	static 	int 		ID_CONTEXT_LINEINFO = 122;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 123;
	const 	static 	int 		ID_CONTEXT_LINEMAP = 124;

	Osp::Ui::Controls::EditField * 		__pSearchEdit;
	GroupedListView* 					__pAllContentsListView;
	ListView * 							__pListViewLines;
	HttpBusSeoul * 						__pHttpSeoulBus;

	static ArrayList * 					__pArrBusLinesTotal;
	ArrayList * 						__pArrActive;

	XmlBusSeoulParseLines	 			__parser;

	HashMap *							__pTypedGroupMap;
	ArrayList * 						__pTypedGroupArr;

	ListContextItem *  					__pContextMenu;

protected:
	void 			init();
	void 			invokeQuery();
	void 			makeGroup();
	void 			showSearching();

	GroupBusLine * 	GetBusLineGroup(const String & key);

	void 			doContextMenu(int command, BusSeoul_Line* pline);
	void			saveDb(BusSeoul_Line* pline);
	void 			doTTS(int groupIndex, int itemIndex);
	void 			doTTS(int index);

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

	//ITextEventListener
	virtual void OnTextValueChanged(const Osp::Ui::Control& source);
	virtual void OnTextValueChangeCanceled(const Osp::Ui::Control& source);

	// FastScroll
	virtual void OnFastScrollIndexSelected(Osp::Ui::Control& source, Osp::Base::String& index);
};

#endif /* FORMAXBUSABOUTLINE_H_ */
