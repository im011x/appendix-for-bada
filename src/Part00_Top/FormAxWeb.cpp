/*
 * FormAxWeb.cpp
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#include "FormAxWeb.h"
#include "UtilApp.h"

FormAxWeb::FormAxWeb() : __pWebView(null)
{
}

FormAxWeb::~FormAxWeb()
{
}

bool FormAxWeb::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	String * pKey = GetStringByKey("웹_title");
	if (pKey == null) {
		SetTitle("웹");
	}
	else {
		SetTitle(*pKey);
	}

	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxWeb::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	String * pUrl = GetStringByKey("웹_URL");

	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);

	if (pUrl) {
		commShow(true);
		__pWebView->LoadUrl(*pUrl);
	}
	else {
		//__pWebView->LoadUrl("http://m.seoul.go.kr");
		UtilMessageBox::ConfirmWithCallback("", "URL이 지정되지 않았습니다.", this, ID_MESSAGEBOX_NO_DATA);
	}

	return r;
}

result FormAxWeb::OnTerminating(void)
{
	return AppBaseForm::OnTerminating();
}

void FormAxWeb::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	case REQUEST_GOTO_BACK:
		if (__pWebView->CanGoBack()) {
			__pWebView->GoBack();
		}
		else {
			AppBaseForm::OnActionPerformed(source, actionId);
		}
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxWeb::OnEstimatedProgress (int progress)
{
	AppLog(">>> OnEstimatedProgress - %d %%", progress);

	if (progress > 50) {
		commHide(true);
	}
}

void FormAxWeb::OnHttpAuthenticationCanceled (void)
{
	AppLog(">>> OnHttpAuthenticationCanceled");
}

bool  FormAxWeb::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	AppLog(">>> OnHttpAuthenticationRequestedN");
	return false;
}

bool  FormAxWeb::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	commShow(true);

	__url = url;
	AppLog(">>> OnLoadingRequested (%d) - [%S]", type, url.GetPointer());
	return false;
}

void  FormAxWeb::OnLoadingCompleted (void)
{
	AppLog(">>> OnLoadingCompleted");
}

void  FormAxWeb::OnLoadingCanceled (void)
{
	AppLog(">>> OnLoadingCanceled");
}

void FormAxWeb::OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason)
{
	AppLog(">>> OnLoadingErrorOccurred - %S", reason.GetPointer());

	if (reason.CompareTo("video/mp4") == 0) {
		UtilApp::AppVideo(__url);
	}
}

void FormAxWeb::OnLoadingStarted (void)
{
	AppLog(">>> OnLoadingStarted");
}

DecisionPolicy  FormAxWeb::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	AppLog(">>> OnWebDataReceived");
	return WEB_DECISION_CONTINUE;
}

