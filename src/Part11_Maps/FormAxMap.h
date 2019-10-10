/*
 * FormAxMap.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXMAP_H_
#define FORMAXMAP_H_

#include "AppBaseForm.h"
#include "HttpDaum.h"
#include <FWeb.h>

using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;

class FormAxMap
	: public AppBaseForm
	, public Osp::Web::Controls::ILoadingListener
	, public Osp::Web::Controls::IJavaScriptBridge
	, public IHttpFirstResponder
{
public:
	FormAxMap();
	virtual ~FormAxMap();
	virtual bool AtInitialize();

// Implementation
protected:
	static const int 		ID_MESSAGEBOX_CONFIRM = 101;
	static const int 		ID_MESSAGEBOX_CONFIRMDO = 102;

	Osp::Web::Controls::Web * 			__pWebView;
	HttpDaum 				* 			__pHttpDaumGeoCode;

protected:
	bool 		invokeReverseGeoCoding();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

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

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);
};

#endif /* FORMAXMAP_H_ */
