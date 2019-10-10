/*
 * FormAxPhonesDetail.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef FORMAXPHONESDETAIL_H_
#define FORMAXPHONESDETAIL_H_

#include "AppBaseForm.h"
#include "DatabasePhonebook.h"
#include "HttpGoogleGeo.h"
#include "HttpDaum.h"

class FormAxPhonesDetail
	: public AppBaseForm
	, public IHttpFirstResponder
{
public:
	FormAxPhonesDetail();
	virtual ~FormAxPhonesDetail();
	virtual bool AtInitialize();

protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	const static 	int 		ID_PUBLICINFO_PHONENUM = 111;
	const static 	int 		ID_PUBLICINFO_HOMEPAGE = 112;
	const static 	int 		ID_PUBLICINFO_ADDRESS = 113;

	PhoneItem 		* 		__passInfo;
	Panel 			* 		__pPanelL;
	Panel 			* 		__pPanelR;
	String 					__phoneNum;

	HttpGoogleGeo 	* 			__pHttpGoogleGeoCode;
	HttpDaum 		* 			__pHttpDaumGeoCode;

protected:
	void 		invokeGeoCoding(const String & address);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);
};

#endif /* FORMAXPHONESDETAIL_H_ */
