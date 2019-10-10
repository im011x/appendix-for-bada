/*
 * FormAxMap.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxMap.h"

FormAxMap::FormAxMap() : __pWebView(null), __pHttpDaumGeoCode(null)
{
}

FormAxMap::~FormAxMap()
{
}

bool FormAxMap::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("지도 - 현재위치");

	SetPanelStype(BOTTOM_PANEL);
	SetPush(NO_PUSH);

	return true;
}

result FormAxMap::OnInitializing(void)
{
	result r = E_SUCCESS;

	// make parameter string
	String param = "?markImage=myLoc.png";
	param.Append("&lat=");
	param.Append(__currPosLatitude);
	param.Append("&lng=");
	param.Append(__currPosLongitude);

	AppLogDebug("map parameter = %S", param.GetPointer());

	String loadingUrl = "file:///Res/GoogleMap/map_center.html";
	loadingUrl.Append(param);

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);
	__pWebView->AddJavaScriptBridge(*this);
	//__pWebView->LoadUrl("/Res/GoogleMap/map.html");
	__pWebView->LoadUrl(loadingUrl);

	return r;
}

result FormAxMap::OnTerminating(void)
{
	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxMap::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	/*
	case FOOTER_ID_ITEM1:
	{
		String mapCommand; //= "setMyLocation(37.5665, 126.9780)";
		if (__currPosLatitude.IsEmpty() || __currPosLongitude.IsEmpty()) {
			mapCommand = "setMyLocation(37.5665, 126.9780)";
		}
		else {
			mapCommand.Format(128, L"setMyLocation(%S, %S)", __currPosLatitude.GetPointer(), __currPosLongitude.GetPointer());
		}

		String * p = __pWebView->EvaluateJavascriptN(mapCommand);
		AppLogDebug("OutPut of EvaluateJavascriptN :: %S \n", p->GetPointer());
		delete p;
	}
		break;
	case FOOTER_ID_ITEM2:
	{
		static bool toggleShowHide = false;

		String mapCommand = (toggleShowHide ? "showMarkers(true)" : "showMarkers(false)");
		toggleShowHide = !toggleShowHide;

		String * p = __pWebView->EvaluateJavascriptN(mapCommand);
		AppLogDebug("OutPut of EvaluateJavascriptN :: %S \n", p->GetPointer());
		delete p;
	}
		break;
	case FOOTER_ID_ITEM3:
	{
		String mapCommand = "deleteMarkers()";
		String * p = __pWebView->EvaluateJavascriptN(mapCommand);
		AppLogDebug("OutPut of EvaluateJavascriptN :: %S \n", p->GetPointer());
		delete p;
	}
		break;
	case FOOTER_ID_ITEM4:
		break;
	 */
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_CONFIRMDO:
		if (__pHttpDaumGeoCode) {
			SetKeyValue("주소검색_키워드", __pHttpDaumGeoCode->__name3);
			GotoForm(FORM_ID_ADDRESS);
		}
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

bool  FormAxMap::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	return false;
}

bool  FormAxMap::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	AppLogDebug(">>> OnLoadingRequested with Nav. Type =%d and URL = %S", type, url.GetPointer());

	if (url.StartsWith("appendix", 0) == true) {
		if (url.EndsWith("currAddress")) {
			AppLogDebug("native 기능: 현재 위치의 주소 목록 구하기가 호출됩니다. - %S", url.GetPointer());
			invokeReverseGeoCoding();
		}
		else if (url.EndsWith("nearStop")) {
			AppLogDebug("native 기능: 현재 위치의 주변 정류소 목록 구하기가 호출됩니다. - %S", url.GetPointer());

			SetKeyValue("정류소검색_위치_lat", __currPosLatitude);
			SetKeyValue("정류소검색_위치_lng", __currPosLongitude);
			GotoForm(FORM_ID_BUS_STOPNEAR);
		}
		else if (url.EndsWith("sendPosition")) {
			AppLogDebug("native 기능: 현재 위치 정보를 이메일로 보내기 기능이 호출됩니다. - %S", url.GetPointer());
		}
		else {
			AppLogDebug("native 기능이 정의되지 않았습니다. - %S", url.GetPointer());
		}
		return true;
	}

	commShow(true);
	return false;
}

void  FormAxMap::OnLoadingCompleted (void)
{
	AppLog(">>> OnLoadingCompleted");

	commHide(true);

}

DecisionPolicy  FormAxMap::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	return WEB_DECISION_CONTINUE;
}

Osp::Base::String FormAxMap::GetName(void)
{
	return String(L"MyJsBridge");
}

void FormAxMap::HandleJavaScriptRequestN(Osp::Web::Json::IJsonValue* pArg)
{

	result r = E_SUCCESS;
	JsonObject *pJsonObject = static_cast<JsonObject*>(pArg);
	IJsonValue *pValue = null;
	JsonString *pJsonStringValue = null;
	String key(L"data");     r = pJsonObject->GetValue(&key, pValue);
	pJsonStringValue = static_cast<JsonString*>(pValue);
	AppLog("data: %S\n", pJsonStringValue->GetPointer());

	/*
	TextBox *pTextBox = static_cast<TextBox *>(GetControl("IDC_TEXTBOX1"));
	String outStr = String(pJsonStringValue->GetPointer());
	r = pTextBox->SetText(outStr);
	pTextBox->Draw();
	pTextBox->Show();
	 */

}

bool FormAxMap::invokeReverseGeoCoding()
{
	if (__pHttpDaumGeoCode == null) {
		__pHttpDaumGeoCode = new HttpDaum(this);
	}

	bool bOK = __pHttpDaumGeoCode->GetReverse(__currPosLatitude, __currPosLongitude);

	if (bOK) {
		commShow(true);
	}

	return bOK;
}

void FormAxMap::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	commHide(true);
	__pHttpDaumGeoCode->ParseReverse(pBuffer, null);

	AppLogDebug("위치정보 (다음 Reverse GEO coding API) : %S, %S, %S - %S [%S]",
			__pHttpDaumGeoCode->__name1.GetPointer(),
			__pHttpDaumGeoCode->__name2.GetPointer(),
			__pHttpDaumGeoCode->__name3.GetPointer(),
			__pHttpDaumGeoCode->__nameCode.GetPointer(),
			__pHttpDaumGeoCode->__nameFormatted.GetPointer()
			 );

	UtilMessageBox::YesNoWithCallback("", "현 위치에 대한 주소는 \n\n" + __pHttpDaumGeoCode->__nameFormatted + "\n입니다. \n관련 주소검색을 진행합니다.",
			this, ID_MESSAGEBOX_CONFIRMDO, ID_MESSAGEBOX_CONFIRM);
	//UtilMessageBox::YesNoWithCallback("", "현 위치에 대한 주소는 \n\n" + __pHttpDaumGeoCode->__nameFormatted + "\n입니다.",
	//		this, ID_MESSAGEBOX_CONFIRM);
}

void FormAxMap::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide(true);
	UtilMessageBox::ConfirmWithCallback("", "오류 발생 " + message, this, ID_MESSAGEBOX_CONFIRM);
}

