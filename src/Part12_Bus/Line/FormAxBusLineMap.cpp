/*
 * FormAxBusLineMap.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusLineMap.h"
#include "DataBusSeoul.h"

FormAxBusLineMap::FormAxBusLineMap() : __httpSeoulBus(null), __pArrStops(null), __pWebView(null)
{
}

FormAxBusLineMap::~FormAxBusLineMap()
{
}

bool FormAxBusLineMap::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	String * pName = GetStringByKey("노선정보_번호이름");
	if (pName) {
		SetTitle("노선지도 - " + *pName);
	}
	else
		SetTitle("노선지도");

	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusLineMap::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	invokeQuery();

	return r;
}

result FormAxBusLineMap::OnTerminating(void)
{
	if (__httpSeoulBus) {
		delete __httpSeoulBus;
	}

	if (__pArrStops) {
		__pArrStops->RemoveAll(true);
		delete __pArrStops;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusLineMap::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
	case ID_MESSAGEBOX_NO_KEYWORD:
		AppLogDebug("ID_MESSAGEBOX_NO_KEYWORD");
		GotoBack();
		break;
	case ID_MESSAGEBOX_NO_DATA:
		AppLogDebug("ID_MESSAGEBOX_NO_DATA");
		GotoBack();
		break;
	case ID_MESSAGEBOX_BAD_DATA:
		AppLogDebug("ID_MESSAGEBOX_BAD_DATA");
		GotoBack();
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusLineMap::invokeQuery()
{
	String * pKey = GetStringByKey("노선정보_검색키");
	if (pKey == null) {
		UtilMessageBox::ConfirmWithCallback("", "노선정보 검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}
	AppLogDebug("노선 정보 검색합니다. 키 = [%S]", pKey->GetPointer());

	if (__pArrStops == null) {
		__pArrStops = new ArrayList();
		__pArrStops->Construct();
	}

	if (__httpSeoulBus == null) {
		__httpSeoulBus = new HttpBusSeoul(this);
	}

	if (__httpSeoulBus->GetStopsByLine(*pKey))
		commShow(true);
}

void FormAxBusLineMap::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	bool bOK = __parser.ParseXml(pBuffer, __pArrStops);

	commHide(true);

	if (bOK) {
		invokeMap();
	}
	else {
		UtilMessageBox::ConfirmWithCallback("수신 에러", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}
}

void FormAxBusLineMap::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide(true);

	AppLogDebug("");
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxBusLineMap::invokeMap()
{
	//int ncenter = __pArrStops->GetCount() / 4;
	//BusSeoul_StopByLine* pCenterStop = static_cast<BusSeoul_StopByLine*> (__pArrStops->GetAt(ncenter));
	//AppLogDebug("line %S has total %d stops and center (seq = %d, X/Y = %S,%S)",
	//		pCenterStop->__busRouteNm.GetPointer(), __pArrStops->GetCount(), ncenter, pCenterStop->__gpsX.GetPointer(), pCenterStop->__gpsY.GetPointer());

	Panel * bottomPanel = GetPanel();

	__pWebView = makeWebControl(bottomPanel->GetBounds(), bottomPanel);
	__pWebView->SetLoadingListener(this);
	__pWebView->AddJavaScriptBridge(*this);
	__pWebView->LoadUrl("/Res/GoogleMap/map.html");

}

bool  FormAxBusLineMap::OnHttpAuthenticationRequestedN (const Osp::Base::String &host, const Osp::Base::String &realm, const Osp::Web::Controls::AuthenticationChallenge &authentication)
{
	AppLogDebug("");
	return false;
}

void  FormAxBusLineMap::OnLoadingCompleted (void)
{
	AppLogDebug(">>> OnLoadingCompleted");

	/*
	String mapCommand = "setMarker(37.5665, 126.9780)";
	String * p = __pWebView->EvaluateJavascriptN(mapCommand);
	AppLogDebug("OutPut of EvaluateJavascriptN :: %S \n", p->GetPointer());
	delete p;
	 */

	commHide(true);

}

bool  FormAxBusLineMap::OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type)
{
	AppLogDebug("");
	commShow(true);

	return false;
}

DecisionPolicy  FormAxBusLineMap::OnWebDataReceived (const Osp::Base::String &mime, const Osp::Net::Http::HttpHeader &httpHeader)
{
	AppLogDebug("");
	return WEB_DECISION_CONTINUE;
}

Osp::Base::String FormAxBusLineMap::GetName(void)
{
	AppLogDebug("");
	return String(L"MyJsBridge");
}

void FormAxBusLineMap::HandleJavaScriptRequestN(Osp::Web::Json::IJsonValue* pArg)
{
	AppLogDebug("");

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
