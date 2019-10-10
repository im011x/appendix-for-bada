/*
 * FormAxAddrSearch.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef FORMAXADDRSEARCH_H_
#define FORMAXADDRSEARCH_H_

#include "FormAxAddr.h"
#include "HttpGoogleGeo.h"
#include "HttpDaum.h"

class FormAxAddrSearch
	: public FormAxAddr
	, public IHttpFirstResponder
{
public:
	FormAxAddrSearch(int addrType = 0);
	virtual ~FormAxAddrSearch();
	virtual bool AtInitialize();

// Implementation
protected:
	const 	static 	int 		ID_SEARCH_CLEAR = 110;
	const 	static 	int 		ID_SEARCH_CANCEL = 111;
	const 	static 	int 		ID_SEARCH_DO = 112;
	const 	static 	int 		ID_SEARCH_DO2 = 113;
	const 	static 	int 		ID_SEARCH_NEAR_GOOGLE = 114;
	const 	static 	int 		ID_SEARCH_NEAR_DAUM = 115;
	const 	static 	int 		ID_SEARCH_BY_VOICE = 116;

	const 	static 	int 		ID_MESSAGEBOX_CONFIRM = 201;

	const 	static 	int 		ID_SEARCH_OPTION_1 = 501;
	const 	static 	int 		ID_SEARCH_OPTION_2 = 502;
	const 	static 	int 		ID_SEARCH_OPTION_3 = 503;
	const 	static 	int 		ID_SEARCH_OPTION_4 = 504;
	const 	static 	int 		ID_SEARCH_OPTION_5 = 505;
	const 	static 	int 		ID_SEARCH_OPTION_6 = 506;

	int 								__nAddressType;
	EditField 	* 						__pSearchEdit;
	Panel 		* 						__pPanelAddrOpt;
	Panel 		* 						__pPanelNewAddrOpt;
	MyButton 	* 						__pOptionsBtn[6];

	HttpGoogleGeo 	* 			__pHttpGoogleGeoCode;
	HttpDaum 		* 			__pHttpDaumGeoCode;

private:
	int 					__searchOption;

protected:
	void 			call_search_form();
	void 			invokeReverseGeoGoogle();
	void 			invokeReverseGeoDaum();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	virtual bool AtWakeup();

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

	// ISTTHandler 가운데 중요한 것 하나만 !
	virtual void OnSttCompleted(String& string, SpeechToTextWarning warning);
	virtual void OnSttError(SpeechToTextError error, const String & message);

};

#endif /* FORMAXADDRSEARCH_H_ */
