/*
 * FormAxAddrSearch.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "FormAxAddrSearch.h"

FormAxAddrSearch::FormAxAddrSearch(int addrType) : __nAddressType(addrType), __pPanelAddrOpt(null), __pPanelNewAddrOpt(null), __pHttpGoogleGeoCode(null), __pHttpDaumGeoCode(null)
{
	if (__nAddressType == 0)
		__searchOption = 0;
	else
		__searchOption = 3;
}

FormAxAddrSearch::~FormAxAddrSearch()
{
}

bool FormAxAddrSearch::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER);

	SetTitle("주소검색");

	//SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxAddrSearch::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxAddr::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int 	nScreenWidth  = dimBottomSize.width;
	int x = 10;
	int y = 0;

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////

	Panel * pNearPanel = makePanel(Rectangle(0, y, nScreenWidth, 80), bottomPanel, "appendix/transparent.png", true);

	int panelW = pNearPanel->GetWidth();
	//makeLabelText(Rectangle(      10,  0, panelW-20,   30),  "현재위치 주소를 검색어로 합니다.", pNearPanel, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	//makeButton(Rectangle(         10, 40, panelW/2-20, 60),  "구글방식", pNearPanel, ID_SEARCH_NEAR_GOOGLE, this);
	//makeButton(Rectangle(panelW/2+10, 40, panelW/2-20, 60),  "다음방식", pNearPanel, ID_SEARCH_NEAR_DAUM, this);
	makeButton(Rectangle(10, 10, panelW-20, 60),  "검색어를 현재위치 주소로 ", pNearPanel, ID_SEARCH_NEAR_DAUM, this);
	//makeButton(Rectangle(10, 80, panelW-20, 60),  "검색어를 한국어 음성으로", pNearPanel, ID_SEARCH_BY_VOICE, this);

	y += 80+20;
	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////

	__pSearchEdit = makeEditField(Rectangle(x, y, nScreenWidth-120, 60),  EDIT_FIELD_STYLE_NORMAL, "검색어를 입력하세요.", GetScrollPanel(), //bottomPanel,
			this, 40, ID_SEARCH_CANCEL, ID_SEARCH_DO, "취소", "확인");

	makeButton(Rectangle(nScreenWidth-100, y, 90, 60), "appendix/bt_eraser.png", "appendix/bt_eraser_p.png", "",
			bottomPanel, ID_SEARCH_CLEAR, this)->SetSpeechText("검색어를 지웁니다.");
	y += (60+20);

	makeButton(Rectangle(10, y, panelW-20, 60),  "검색어를 한국어 음성으로", bottomPanel, ID_SEARCH_BY_VOICE, this);
	y += (60+30);

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////

	__pPanelAddrOpt = makePanel(Rectangle(x, y, nScreenWidth-20, 200), bottomPanel, "appendix/transparent.png", true);
	__pPanelNewAddrOpt = makePanel(Rectangle(x, y, nScreenWidth-20, 200), bottomPanel, "appendix/transparent.png", true);

	if (__nAddressType == 0) {
		__pPanelAddrOpt->SetShowState(true);
		__pPanelNewAddrOpt->SetShowState(false);
	}
	else {
		__pPanelAddrOpt->SetShowState(false);
		__pPanelNewAddrOpt->SetShowState(true);
	}

	//y += (180 + 50);
	y = dimBottomSize.height - (60+20);
	makeButton(Rectangle(x, y, nScreenWidth-20, 60),  "검색", bottomPanel, ID_SEARCH_DO2, this);

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////

	y = 0;
	makeLabelText(Rectangle(10,0, __pPanelAddrOpt->GetWidth()-20, 30), "지번주소 검색 옵션을 선택하세요.", __pPanelAddrOpt, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 30+15;

	__pOptionsBtn[0] = makeStateButtonX(Rectangle(20, y, __pPanelAddrOpt->GetWidth()-40, 50),
			"동/읍/면,아파트/건물  이름", __pPanelAddrOpt, ID_SEARCH_OPTION_1, this, (__searchOption == 0));
	y += 50;
	__pOptionsBtn[1] = makeStateButtonX(Rectangle(20, y, __pPanelAddrOpt->GetWidth()-40, 50),
			"시/군/구 이름", __pPanelAddrOpt, ID_SEARCH_OPTION_2, this, (__searchOption == 1));
	y += 50;
	__pOptionsBtn[2] = makeStateButtonX(Rectangle(20, y, __pPanelAddrOpt->GetWidth()-40, 50),
			"우편번호", __pPanelAddrOpt, ID_SEARCH_OPTION_3, this, (__searchOption == 2));


	y = 0;
	makeLabelText(Rectangle(10,0,__pPanelNewAddrOpt->GetWidth()-20, 30), "도로명주소(새주소) 검색 옵션을 선택하세요.", __pPanelNewAddrOpt, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 30+15;

	__pOptionsBtn[3] = makeStateButtonX(Rectangle(20, y, __pPanelNewAddrOpt->GetWidth()-40, 50),
			"동/읍/면 이름", __pPanelNewAddrOpt, ID_SEARCH_OPTION_4, this, (__searchOption == 3));
	y += 50;
	__pOptionsBtn[4] = makeStateButtonX(Rectangle(20, y, __pPanelNewAddrOpt->GetWidth()-40, 50),
			"도로 이름 (ex 세종로 18)", __pPanelNewAddrOpt, ID_SEARCH_OPTION_5, this, (__searchOption == 4));
	y += 50;
	__pOptionsBtn[5] = makeStateButtonX(Rectangle(20, y, __pPanelNewAddrOpt->GetWidth()-40, 50),
			"우편번호", __pPanelNewAddrOpt, ID_SEARCH_OPTION_6, this, (__searchOption == 5));

	//
	String * pKeyword  = GetStringByKey("주소검색_키워드");
	if (pKeyword) {
		__pSearchEdit->SetText(*pKeyword);
		__pSearchEdit->RequestRedraw();
		call_search_form();
	}

	return r;
}

bool FormAxAddrSearch::AtWakeup()
{
	AppLog("FormAxAddrSearch::AtWakeup()");

	InitIntent();

	return true;
}

result FormAxAddrSearch::OnTerminating(void)
{
	if (__pHttpGoogleGeoCode) {
		delete __pHttpGoogleGeoCode;
	}

	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	return FormAxAddr::OnTerminating();
}

void FormAxAddrSearch::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_SEARCH_OPTION_1:
	case ID_SEARCH_OPTION_2:
	case ID_SEARCH_OPTION_3:
	case ID_SEARCH_OPTION_4:
	case ID_SEARCH_OPTION_5:
	case ID_SEARCH_OPTION_6:
	{
		for (int i=0; i<6; i++) {
			__pOptionsBtn[i]->SetSelection(false);
		}
		//
		__searchOption = actionId-ID_SEARCH_OPTION_1;
		__pOptionsBtn[__searchOption]->SetSelection(true);

		if (__searchOption == 2 || __searchOption == 5) {
			__pSearchEdit->SetCurrentInputModeCategory(EDIT_INPUTMODE_NUMERIC);
		}
		else {
			__pSearchEdit->SetCurrentInputModeCategory(EDIT_INPUTMODE_ALPHA);
		}

		this->RequestRedraw();
	}
		break;
	case ID_SEARCH_CANCEL:
	case ID_SEARCH_CLEAR:
		if (actionId == ID_SEARCH_CANCEL)
			__pSearchEdit->HideKeypad();
		__pSearchEdit->SetText("");
		__pSearchEdit->RequestRedraw();
		break;
	case ID_SEARCH_DO:
		__pSearchEdit->HideKeypad();
	case ID_SEARCH_DO2:
		call_search_form();
		break;
	case ID_SEARCH_NEAR_GOOGLE:
		invokeReverseGeoGoogle();
		break;
	case ID_SEARCH_NEAR_DAUM:
		invokeReverseGeoDaum();
		break;
	case ID_SEARCH_BY_VOICE:
		AppLogDebug("음성 인식으로 검색어를 만듭니다.");
		STT();
		break;
	case ID_MESSAGEBOX_CONFIRM:
		break;
	default:
		FormAxAddr::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxAddrSearch::call_search_form()
{
	String sKeyword = __pSearchEdit->GetText();
	if (sKeyword.IsEmpty()) {
		return;
	}

	sKeyword.Trim();

	SetKeyValue("주소검색_대상", __searchOption);
	SetKeyValue("주소검색_키", sKeyword);
	GotoForm(FORM_ID_ADDRESS_LIST);

}

void FormAxAddrSearch::OnSttCompleted(String& string, SpeechToTextWarning warning)
{
	FormAxAddr::OnSttCompleted(string, warning);

	__pSearchEdit->SetText(string);
	__pSearchEdit->SetFocus();
	this->RequestRedraw();
}

void FormAxAddrSearch::OnSttError(SpeechToTextError error, const String & message)
{
	FormAxAddr::OnSttError(error, message);

	__pSearchEdit->SetText("");
	__pSearchEdit->SetFocus();
	this->RequestRedraw();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxAddrSearch::invokeReverseGeoGoogle()
{
	if (__pHttpGoogleGeoCode == null) {
		__pHttpGoogleGeoCode = new HttpGoogleGeo(this);
	}

	if (__pHttpGoogleGeoCode->GetReverse(__currPosLatitude, __currPosLongitude))
		commShow();
}

void FormAxAddrSearch::invokeReverseGeoDaum()
{
	if (__pHttpDaumGeoCode == null) {
		__pHttpDaumGeoCode = new HttpDaum(this);
	}

	if (__pHttpDaumGeoCode->GetReverse(__currPosLatitude, __currPosLongitude))
		commShow();

}

void FormAxAddrSearch::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	commHide();

	if (__pHttpGoogleGeoCode && __pHttpGoogleGeoCode->GetHandler() == pHttp) {
		__pHttpGoogleGeoCode->ParseReverse(pBuffer, null);
	}
	else if (__pHttpDaumGeoCode && __pHttpDaumGeoCode->GetHandler() == pHttp) {
		__pHttpDaumGeoCode->ParseReverse(pBuffer, null);

#ifdef _DEBUG_DONE
		String message;
		message.Format(512, L"다음결과는 %S, %S,%S,%S, %S",
				__pHttpDaumGeoCode->__nameCode.GetPointer(),
				__pHttpDaumGeoCode->__name1.GetPointer(),
				__pHttpDaumGeoCode->__name2.GetPointer(),
				__pHttpDaumGeoCode->__name3.GetPointer(),
				__pHttpDaumGeoCode->__nameFormatted.GetPointer()
				);

		UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_CONFIRM);
#endif

		if (__searchOption == 1) {
			__pSearchEdit->SetText(__pHttpDaumGeoCode->__name2);
		}
		else {
			__pSearchEdit->SetText(__pHttpDaumGeoCode->__nameCode);
		}

		__pSearchEdit->SetFocus();
		this->RequestRedraw();
	}
}

void FormAxAddrSearch::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	AppLogDebug("");
	commHide();
}

