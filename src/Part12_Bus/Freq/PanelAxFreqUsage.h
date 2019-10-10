/*
 * PanelAxFreqUsage.h
 *
 *  Created on: 2012. 8. 31.
 *      Author: benny
 */

#ifndef PANELAXFREQUSAGE_H_
#define PANELAXFREQUSAGE_H_

#include <FUi.h>
#include <FWeb.h>
#include "ControlMaker.h"

class FormAxBus;

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Web::Controls;
using namespace Osp::Web::Json;

class PanelAxFreqUsage
	: public Osp::Ui::Controls::Panel
	, public ControlMaker
	, public Osp::Web::Controls::ILoadingListener
{
public:
	PanelAxFreqUsage(FormAxBus * pHandler);
	virtual ~PanelAxFreqUsage();

// Implementation
protected:
	FormAxBus 						* 		__pFormParent;
	Osp::Ui::IActionEventListener	* 		__pActionListener;
	Osp::Web::Controls::Web 		* 		__pWebView;

	bool		__bLoaded;

public:
	bool 		Initialize(Rectangle rect, Container * pContainer, Osp::Ui::IActionEventListener* pActionListener);
	void		Load();

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

#endif /* PANELAXFREQUSAGE_H_ */
