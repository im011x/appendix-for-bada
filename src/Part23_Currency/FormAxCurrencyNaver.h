/*
 * FormAxCurrencyNaver.h
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#ifndef FORMAXCURRENCYNAVER_H_
#define FORMAXCURRENCYNAVER_H_

#include "AppBaseForm.h"
#include "HttpCurrencyNaver.h"

class FormAxCurrencyNaver
	: public AppBaseForm
	, public IHttpFirstResponder
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxCurrencyNaver();
	virtual ~FormAxCurrencyNaver();

	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;
	const static 	int 		ID_UPDATE_CURRENCY = 201;

	MyLabel 		* 			__pLabelCurrencyDate;
	ListView 		* 			__pListView;
	ListContextItem *  			__pContextMenu;

	HttpCurrencyNaver 	* 		__pHttpCurrency;
	HttpCurrencyNaver 	* 		__pHttpCurrencyTime;
	ArrayList 			* 		__pArrCurrency;

protected:
	void 		init();
	void 		invokeQuery();

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

#endif /* FORMAXCURRENCYNAVER_H_ */
