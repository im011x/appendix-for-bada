/*
 * FormAxMapCenter.cpp
 *
 *  Created on: 2012. 7. 26.
 *      Author: benny
 */

#include "FormAxMapCenter.h"
#include "UtilCode.h"

FormAxMapCenter::FormAxMapCenter() : __pWebView(null), __pHttpDaumGeoCode(null)
{
}

FormAxMapCenter::~FormAxMapCenter()
{
}

bool FormAxMapCenter::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	String * pKey = GetStringByKey("위치_title");
	if (pKey == null) {
		SetTitle("지도 보기");
	}
	else {
		SetTitle(*pKey);
	}

	SetPanelStype(BOTTOM_PANEL);
	SetPush(NO_PUSH);

	return true;
}

result FormAxMapCenter::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	// 일단 폼 파라미터를 분석한다.
	String * pLat  = GetStringByKey("위치_lat");
	String * pLng  = GetStringByKey("위치_lng");
	String * pInfo = GetStringByKey("위치_info");

	// make parameter string
	String param = "?markImage=marker_orange.png";

	if (pLat) {
		param.Append("&lat=");
		param.Append(*pLat);
	}
	if (pLng) {
		param.Append("&lng=");
		param.Append(*pLng);
	}
	if (pInfo) {
		param.Append("&info=");
		//param.Append(*pInfo);
		String encodedStr;

		/*
		Osp::Base::Utility::UrlEncoder::Encode(*pInfo, "UTF-8", encodedStr);
		//Osp::Base::Utility::UrlEncoder::Encode(*pInfo, "KSC5601", encodedStr);
		param.Append(encodedStr);
		 */

		String nextCommand;
		nextCommand.Append("<a href='appendix://?nearStop' target='_blank'>near stops</a><br/>");
		nextCommand.Append("<a href='appendix://?currAddress' target='_blank'>get address</a>");

		Osp::Base::Utility::UrlEncoder::Encode(nextCommand, "UTF-8", encodedStr);
		param.Append(nextCommand);

	}

	AppLogDebug("map parameter = %S", param.GetPointer());

	String loadingUrl = "file:///Res/GoogleMap/map_center.html";
	loadingUrl.Append(param);

	// 화면 생성
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Rectangle rectClient = bottomPanel->GetBounds();

	__pWebView = makeWebControl(rectClient, bottomPanel);
	__pWebView->SetLoadingListener(this);
	__pWebView->AddJavaScriptBridge(*this);
	__pWebView->LoadUrl(loadingUrl);

	return r;
}

result FormAxMapCenter::OnTerminating(void)
{
	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxMapCenter::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

bool  FormAxMapCenter::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	return false;
}

void  FormAxMapCenter::OnLoadingCompleted (void)
{
	AppLog(">>> OnLoadingCompleted");

	commHide(true);
}

bool  FormAxMapCenter::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	AppLogDebug(">>> OnLoadingRequested with Nav. Type =%d and URL = %S", type, url.GetPointer());

	if (url.StartsWith("appendix", 0) == true) {
		if (url.EndsWith("currAddress")) {
			AppLogDebug("native 기능: 현재 위치의 주소 목록 구하기가 호출됩니다. - %S", url.GetPointer());
			invokeReverseGeoCoding();
		}
		else if (url.EndsWith("nearStop")) {
			AppLogDebug("native 기능: 현재 위치의 주변 정류소 목록 구하기가 호출됩니다. - %S", url.GetPointer());
			SetKeyValue("정류소검색_위치_lat", *(GetStringByKey("위치_lat")));
			SetKeyValue("정류소검색_위치_lng", *(GetStringByKey("위치_lng")));
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

DecisionPolicy  FormAxMapCenter::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	return WEB_DECISION_CONTINUE;
}

Osp::Base::String FormAxMapCenter::GetName(void)
{
	return String(L"MyJsBridge");
}

void FormAxMapCenter::HandleJavaScriptRequestN(Osp::Web::Json::IJsonValue* pArg)
{
	result r = E_SUCCESS;
	JsonObject *pJsonObject = static_cast<JsonObject*>(pArg);
	IJsonValue *pValue = null;
	JsonString *pJsonStringValue = null;
	String key(L"data");

	r = pJsonObject->GetValue(&key, pValue);
	pJsonStringValue = static_cast<JsonString*>(pValue);
	AppLogDebug("data: %S\n", pJsonStringValue->GetPointer());

	UtilMessageBox::ConfirmWithCallback("", "현 위치에 대한 다음 처리를 선택합니다.", this, ID_MESSAGEBOX_CONFIRM);
}

bool FormAxMapCenter::invokeReverseGeoCoding()
{
	String * pLat  = GetStringByKey("위치_lat");
	String * pLng  = GetStringByKey("위치_lng");

	if (__pHttpDaumGeoCode == null) {
		__pHttpDaumGeoCode = new HttpDaum(this);
	}

	bool bOK = __pHttpDaumGeoCode->GetReverse(*pLat, *pLng);

	if (bOK) {
		commShow();
	}

	return bOK;
}

void FormAxMapCenter::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	commHide();

	__pHttpDaumGeoCode->ParseReverse(pBuffer, null);

	AppLogDebug("위치정보 (다음 Reverse GEO coding API) : %S, %S, %S - %S [%S]",
			__pHttpDaumGeoCode->__name1.GetPointer(),
			__pHttpDaumGeoCode->__name2.GetPointer(),
			__pHttpDaumGeoCode->__name3.GetPointer(),
			__pHttpDaumGeoCode->__nameCode.GetPointer(),
			__pHttpDaumGeoCode->__nameFormatted.GetPointer()
			 );

	UtilMessageBox::ConfirmWithCallback("", "현 위치에 대한 주소는 " + __pHttpDaumGeoCode->__nameFormatted, this, ID_MESSAGEBOX_CONFIRM);
}

void FormAxMapCenter::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", "오류 발생 " + message, this, ID_MESSAGEBOX_CONFIRM);
}

