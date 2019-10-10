/*
 * FormAxSeoul.h
 *
 *  Created on: 2012. 6. 20.
 *      Author: benny
 */

#ifndef FORMAXSEOUL_H_
#define FORMAXSEOUL_H_

#include "AppBaseForm.h"
#include <FWeb.h>

using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;

class FormAxSeoul
	: public AppBaseForm
	, public Osp::Web::Controls::ILoadingListener
	//, public Osp::Web::Controls::IJavaScriptBridge
{
public:
	FormAxSeoul();
	virtual ~FormAxSeoul();
	virtual bool AtInitialize();

// Implementation
protected:
	Osp::Web::Controls::Web * 			__pWebView;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// Osp::Web::Controls::ILoadingListener
	virtual void  OnEstimatedProgress (int progress);
	virtual void  OnHttpAuthenticationCanceled (void);
	virtual bool  OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication);
	virtual void  OnLoadingCanceled (void);
	virtual void  OnLoadingCompleted (void);
	virtual void  OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason);
	virtual bool  OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type);
	virtual void  OnLoadingStarted (void);
	virtual void  OnPageTitleReceived (const Osp::Base::String &title) {}
	virtual DecisionPolicy  OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader);

	// Osp::Web::Controls::IJavaScriptBridge
	//virtual Osp::Base::String  GetName();
	//virtual void  HandleJavaScriptRequestN (Osp::Web::Json::IJsonValue *pArg);
};

#endif /* FORMAXSEOUL_H_ */
