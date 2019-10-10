/*
 * FormAxPhonesDetail.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "FormAxPhonesDetail.h"
#include "UtilApp.h"

FormAxPhonesDetail::FormAxPhonesDetail() : __pHttpGoogleGeoCode(null), __pHttpDaumGeoCode(null)
{
}

FormAxPhonesDetail::~FormAxPhonesDetail()
{
}

bool FormAxPhonesDetail::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	__passInfo = static_cast<PhoneItem *> (GetObjectByKey("공공전화_상세"));

	if (__passInfo) {
		SetTitle(__passInfo->__owner_name);
	}

	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxPhonesDetail::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();
	Panel * bottomPanel = GetPanel();

	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;

	// 광고 공간 확보!
	Panel * panel = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	createAds(Rectangle(0, y, dimBottomSize.width, 80), panel);
	y = 80;

	int x = 10;
	y += 50;

	__pPanelL = makePanel(Rectangle(x, y, 434/3, 320+10), bottomPanel, "appendix/bg_434x190_blue.png", true);
	__pPanelR = makePanel(Rectangle(x+434/3, y, 434*2/3, 320+10), bottomPanel, "appendix/bg_434x190_white.png", true);
	y += 330;

	int w = 434/3 - 20;
	makeLabelText(Rectangle(10,   0, w, 80), "기 관  명", __pPanelL, 30, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10,  80, w, 80), "전화번호", __pPanelL, 30, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10, 160, w, 80), "홈페이지", __pPanelL, 30, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10, 240, w, 80), "주      소", __pPanelL, 30, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);

	//makeButton(Rectangle(10, 80*1, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelL, ID_PUBLICINFO_PHONENUM, this);
	//makeButton(Rectangle(10, 80*2, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelL, ID_PUBLICINFO_HOMEPAGE, this);
	//makeButton(Rectangle(10, 80*3, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelL, ID_PUBLICINFO_ADDRESS, this);

	//
	__phoneNum = __passInfo->__num_area;
	if (!__phoneNum.IsEmpty()) {
		__phoneNum.Append("-");
	}
	__phoneNum.Append(__passInfo->__num_station);
	if (!__phoneNum.IsEmpty()) {
		__phoneNum.Append("-");
	}
	__phoneNum.Append(__passInfo->__num_last);

	w = 434*2/3 - 20;
	makeLabelText(Rectangle(10,   0, w, 80), __passInfo->__owner_name, __pPanelR, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10,  80, w, 80), __phoneNum,                 __pPanelR, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	makeLabelText(Rectangle(10, 160, w, 80), __passInfo->__home_page,  __pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT)->SetWrap();
	makeLabelText(Rectangle(10, 240, w, 80), __passInfo->__address,    __pPanelR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT)->SetWrap();

	//makeButton(Rectangle(10, 80*1, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelR, ID_PUBLICINFO_PHONENUM, this);
	//makeButton(Rectangle(10, 80*2, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelR, ID_PUBLICINFO_HOMEPAGE, this);
	//makeButton(Rectangle(10, 80*3, w, 80), "common/transparent.png", "common/transparent.png", "", __pPanelR, ID_PUBLICINFO_ADDRESS, this);

	//
/*
	if (__pAd) {
		RequestId reqId  = INVALID_REQUEST_ID;
		__pAd->RequestAd(reqId);
	}
*/

	MyButton * pbtn;

	w = 100;
	y += 50;

	pbtn = makeButton(Rectangle(45      , y,  92, 78), "appendix/phone_call.png", "", "", bottomPanel, ID_PUBLICINFO_PHONENUM, this);
	pbtn->SetSpeechText("전화");
	pbtn = makeButton(Rectangle(45*2+92, y, 100, 78), "appendix/phone_www.png", "", "", bottomPanel, ID_PUBLICINFO_HOMEPAGE, this);
	pbtn->SetSpeechText("웹");
	pbtn = makeButton(Rectangle(45*3+200, y,  77, 83), "appendix/phone_map.png", "", "", bottomPanel, ID_PUBLICINFO_ADDRESS, this);
	pbtn->SetSpeechText("지도");

	return r;
}

result FormAxPhonesDetail::OnTerminating(void)
{
	if (__pHttpGoogleGeoCode) {
		delete __pHttpGoogleGeoCode;
	}

	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxPhonesDetail::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_PUBLICINFO_PHONENUM:
		UtilApp::AppCall(__phoneNum);
		break;
	case ID_PUBLICINFO_HOMEPAGE:
		if (__passInfo->__home_page.StartsWith("http://", 0))
			UtilApp::AppWeb(__passInfo->__home_page);
		break;
	case ID_PUBLICINFO_ADDRESS:
		invokeGeoCoding(__passInfo->__address);
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxPhonesDetail::invokeGeoCoding(const String & address)
{
	//HttpDaum 		* 			__pHttpDaumGeoCode;  // 이것은 보류 ...

	if (__pHttpGoogleGeoCode == null) {
		__pHttpGoogleGeoCode = new HttpGoogleGeo(this);
	}

	__pHttpGoogleGeoCode->GetPosition(address); // 구글은 한개를 돌려 줌

	commShow();
}

void FormAxPhonesDetail::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	commHide();

	__pHttpGoogleGeoCode->ParsePosition(pBuffer, null);

#ifdef _DEBUG_XXX
	String smessage = "구글 위치조회 결과\n \n";
	smessage.Append(__pHttpGoogleGeoCode->__formattedAddress);
	smessage.Append("\n");
	smessage.Append(__pHttpGoogleGeoCode->__lat);
	smessage.Append("\n");
	smessage.Append(__pHttpGoogleGeoCode->__lng);
	UtilMessageBox::ConfirmWithCallback("", smessage, this, ID_MESSAGEBOX_CONFIRM);
#endif

	SetKeyValue("위치_lat", __pHttpGoogleGeoCode->__lat);
	SetKeyValue("위치_lng", __pHttpGoogleGeoCode->__lng);
	SetKeyValue("위치_info", __pHttpGoogleGeoCode->__formattedAddress);
	SetKeyValue("위치_title", __passInfo->__owner_name);

	GotoForm(FORM_ID_MAP_CENTER);

}

void FormAxPhonesDetail::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}
