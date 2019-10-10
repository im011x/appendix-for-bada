/*
 * FormAxSeoul.cpp
 *
 *  Created on: 2012. 6. 20.
 *      Author: benny
 */

#include "FormAxSeoul.h"

FormAxSeoul::FormAxSeoul()
{
	__pWebView = null;
}

FormAxSeoul::~FormAxSeoul()
{
}

bool FormAxSeoul::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("모바일 서울");

	SetTabName(0, "뒤로가기");
	SetTabName(1, "m 서울");
	SetTabName(2, "w 서울");
	SetTabName(3, "추가기능");

	return true;
}

result FormAxSeoul::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);
	//__pWebView->AddJavaScriptBridge(*this);
	__pWebView->LoadUrl("http://m.seoul.go.kr");

	return r;
}

result FormAxSeoul::OnTerminating(void)
{
	return AppBaseForm::OnTerminating();
}

void FormAxSeoul::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case FOOTER_ID_ITEM1:
		if (__pWebView)
			__pWebView->GoBack();
		break;
	case FOOTER_ID_ITEM2:
		if (__pWebView)
			__pWebView->LoadUrl("http://m.seoul.go.kr");
		break;
	case FOOTER_ID_ITEM3:
		if (__pWebView)
			__pWebView->LoadUrl("http://www.seoul.go.kr");
		break;
	case FOOTER_ID_ITEM4:
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxSeoul::OnEstimatedProgress (int progress)
{
	AppLog(">>> OnEstimatedProgress - %d %%", progress);
}

void FormAxSeoul::OnHttpAuthenticationCanceled (void)
{
	AppLog(">>> OnHttpAuthenticationCanceled");
}

bool  FormAxSeoul::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	AppLog(">>> OnHttpAuthenticationRequestedN");
	return false;
}

void  FormAxSeoul::OnLoadingCompleted (void)
{
	commHide();

	AppLog(">>> OnLoadingCompleted");
}

void  FormAxSeoul::OnLoadingCanceled (void)
{
	AppLog(">>> OnLoadingCanceled");
}

void FormAxSeoul::OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason)
{
	AppLog(">>> OnLoadingErrorOccurred");
}

void FormAxSeoul::OnLoadingStarted (void)
{
	AppLog(">>> OnLoadingStarted");
}

bool  FormAxSeoul::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	commShow();

	AppLog(">>> OnLoadingRequested");
	return false;
}

DecisionPolicy  FormAxSeoul::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	AppLog(">>> OnWebDataReceived");
	return WEB_DECISION_CONTINUE;
}

