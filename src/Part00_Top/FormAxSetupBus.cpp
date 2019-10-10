/*
 * FormAxSetupBus.cpp
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#include "FormAxSetupBus.h"

#define 	UPDATE_MIN_SCREEN 		5
#define 	UPDATE_MIN_SERVER 		20


FormAxSetupBus::FormAxSetupBus()
{
	for (int i=0; i<3; i++)
		__pOptionsMybus[i] = null;

	for (int i=0; i<2; i++)
		__pOptionsStoplines[i] = null;

	__nTapMybusGoto = 0;
	__nTapStoplineGoto = 0;
}

FormAxSetupBus::~FormAxSetupBus()
{
}

bool FormAxSetupBus::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("설정");
	//SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxSetupBus::OnInitializing(void)
{
	result r = E_SUCCESS;

	init();

	// TODO: Add your initialization code here
	FormAxSetup::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int w = dimBottomSize.width - 20;
	int y = 0;

	Panel * pPanelBG = makePanel(Rectangle(0,  y, dimBottomSize.width, 60), bottomPanel, "appendix/bg_white.png");
	makeLabelText(Rectangle(10, y, w-10, 60), "갱신주기", pPanelBG, 40, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 60;

	Panel * pPanelUpdateScreen = makePanel(Rectangle(0,  y, dimBottomSize.width, 200), bottomPanel);
	y += 210;
	Panel * pPanelUpdateServer = makePanel(Rectangle(0,  y, dimBottomSize.width, 200), bottomPanel);
	y += 220;

	// 추가
	Panel * pPanelBG2 = makePanel(Rectangle(0,  y, dimBottomSize.width, 60), bottomPanel, "appendix/bg_white.png");
	makeLabelText(Rectangle(10, 0, w-10, 60), "Tap 기능", pPanelBG2, 40, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 60;
	Panel * pPanelTap = makePanel(Rectangle(0,  y, dimBottomSize.width, 420), bottomPanel);
	y += 420 + 30;
	bottomPanel->SetSize(dimBottomSize.width, y);
	// 추가 중 ...


	//
	y = 0;
	int unit = pPanelUpdateScreen->GetWidth()/5;
	int x = 10;
	x += unit/2;

	makeLabelText(Rectangle(10, y, w-10, 40), "화   면 갱신", pPanelUpdateScreen, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 40;
	__pSliderScreen = makeSlider(Rectangle(x, y, unit*3-20, 100), pPanelUpdateScreen, UPDATE_MIN_SCREEN, 30, this, this);
	__pSliderScreen->SetValue(UPDATE_MIN_SCREEN);
	__pLabelTimeScreen = makeLabelText(Rectangle(x+unit*3+20, y, unit, 100), " 5 초", pPanelUpdateScreen, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 100;
	makeLabelText(Rectangle(20, y, w-30, 120/2), "잔여시간 정보 갱신주기를 설정합니다. (내버스/정류소 도착정보 화면에서 "
			"도착버스의 잔여시간 정보를 갱신하는 주기를 의미합니다.)\n"
			"리스트항목 swipe 위해 최소 시간을 5초로 사용합니다"
			, pPanelUpdateScreen, 25)->SetWrap();

	//
	y = 0;
	makeLabelText(Rectangle(10, y, w-10, 40), "데이터 갱신", pPanelUpdateServer, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 40;
	__pSliderServer = makeSlider(Rectangle(x, y, unit*3-20, 100), pPanelUpdateServer, UPDATE_MIN_SERVER, 60, this, this);
	__pSliderServer->SetValue(UPDATE_MIN_SERVER+10);
	__pLabelTimeServer = makeLabelText(Rectangle(x+unit*3+20, y, unit, 100), "30 초", pPanelUpdateServer, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 100;
	makeLabelText(Rectangle(20, y, w-30, 120/2), "서버 데이터 정보 갱신주기를 설정합니다. (내버스/정류소 도착정보/노선도 화면에서 "
			"관련 정보를 서버에서 다시 가져오는 주기를 의미합니다."
			, pPanelUpdateServer, 25)->SetWrap();

	// 추가 중 계속 ...
	y = 10;
	makeLabelText(Rectangle(10, y, w-10, 40), "내 버스 리스트 항목 선택 시", pPanelTap, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 40;
	__pOptionsMybus[0] = makeStateButtonX(Rectangle(20,  y, w-20, 50), "이동없이 음성출력", pPanelTap, ID_TAP_MYBUS_GOTO_NONE, this, (__nTapMybusGoto == 0));
	y += 50;
	__pOptionsMybus[1] = makeStateButtonX(Rectangle(20, y, w-20, 50), "정류소 도착버스로 이동", pPanelTap, ID_TAP_MYBUS_GOTO_STOP, this, (__nTapMybusGoto == 1));
	y += 50;
	__pOptionsMybus[2] = makeStateButtonX(Rectangle(20, y, w-20, 50), "노선도 - 위치정보로 이동", pPanelTap, ID_TAP_MYBUS_GOTO_LINE, this, (__nTapMybusGoto == 2));
	y += 50;

	y += 20;
	makeLabelText(Rectangle(10, y, w-10, 40), "정류소 도착버스 리스트 항목 선택 시", pPanelTap, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 40;
	__pOptionsStoplines[0] = makeStateButtonX(Rectangle(20,  y, w-20, 50), "이동없이 음성출력", pPanelTap, ID_TAP_STOPLINE_GOTO_NONE, this, (__nTapStoplineGoto == 0));
	y += 50;
	//__pOptionsStoplines[1] = makeStateButtonX(Rectangle(20, y, w-20, 50), "정류소 정보로 이동", pPanelTap, ID_TAP_STOPLINE_GOTO_STOP, this, (__nTapStoplineGoto == 1));
	//y += 50;
	__pOptionsStoplines[1] = makeStateButtonX(Rectangle(20, y, w-20, 50), "노선도 - 위치정보로 이동", pPanelTap, ID_TAP_STOPLINE_GOTO_LINE, this, (__nTapStoplineGoto == 1));
	y += 50;


	update();

	return r;
}

result FormAxSetupBus::OnTerminating(void)
{
	return FormAxSetup::OnTerminating();
}

void FormAxSetupBus::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	// 일단 저장 ...
	save();

	switch(actionId)
	{
	case ID_TAP_MYBUS_GOTO_NONE:
	case ID_TAP_MYBUS_GOTO_STOP:
	case ID_TAP_MYBUS_GOTO_LINE:
		__pOptionsMybus[0]->SetSelection(false);
		__pOptionsMybus[1]->SetSelection(false);
		__pOptionsMybus[2]->SetSelection(false);
		__nTapMybusGoto = actionId - ID_TAP_MYBUS_GOTO_NONE;
		__pOptionsMybus[__nTapMybusGoto]->SetSelection(true);
		break;

	case ID_TAP_STOPLINE_GOTO_NONE:
	case ID_TAP_STOPLINE_GOTO_LINE:
	case ID_TAP_STOPLINE_GOTO_STOP:
		__pOptionsStoplines[0]->SetSelection(false);
		__pOptionsStoplines[1]->SetSelection(false);
		//__pOptionsStoplines[2]->SetSelection(false);
		__nTapStoplineGoto = actionId - ID_TAP_STOPLINE_GOTO_NONE;
		__pOptionsStoplines[__nTapStoplineGoto]->SetSelection(true);
		break;
	default:
		FormAxSetup::OnActionPerformed(source, actionId);
		break;
	}

	this->RequestRedraw();
}

void FormAxSetupBus::update()
{
	int n1 = UPDATE_MIN_SCREEN, n2 = UPDATE_MIN_SERVER + 10;

	RegistryGet("UpdateSecond_Screen", n1);
	RegistryGet("UpdateSecond_Server", n2);

	__pSliderScreen->SetValue(n1);
	__pSliderServer->SetValue(n2);

	//int n1 = __pSliderScreen->GetValue();
	//int n2 = __pSliderServer->GetValue();

	AppLogDebug("load registry value (UpdateSecond_Screen) = %d", n1);
	AppLogDebug("load registry value (UpdateSecond_Server) = %d", n2);

	String s1, s2;
	s1.Format(32, L"%2d 초", n1);
	s2.Format(32, L"%2d 초", n2);

	__pLabelTimeScreen->SetText(s1);
	__pLabelTimeServer->SetText(s2);

	this->RequestRedraw();
}

void FormAxSetupBus::init()
{
	// read setting information form AppRegistry and keep them ...
	RegistryGet("TapOnMybus", __nTapMybusGoto);
	RegistryGet("TapOnStopline", __nTapStoplineGoto);

	AppLogDebug("load registry value (TapOnMybus) = %d", __nTapMybusGoto);
	AppLogDebug("load registry value (TapOnStopline) = %d", __nTapStoplineGoto);
}

void FormAxSetupBus::save()
{
	// save the setting information to AppRegistry
	int n1 = __pSliderScreen->GetValue();
	int n2 = __pSliderServer->GetValue();

	AppLogDebug("save registry value (UpdateSecond_Screen) = %d", n1);
	AppLogDebug("save registry value (UpdateSecond_Server) = %d", n2);
	AppLogDebug("save registry value (TapOnMybus) = %d", __nTapMybusGoto);
	AppLogDebug("save registry value (TapOnStopline) = %d", __nTapStoplineGoto);

	RegistrySet("UpdateSecond_Screen", n1);
	RegistrySet("UpdateSecond_Server", n2);

	//
	RegistrySet("TapOnMybus", __nTapMybusGoto);
	RegistrySet("TapOnStopline", __nTapStoplineGoto);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxSetupBus::OnAdjustmentValueChanged(const Osp::Ui::Control& source, int adjustment)
{
	AppLogDebug("adjusted value = %d", adjustment);

	/*
	__sDistanceNow.Format(64, L"%d", adjustment * 100);

	__pLabelRadius->SetText(__sDistanceNow + " m");
	__pLabelRadius->RequestRedraw();

	invokeQuery(true);
	 */

	update();
}

void FormAxSetupBus::OnSliderBarMoved(Osp::Ui::Controls::Slider& source, int value)
{
	AppLogDebug("moved value = %d", value);
}
