/*
 * FormAxAddresses.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#ifndef FORMAXADDRESSES_H_
#define FORMAXADDRESSES_H_

#include "FormAxAddr.h"
#include "HttpAddress.h"
#include "HttpGoogleGeo.h"
#include "HttpDaum.h"

class FormAxAddresses
	: public FormAxAddr
	, public IHttpFirstResponder
	, public Osp::Ui::Controls::IListViewItemProvider
	, public Osp::Ui::Controls::IListViewItemEventListener
{
public:
	FormAxAddresses();
	virtual ~FormAxAddresses();
	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const static 	int 		ID_CONTEXT_BOOK_MARK = 201;
	const static 	int 		ID_CONTEXT_GET_POSITION_DAUM = 202;
	const static 	int 		ID_CONTEXT_GET_POSITION = 203;
	const static 	int 		ID_CONTEXT_GET_NEAR_STOP = 204;

	HttpAddress 	* 			__pHttpAddress;
	HttpGoogleGeo 	* 			__pHttpGoogleGeoCode;
	HttpDaum 		* 			__pHttpDaumGeoCode;

	int 						__currOption;
	ArrayList 		* 			__pArrAddresses;
	ListView * 					__pListViewAddresses;

	MyLabel 		*  			__pLabelSearchWhat;

	ListContextItem *  			__pContextMenu;

private:
	int 						__lastContextMenu;

protected:
	void 		init();
	void 		invokeQuery();
	void 		invokeGeoCoding(int nth, int whichMethod);

	void 		bookMark(int index);

	void 		onHttpAddress(ByteBuffer* pBuffer);
	void 		onHttpGoogleGeoCode(ByteBuffer* pBuffer);
	void 		onHttpDaumGeoCode(ByteBuffer* pBuffer);

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

#endif /* FORMAXADDRESSES_H_ */
