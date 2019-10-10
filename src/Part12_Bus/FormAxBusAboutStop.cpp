/*
 * FormAxBusStop.cpp
 *
 *  Created on: 2012. 7. 27.
 *      Author: benny
 */

#include "FormAxBusAboutStop.h"

FormAxBusAboutStop::FormAxBusAboutStop() : __pSearchEdit(null)
{
	__pOptionsBtn[0] = __pOptionsBtn[1] = null;
	__searchOption = 0;
}

FormAxBusAboutStop::~FormAxBusAboutStop()
{
}

bool FormAxBusAboutStop::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("정류소 검색");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusAboutStop::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int y = 0;

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
	Panel * panel_easyIn = makePanel(Rectangle(0, y, dimBottomSize.width, 80), bottomPanel, "appendix/transparent.png", true);
	y += (80);

	Panel * panel_search = makePanel(Rectangle(0, y, dimBottomSize.width, 360), bottomPanel, "appendix/transparent.png", true);
	y += (280 + 10 + 70);

	Panel * panel_address = makePanel(Rectangle(0, y, dimBottomSize.width, 190), bottomPanel, "appendix/transparent.png", true);
	y += (190 + 10);

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
	int width  = panel_easyIn->GetWidth();

	y = 10;
	makeButton(Rectangle(10, y, width-20, 60),  "주변 정류소 검색", panel_easyIn, ID_GOTO_SEARCH_NEARSTOP, this);

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
	width  = panel_search->GetWidth();
	int x = 10;
	y = 10;

#if 0
	Point pt = panel_search->GetPosition();
	__pSearchEdit = makeEditField(Rectangle(x, y+pt.y, width-120, 60),  EDIT_FIELD_STYLE_NORMAL, "검색어를 입력하세요.", GetScrollPanel(), //panel_search,
			this, 40, ID_SEARCH_CANCEL, ID_SEARCH_DO, "취소", "확인");
#else
	__pSearchEdit = makeEditField(Rectangle(x, y, width-120, 60),  EDIT_FIELD_STYLE_NORMAL, "검색어를 입력하세요.", panel_search,
			this, 40, ID_SEARCH_CANCEL, ID_SEARCH_DO, "취소", "확인");
#endif

	makeButton(Rectangle(width-100, y, 90, 60), "appendix/bt_eraser.png", "appendix/bt_eraser_p.png", "",
			panel_search, ID_SEARCH_CLEAR, this)->SetSpeechText("검색어를 지웁니다.");;
	y += (60+20);


	makeButton(Rectangle(10, y, width-20, 60),  "검색어를 한국어 음성으로", panel_search, ID_SEARCH_BY_VOICE, this);
	y += 70;


	__pOptionsBtn[0] = makeStateButtonX(Rectangle(20, y, width-40, 50),
			//"common/checkbox_normal_401x50.png", "common/checkbox_selected_401x50.png",
			"정류소 명칭검색", panel_search, ID_SEARCH_OPTION_1, this, (__searchOption == 0));
	y += 50;
	__pOptionsBtn[1] = makeStateButtonX(Rectangle(20, y, width-40, 50),
			//"common/checkbox_normal_401x50.png", "common/checkbox_selected_401x50.png",
			"정류소 코드검색", panel_search, ID_SEARCH_OPTION_2, this, (__searchOption == 1));
	y += 50 + 20;

	makeButton(Rectangle(10, y, width-20, 60),  "검색", panel_search, ID_SEARCH_DO2, this);

	///////////////////////////////////////////////////////////////////////////
	//
	///////////////////////////////////////////////////////////////////////////
	width  = panel_address->GetWidth();

	y = 10;
	makeLabelText(Rectangle(10, y, width-20, 30), "주소 이용한 정류소 검색 과정은 다음과 같습니다.", panel_address, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 30;
	makeLabelText(Rectangle(20, y, width-20, 60), " * 주소 검색 -> 결과 -> (지도) -> 주변 정류소", panel_address, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 60;
	makeButton(Rectangle(10, y, width-20, 60),  "주소검색", panel_address, ID_GOTO_SEARCH_ADDRESS, this);
	y += 60+10;


	return r;
}

bool FormAxBusAboutStop::AtWakeup()
{
	AppLog("AtWakeup()");
	InitIntent();
	return true;
}

result FormAxBusAboutStop::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}

void FormAxBusAboutStop::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_SEARCH_OPTION_1:
	case ID_SEARCH_OPTION_2:
	{
		for (int i=0; i<2; i++) {
			__pOptionsBtn[i]->SetSelection(false);
		}
		//
		__searchOption = actionId-ID_SEARCH_OPTION_1;
		__pOptionsBtn[__searchOption]->SetSelection(true);

		if (__searchOption == 0) {
			__pSearchEdit->SetGuideText("정류소 명칭을 입력하세요");
			__pSearchEdit->SetCurrentInputModeCategory(EDIT_INPUTMODE_ALPHA);
		}
		else if (__searchOption == 1) {
			__pSearchEdit->SetGuideText("정류소 코드를 입력하세요");
			__pSearchEdit->SetCurrentInputModeCategory(EDIT_INPUTMODE_NUMERIC);
		}

		__pSearchEdit->SetFocus();
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
	case ID_GOTO_SEARCH_ADDRESS:
		GotoForm(FORM_ID_ADDRESS);
		break;
	case ID_SEARCH_BY_VOICE:
		AppLogDebug("음성 인식으로 검색어를 만듭니다.");
		STT();
		break;
	case ID_GOTO_SEARCH_NEARSTOP:
		SetKeyValue("정류소검색_위치_lat", __currPosLatitude);
		SetKeyValue("정류소검색_위치_lng", __currPosLongitude);
		GotoForm(FORM_ID_BUS_STOPNEAR);
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

void FormAxBusAboutStop::call_search_form()
{
	String sKeyword = __pSearchEdit->GetText();
	if (sKeyword.IsEmpty()) {
		return;
	}

	SetKeyValue("정류소검색_키", sKeyword);

	switch (__searchOption) {
	case 0:  // 명칭 검색
		GotoForm(FORM_ID_BUS_STOPNAMES);
		break;
	case 1:  // 코드 검색
		GotoForm(FORM_ID_BUS_STOPLINES);
		break;
	default:
		break;
	}

}

void FormAxBusAboutStop::OnSttCompleted(String& string, SpeechToTextWarning warning)
{
	FormAxBus::OnSttCompleted(string, warning);

	__pSearchEdit->SetText(string);
	__pSearchEdit->SetFocus();
	this->RequestRedraw();
}

void FormAxBusAboutStop::OnSttError(SpeechToTextError error, const String & message)
{
	FormAxBus::OnSttError(error, message);

	__pSearchEdit->SetText("");
	__pSearchEdit->SetFocus();
	this->RequestRedraw();
}
