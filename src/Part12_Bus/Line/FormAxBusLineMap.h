/*
 * FormAxBusLineMap.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSLINEMAP2NEAR_H_
#define FORMAXBUSLINEMAP2NEAR_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"
#include <FWeb.h>

using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;

class FormAxBusLineMap
	: public FormAxBus
	, public IHttpFirstResponder
	, public Osp::Web::Controls::ILoadingListener
	, public Osp::Web::Controls::IJavaScriptBridge
{
public:
	FormAxBusLineMap();
	virtual ~FormAxBusLineMap();

	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	HttpBusSeoul * 						__httpSeoulBus;
	ArrayList * 						__pArrStops;
	Osp::Web::Controls::Web * 			__pWebView;
	XmlBusSeoulParseLineStops 			__parser;

	void 		invokeQuery();
	void 		invokeMap();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

	// Osp::Web::Controls::ILoadingListener
	virtual void  OnEstimatedProgress (int progress) {}
	virtual void  OnHttpAuthenticationCanceled (void) {}
	virtual bool  OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication);
	virtual void  OnLoadingCanceled (void) {}
	virtual void  OnLoadingCompleted (void);
	virtual void  OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason) {}
	virtual bool  OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type);
	virtual void  OnLoadingStarted (void) {}
	virtual void  OnPageTitleReceived (const Osp::Base::String &title) {}
	virtual DecisionPolicy  OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader);

	// Osp::Web::Controls::IJavaScriptBridge
	virtual Osp::Base::String  GetName();
	virtual void  HandleJavaScriptRequestN (Osp::Web::Json::IJsonValue *pArg);
};

#endif /* FORMAXBUSLINEMAP2NEAR_H_ */
