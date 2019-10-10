/*
 * FormAxInfo.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxInfo.h"
#include "UtilApp.h"

FormAxInfo::FormAxInfo()
{
}

FormAxInfo::~FormAxInfo()
{
}

bool FormAxInfo::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("About Appendix");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxInfo::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);

/*
	if (pUrl) {
		commShow();
		__pWebView->LoadUrl(*pUrl);
	}
	else {
		//__pWebView->LoadUrl("http://m.seoul.go.kr");
		UtilMessageBox::ConfirmWithCallback("", "URL이 지정되지 않았습니다.", this, ID_MESSAGEBOX_NO_DATA);
	}
*/
	loadAbout(0);

	return r;
}

result FormAxInfo::OnTerminating(void)
{
	return AppBaseForm::OnTerminating();
}

void FormAxInfo::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case FOOTER_ID_ITEM1:
		loadAbout(0);
		break;
	case FOOTER_ID_ITEM2:
		loadAbout(1);
		break;
	case FOOTER_ID_ITEM3:
		loadAbout(2);
		break;
	case FOOTER_ID_ITEM4:
		loadAbout(3);
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxInfo::loadAbout(int nTab)
{
	String loadingUrl;

	switch (nTab) {
	case 1:
		loadingUrl = "file:///Home/about_bus.html";
		break;
	case 2:
		loadingUrl = "file:///Home/about_others.html";
		break;
	case 3:
		loadingUrl = "file:///Home/about_contact.html";
		break;
	case 0:
	default:
		loadingUrl = "file:///Home/about_overview.html";
		break;
	}

	__pWebView->LoadUrl(loadingUrl);
}

void FormAxInfo::buildFooter()
{
	Footer* pFooter = GetFooter();
	if (!pFooter) {
		return;
	}

	pFooter->SetStyle(FOOTER_STYLE_TAB);
	//pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON);
	//pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerItem1;
	footerItem1.Construct(FOOTER_ID_ITEM1);
	footerItem1.SetText("개요");
	pFooter->AddItem(footerItem1);

	FooterItem footerItem2;
	footerItem2.Construct(FOOTER_ID_ITEM2);
	footerItem2.SetText("버스");
	pFooter->AddItem(footerItem2);

	FooterItem footerItem3;
	footerItem3.Construct(FOOTER_ID_ITEM3);
	footerItem3.SetText("버스 외");
	pFooter->AddItem(footerItem3);

	FooterItem footerItem4;
	footerItem4.Construct(FOOTER_ID_ITEM4);
	footerItem4.SetText("기타");
	pFooter->AddItem(footerItem4);

	pFooter->AddActionEventListener(*this);
}

bool FormAxInfo::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	if (url.StartsWith("appendix", 0) == true) {
		if (url.EndsWith("appStore")) {
			AppLogDebug("appStore로 이동합니다 - %S", url.GetPointer());
			//UtilMessageBox::ConfirmWithCallback("", "appStore로 이동합니다.", this, ID_MESSAGEBOX_CONFIRM);
			UtilApp::RunSamsungApps();
		}
		else if (url.EndsWith("mailTo")) {
			AppLogDebug("mail 을 보냅니다 - %S", url.GetPointer());
			//SetKeyValue("정류소검색_위치_lat", *(GetStringByKey("위치_lat")));
			//SetKeyValue("정류소검색_위치_lng", *(GetStringByKey("위치_lng")));
			//GotoForm(FORM_ID_BUS_STOPNEAR);
			UtilMessageBox::ConfirmWithCallback("", "부록제공자의 메일 주소는\n\n im011x@daum.net \n\n입니다.", this, ID_MESSAGEBOX_CONFIRM);
		}
		else if (url.EndsWith("getVersion")) {
			String sVersion = Osp::App::Application::GetInstance()->GetAppVersion();

			AppLogDebug("버전을 확인합니다. = %S", sVersion.GetPointer());
			UtilMessageBox::ConfirmWithCallback("", "부록 현재 버전은 \n\n" + sVersion + "\n\n입니다.", this, ID_MESSAGEBOX_CONFIRM);
		}
		else {
			AppLogDebug("native 기능이 정의되지 않았습니다. - %S", url.GetPointer());
		}
		return true;
	}

	return FormAxWeb::OnLoadingRequested(url, type);
}
