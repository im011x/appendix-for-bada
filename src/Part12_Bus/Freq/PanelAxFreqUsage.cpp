/*
 * PanelAxFreqUsage.cpp
 *
 *  Created on: 2012. 8. 31.
 *      Author: benny
 */

#include "PanelAxFreqUsage.h"

PanelAxFreqUsage::PanelAxFreqUsage(FormAxBus * pHandler) : __pFormParent(pHandler), __pActionListener(null), __pWebView(null)
{
	__bLoaded = false;
}

PanelAxFreqUsage::~PanelAxFreqUsage()
{
}

bool PanelAxFreqUsage::Initialize(Rectangle rect, Container * pContainer, Osp::Ui::IActionEventListener* pActionListener)
{
	this->Construct(rect);
	pContainer->AddControl(*this);
	__pActionListener = pActionListener;

	Dimension sz = this->GetSize();
	Osp::Graphics::Rectangle rectClient(0,0, sz.width, sz.height);

	__pWebView = makeWebControl(rectClient, this);
	__pWebView->SetLoadingListener(this);

	return true;
}

void PanelAxFreqUsage::Load()
{
	AppLogDebug("loading web view control ...");

	if (__bLoaded == false) {
		__pWebView->LoadUrl("file:///Home/usage_freq.html");
	}

	__bLoaded = true;
}

void PanelAxFreqUsage::OnEstimatedProgress (int progress)
{
	AppLog(">>> OnEstimatedProgress - %d %%", progress);
}

void PanelAxFreqUsage::OnHttpAuthenticationCanceled (void)
{
	AppLog(">>> OnHttpAuthenticationCanceled");
}

bool  PanelAxFreqUsage::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	AppLog(">>> OnHttpAuthenticationRequestedN");
	return false;
}

bool  PanelAxFreqUsage::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	AppLog(">>> OnLoadingRequested (%d) - [%S]", type, url.GetPointer());
	return false;
}

void  PanelAxFreqUsage::OnLoadingCompleted (void)
{
	AppLog(">>> OnLoadingCompleted");
}

void  PanelAxFreqUsage::OnLoadingCanceled (void)
{
	AppLog(">>> OnLoadingCanceled");
}

void PanelAxFreqUsage::OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason)
{
	AppLog(">>> OnLoadingErrorOccurred - %S", reason.GetPointer());
}

void PanelAxFreqUsage::OnLoadingStarted (void)
{
	AppLog(">>> OnLoadingStarted");
}

DecisionPolicy  PanelAxFreqUsage::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	AppLog(">>> OnWebDataReceived");
	return WEB_DECISION_CONTINUE;
}

