/*
 * FormAxAddressWeb.cpp
 *
 *  Created on: 2012. 8. 21.
 *      Author: benny
 */

#include "FormAxAddressWeb.h"
#include "UtilApp.h"

FormAxAddressWeb::FormAxAddressWeb() : __pWebView(null)
{
}

FormAxAddressWeb::~FormAxAddressWeb()
{
}

bool FormAxAddressWeb::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("지번 - 도로명");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxAddressWeb::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxAddr::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y=0;

	// 위에 PC에서 사용할 주소 안내 문구~
	Panel * pPanelBG = makePanel(Rectangle(0,  y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png");
	makeLabelText(Rectangle(10, y, dimBottomSize.width-20, 80), "PC 브라우저에서 http://www.juso.go.kr 을 통해 보다 간편한 이용이 가능합니다.",
			pPanelBG, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT)->SetWrap();
	y += 80;

	rectClient.y += y;
	rectClient.height -= y;

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);

	commShow(true);
	__pWebView->LoadUrl("http://www.juso.go.kr");

	return r;
}

result FormAxAddressWeb::OnTerminating(void)
{
	return FormAxAddr::OnTerminating();
}

void FormAxAddressWeb::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
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
			FormAxAddr::OnActionPerformed(source, actionId);
		}
		break;
	default:
		FormAxAddr::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxAddressWeb::OnEstimatedProgress (int progress)
{
	AppLog(">>> OnEstimatedProgress - %d %%", progress);

	if (progress > 50) {
		commHide(true);
	}
}

void FormAxAddressWeb::OnHttpAuthenticationCanceled (void)
{
	AppLog(">>> OnHttpAuthenticationCanceled");
}

bool  FormAxAddressWeb::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	AppLog(">>> OnHttpAuthenticationRequestedN");
	return false;
}

bool  FormAxAddressWeb::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	commShow(true);

	__url = url;
	AppLog(">>> OnLoadingRequested (%d) - [%S]", type, url.GetPointer());
	return false;
}

void  FormAxAddressWeb::OnLoadingCompleted (void)
{
	AppLog(">>> OnLoadingCompleted");

	__pWebView->SetZoomLevel(1.5);
}

void  FormAxAddressWeb::OnLoadingCanceled (void)
{
	AppLog(">>> OnLoadingCanceled");
}

void FormAxAddressWeb::OnLoadingErrorOccurred (LoadingErrorType error, const Osp::Base::String &reason)
{
	AppLog(">>> OnLoadingErrorOccurred - %S", reason.GetPointer());

	if (reason.CompareTo("video/mp4") == 0) {
		UtilApp::AppVideo(__url);
	}
}

void FormAxAddressWeb::OnLoadingStarted (void)
{
	AppLog(">>> OnLoadingStarted");
}

DecisionPolicy  FormAxAddressWeb::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	AppLog(">>> OnWebDataReceived");
	return WEB_DECISION_CONTINUE;
}

