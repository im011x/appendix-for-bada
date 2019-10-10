/*
 * FormAxAddressWeb.h
 *
 *  Created on: 2012. 8. 21.
 *      Author: benny
 */

#ifndef FORMAXADDRESSWEB_H_
#define FORMAXADDRESSWEB_H_

#include "FormAxAddr.h"
#include <FWeb.h>

using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;


class FormAxAddressWeb
	: public FormAxAddr
	, public Osp::Web::Controls::ILoadingListener
{
public:
	FormAxAddressWeb();
	virtual ~FormAxAddressWeb();
	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;

	Osp::Web::Controls::Web * 			__pWebView;
	Osp::Base::String 					__url;

protected:

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
};

#endif /* FORMAXADDRESSWEB_H_ */
