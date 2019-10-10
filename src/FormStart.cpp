#include "FormStart.h"
#include "UtilPopupProgress.h"
#include "UtilApp.h"
#include "AppController.h"

FormStart::FormStart(void) : __pHttpDaumGeoCode(null), __pHttpGoogleWeather(null), __pHttpKmaWeather(null),
	__pTimerWaitGps(null), __pTimerAds(null), __pTimerShowAds(null), __stateGetWeather(READY)
{
	__pPanelWeather = null;
	__pPanelTopMenu = null;
	__pPanelAds = null;

	__pPanelWeatherProgress = null;
	__pPanelWeatherGoogle = null;
	__pPanelWeatherKma = null;

	__nCountShowAds = 0;
	__nCountWaitGPS = 0;

	//
	__pLabelProgress = null;

	__pLabelWeatherIcon = null;
	__pLabelWeatherAddr = null;
	__pLabelWeatherDate = null;
	__pLabelWeatherDegree = null;
	__pLabelWeatherCondition = null;
	__pLabelWeatherWind = null;
	__pLabelWeatherHumidity = null;

	for (int i=0; i<4; i++) {
		__pLabelWeatherForcastWeekDay[i] = null;
		__pLabelWeatherForcastIcon[i] = null;
		__pLabelWeatherForcastDegree[i] = null;
	}
}

FormStart::~FormStart(void)
{
}

bool FormStart::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER );

	SetPanelStype(BOTTOM_PANEL);
	//SetHeaderButton("appendix/refresh.png", "appendix/refresh_ov.png", ID_UPDATE_WEATHER);

	return true;
}

result FormStart::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanelOrg = GetPanel();

	Panel * bottomPanel = makePanel(Rectangle(0, 0, bottomPanelOrg->GetWidth(), bottomPanelOrg->GetHeight()), bottomPanelOrg,
			"appendix/mainbg.png", true);

	int h = 110; //205;
	int y = 0;
	__pPanelWeather = makePanel(Rectangle(0, y, bottomPanel->GetWidth(), h), bottomPanel, "appendix/transparent.png", true);
	y += h;

	__pPanelTopMenu = makePanel(Rectangle(0, y, bottomPanel->GetWidth(), bottomPanel->GetHeight()-y), bottomPanel, "appendix/transparent.png", true);
	__pPanelAds = makePanel(Rectangle(0, bottomPanel->GetHeight()-90, bottomPanel->GetWidth(), 90), bottomPanel, "appendix/transparent.png", true);
	__pPanelAds->SetShowState(false);

	layoutWeather(__pPanelWeather);
	layoutTopMenu(__pPanelTopMenu);

	//
	init();
	shortCut();

	checkupGpsAuthority();

	__pTimerAds = createAds(Rectangle(0, 5, 480, 80), __pPanelAds);
	__pTimerShowAds = CreateTimer(2, 0);

	return r;
}

void FormStart::init()
{
	if (__pHttpDaumGeoCode == null) {
		__pHttpDaumGeoCode = new HttpDaum(this);
	}

	if (__pHttpGoogleWeather == null) {
		__pHttpGoogleWeather = new HttpGoogleWeather(this);
	}

	if (__pHttpKmaWeather == null) {
		__pHttpKmaWeather = new HttpKmaWeather(this);
		__pHttpKmaWeather->Init();
	}

	// upgrade to 1.0.3
	RegistryGet("lastLatitude",  AppBaseForm::__currPosLatitude);
	RegistryGet("lastLongitude", AppBaseForm::__currPosLongitude);

#ifdef _DEBUG_DONE
	AppLogDebug("마지막으로 저장되었던 현재 위치는 (lat/lng = %S/%S)", AppBaseForm::__currPosLatitude.GetPointer(), AppBaseForm::__currPosLongitude.GetPointer());
	UtilMessageBox::ConfirmWithCallback("", "마지막 위치 = " + AppBaseForm::__currPosLatitude + ", " + AppBaseForm::__currPosLongitude,
			this, ID_MESSAGEBOX_CONFIRM);
#endif

	__pTimerWaitGps = CreateTimer(0, 500);
}

void FormStart::shortCut()
{
	int nDo = 0;

	RegistryGet("ShortcutOnStartup", nDo);

	if (nDo == 0) {
		return;
	}

	gotoShortcut();
}

void FormStart::gotoShortcut()
{
	int index = 0;

	RegistryGet("ShortcutIndex", index);
	switch (index) {
	case 0:  // 버스 - 내버스
		GotoForm(FORM_ID_BUS_ABOUT_MYBUS);
		break;
	case 1: // 버스 - 자주쓰는
		GotoForm(FORM_ID_BUS_ABOUT_FREQ);
		break;
	case 2: // 주소 - 북 마크
		GotoForm(FORM_ID_ADDRESS_BOOKMARK);
		break;
	case 3: // 주요 전화
		GotoForm(FORM_ID_PHONES);
		break;
	case 4: // m 서울
		SetKeyValue("웹_title", "모바일 서울");
		SetKeyValue("웹_URL", "http://m.seoul.go.kr");
		//AppBaseForm::OnActionPerformed(source, FORM_ID_WEB);
		GotoForm(FORM_ID_WEB);
		break;
	case 5: // 환율
		GotoForm(FORM_ID_CURRENCY);
		break;
	default:
		return;
	}
}

void FormStart::showWeatherGoogle()
{
	__pLabelWeatherIcon->SetBackgroundImage("Weather" + __pHttpGoogleWeather->__currentIcon);

	__pLabelWeatherAddr->SetText(__pHttpGoogleWeather->__forcastCity);
	__pLabelWeatherDate->SetText(__pHttpGoogleWeather->__forcastDate);
	__pLabelWeatherDegree->SetText(__pHttpGoogleWeather->__currentTempC + "℃");
	__pLabelWeatherCondition->SetText(__pHttpGoogleWeather->__currentCondition);
	__pLabelWeatherWind->SetText(__pHttpGoogleWeather->__currentWind);
	__pLabelWeatherHumidity->SetText(__pHttpGoogleWeather->__currentHumidity);

#if 0
	ArrayList * pArr = __pHttpGoogleWeather->__pArrForcasts;

	for (int i=0; i<4; i++) {
		GoogleWeatherForcast * pForcast = static_cast <GoogleWeatherForcast *> (pArr->GetAt(i));
		__pLabelWeatherForcastWeekDay[i]->SetText(pForcast->__dayOfWeek);
		__pLabelWeatherForcastIcon[i]->SetBackgroundImage("Weather" + pForcast->__icon);
		__pLabelWeatherForcastDegree[i]->SetText(pForcast->__low + "~" + pForcast->__high + "℃");
	}
#endif

	__pPanelWeatherProgress->SetShowState(false);
	__pPanelWeatherGoogle->SetShowState(true);
	//
	__pPanelWeather->RequestRedraw();
}

void FormStart::showWeatherKma()
{
	__pLabelWeatherIcon->SetBackgroundImage("Weather" + __pHttpKmaWeather->__currentIcon);
	__pLabelWeatherAddr->SetText(__pHttpKmaWeather->__forcastCity);
	__pLabelWeatherDate->SetText(__pHttpKmaWeather->__forcastDate);
	__pLabelWeatherDegree->SetText(__pHttpKmaWeather->__currentTempC + "℃");
	__pLabelWeatherCondition->SetText(__pHttpKmaWeather->__currentCondition);
	__pLabelWeatherWind->SetText("");
	__pLabelWeatherHumidity->SetText("");

	//
	__pPanelWeatherProgress->SetShowState(false);
	__pPanelWeatherGoogle->SetShowState(false);
	__pPanelWeatherKma->SetShowState(true);

	__pPanelWeather->RequestRedraw();
}


void FormStart::layoutWeather(Panel * pContainer)
{
	__pPanelWeatherProgress = makePanel(Rectangle(0,0, pContainer->GetWidth(), pContainer->GetHeight()), pContainer);
	__pPanelWeatherGoogle = makePanel(Rectangle(0,0, pContainer->GetWidth(), pContainer->GetHeight()), pContainer);
	__pPanelWeatherKma = makePanel(Rectangle(0,0, pContainer->GetWidth(), pContainer->GetHeight()), pContainer);

	// 일단 progress bar 보이게 한다. 아직 날씨 데이터 없기에 이 부분은 안 보이도록 하고~
	__pPanelWeatherProgress->SetShowState(true);
	__pPanelWeatherGoogle->SetShowState(false);
	__pPanelWeatherKma->SetShowState(false);

	// progress bar panel 내용
	__pProgressWaitGPS = makeProgress(Rectangle(20, 20, pContainer->GetWidth()-40, 20), __pPanelWeatherProgress, 0, 100);
	__pLabelProgress = makeLabelText(Rectangle(10, 40, pContainer->GetWidth()-20, 50), "현재 위치를 확인중입니다...", __pPanelWeatherProgress, 30, Color::COLOR_WHITE);

#if 1 // KMA
	int x = 10;
	int y = 17; // __pPanelWeatherKma->GetHeight()-75/2;

	__pLabelWeatherIcon = makeLabelImage(Rectangle(10, y, 70, 70), "Weather/sunny.png", __pPanelWeatherKma, true);
	x += 75 + 5;

	// 날씨 데이터 표시 내용
	y = 10;
	__pLabelWeatherDegree = makeLabelTextLeft(Rectangle(x, y, 80, 30), "현재온도", __pPanelWeatherKma, 25, Color::COLOR_WHITE);
	y += 30;
	__pLabelWeatherCondition = makeLabelTextLeft(Rectangle(x, y, 80, 55), "현재상태표시", __pPanelWeatherKma, 25, Color::COLOR_WHITE);
	__pLabelWeatherCondition->SetWrap();

	x += 80+5;
	y = 10; // __pPanelWeatherKma->GetHeight()-75/2;
	__pLabelWeatherAddr = makeLabelTextLeft (Rectangle(x, y, __pPanelWeatherKma->GetWidth()/2-50, 55), "현재 위치에 대한 주소를 표시합니다", __pPanelWeatherKma, 25, Color::COLOR_WHITE);
	__pLabelWeatherAddr->SetWrap();
	y += 55+5;
	__pLabelWeatherDate = makeLabelTextLeft(Rectangle(x, y, __pPanelWeatherKma->GetWidth()/2-50, 30), "오늘날짜표시.", __pPanelWeatherKma, 20, Color::COLOR_WHITE);

	x += __pPanelWeatherKma->GetWidth()/2 + 3;
	y = __pPanelWeatherKma->GetHeight() + 100;  // 안 나오게 한다.
	__pLabelWeatherWind = makeLabelTextLeft(Rectangle(x, y, 170, 25), "바람상태표시", __pPanelWeatherKma, 20);
	__pLabelWeatherHumidity = makeLabelTextLeft(Rectangle(x, y, 170, 25), "현재습도표시", __pPanelWeatherKma, 20);

	MyButton * pbtn;
	x = __pPanelWeatherKma->GetWidth() - 80;
	y = 17; // __pPanelWeatherKma->GetHeight()-75/2;
	pbtn = makeButton(Rectangle(x, y, 75, 75), "appendix/kweather.png", "", "",  __pPanelWeatherKma, FORM_ID_WEB, this);
	pbtn->SetSpeechText("방송");

	x = 10;
	y = 10;
	makeButton(Rectangle(x, y, __pPanelWeatherKma->GetWidth() - 80, 100), "appendix/transparent.png", "", "",  __pPanelWeatherKma, ID_SETUP_WEATHER, this);

#else  // Google
	int x = 10;
	int y = 30;

	// 날씨 데이터 표시 내용
	__pLabelWeatherAddr = makeLabelTextLeft(Rectangle(10, 2, __pPanelWeatherGoogle->GetWidth()*2/3, 25), "현재 위치에 대한 주소를 표시합니다", __pPanelWeatherGoogle, 25);
	__pLabelWeatherDate = makeLabelTextRight(Rectangle(__pPanelWeatherGoogle->GetWidth()*2/3+10, 2, __pPanelWeatherGoogle->GetWidth()/3-20, 25), "오늘날짜표시.", __pPanelWeatherGoogle, 25);


	__pLabelWeatherIcon = makeLabelImage(Rectangle(x, y, 75, 75), "Weather/phoneNum", __pPanelWeatherGoogle, true);
	x += 75 + 10;

	__pLabelWeatherDegree = makeLabelTextLeft(Rectangle(x, y, 80, 75), "현재온도", __pPanelWeatherGoogle, 30);
	x += 80;

	__pLabelWeatherCondition = makeLabelTextLeft(Rectangle(x,  y, 170, 25), "현재상태표시", __pPanelWeatherGoogle, 20);
	__pLabelWeatherWind = makeLabelTextLeft(Rectangle(x, 55, 170, 25), "바람상태표시", __pPanelWeatherGoogle, 20);
	__pLabelWeatherHumidity = makeLabelTextLeft(Rectangle(x, 80, 170, 25), "현재습도표시", __pPanelWeatherGoogle, 20);

	makeButton(Rectangle(x+200, y, 75, 75), "방송",  __pPanelWeatherGoogle, FORM_ID_WEB, this);

#if 0
	x = 10;
	y = 110;

	__pLabelWeatherForcastWeekDay[0] = makeLabelText(Rectangle(x, y, 100, 25), "요일1", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastWeekDay[1] = makeLabelText(Rectangle(x, y, 100, 25), "요일2", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastWeekDay[2] = makeLabelText(Rectangle(x, y, 100, 25), "요일3", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastWeekDay[3] = makeLabelText(Rectangle(x, y, 100, 25), "요일4", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);

	x = 10+30;
	y = 135;

	__pLabelWeatherForcastIcon[0] = makeLabelImage(Rectangle(x, y, 40, 40), "Weather/phoneNum", __pPanelWeatherGoogle, true);
	x += 115;
	__pLabelWeatherForcastIcon[1] = makeLabelImage(Rectangle(x, y, 40, 40), "Weather/phoneNum", __pPanelWeatherGoogle, true);
	x += 115;
	__pLabelWeatherForcastIcon[2] = makeLabelImage(Rectangle(x, y, 40, 40), "Weather/phoneNum", __pPanelWeatherGoogle, true);
	x += 115;
	__pLabelWeatherForcastIcon[3] = makeLabelImage(Rectangle(x, y, 40, 40), "Weather/phoneNum", __pPanelWeatherGoogle, true);
	x += 115;

	x = 10;
	y = 175;

	__pLabelWeatherForcastDegree[0] = makeLabelText(Rectangle(x, y, 100, 25), "최소~최대1", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastDegree[1] = makeLabelText(Rectangle(x, y, 100, 25), "최소~최대2", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastDegree[2] = makeLabelText(Rectangle(x, y, 100, 25), "최소~최대3", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
	x += 115;
	__pLabelWeatherForcastDegree[3] = makeLabelText(Rectangle(x, y, 100, 25), "최소~최대4", __pPanelWeatherGoogle, 20, Color::COLOR_BLACK);
#endif
#endif

}

#if 0
void FormStart::layoutTopMenu(Panel * pContainer)
{
	Osp::Graphics::Dimension dimBottomSize = pContainer->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int 	nScreenWidth  = dimBottomSize.width;
	int 	nScreenHeight = dimBottomSize.height;  // offset

	int 	nNumCol = 3; 		// 배치할 버튼 (가로) 갯수
	int 	nNumRow = 4; 		// 배치할 버튼 (세로) 갯수
	int 	nBtnWidth  = 100;  	// 버튼의 크기
	int 	nBtnHeight = 100;
	int 	nSpaceX = 45;
	int 	nOffsetY = 10;

	int 	nStepX = (nScreenWidth - nSpaceX * 2 - nBtnWidth * nNumCol) / (nNumCol-1) + nBtnWidth;
	int 	nStepY = (nScreenHeight - nBtnHeight * nNumRow) / (nNumRow+1) + nBtnHeight;

	int 	x = nSpaceX;
	int 	y = nStepY - nBtnHeight + nOffsetY;

	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_bus.png", "", "버스",  pContainer, FORM_ID_BUS_ABOUT_STOP, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_map.png", "", "지도",  pContainer, FORM_ID_MAP, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_postoffice.png", "", "주소",  pContainer, FORM_ID_ADDRESS, this);
	x = nSpaceX;
	y += nStepY;

	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_yellow.png", "", "주요전화",  pContainer, FORM_ID_PHONES, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_seoul.png", "", "m서울",  pContainer, FORM_ID_SEOUL, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_currency2.png", "", "환율",  pContainer, FORM_ID_CURRENCY, this);
	x = nSpaceX;
	y += nStepY;

	/*
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "세계지도",  pContainer, FORM_ID_WORLD, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "메모",  pContainer, FORM_ID_UTILS, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "도량형환산",  pContainer, FORM_ID_MEASURES, this);
	x = nSpaceX;
	 */
	y += nStepY;

	y += nBtnHeight/3;
	makeButton(Rectangle(x, y, nBtnWidth/2, nBtnHeight*2/3), "appendix/other_config.png", "", "설정",  pContainer, FORM_ID_SETUP_SHORTCUT, this);
	x += nBtnWidth;
	makeButton(Rectangle(x, y, nBtnWidth*2, nBtnHeight*2/3), "appendix/other_go.png", "", "바로가기",  pContainer, FORM_ID_SHORTCUT, this);
	x += nBtnWidth*2 + nBtnWidth/2;
	makeButton(Rectangle(x, y, nBtnWidth/2, nBtnHeight*2/3), "appendix/other_info.png", "", "정보",  pContainer, FORM_ID_INFO, this);
	x = nSpaceX;
	y += nBtnHeight;

	//
	AppLogDebug("화면 배치 완료 후 논리적 높이 = %d (화면 물리적 높이 : %d)", y, dimBottomSize.height);
	//
	if (y > dimBottomSize.height)
		pContainer->SetSize(dimBottomSize.width, y);

}
#else
void FormStart::layoutTopMenu(Panel * pContainer)
{
	Osp::Graphics::Dimension dimBottomSize = pContainer->GetSize();
	//AppLogDebug("menu panel size = (%d,%d)", dimBottomSize.width, dimBottomSize.height);

	int 	x = 0;
	int 	y = 0;
	int 	nBtnWidth  = 480;  	// 버튼의 크기
	int 	nBtnHeight = 82;
	int 	nStepY = 82;

	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_bus.png", "", "버스",  pContainer, FORM_ID_BUS_ABOUT_STOP, this);
	y += nStepY;
	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_map.png", "", "지도",  pContainer, FORM_ID_MAP, this);
	y += nStepY;
	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_postoffice.png", "", "주소",  pContainer, FORM_ID_ADDRESS, this);
	y += nStepY;

	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_yellow.png", "", "주요전화",  pContainer, FORM_ID_PHONES, this);
	y += nStepY;
	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_seoul.png", "", "m서울",  pContainer, FORM_ID_SEOUL, this);
	y += nStepY;
	makeButtonX(Rectangle(x, y, nBtnWidth, nBtnHeight), "appendix/icon_currency.png", "", "환율",  pContainer, FORM_ID_CURRENCY, this);
	y += nStepY;

	MyButton * pbtn;
	x = 10;
	nBtnHeight = 88;
	pbtn = makeButtonImageText(Rectangle(x, y, 88, nBtnHeight), "appendix/icon_option.png", "", "",
			Color(255,255,255,0), pContainer, FORM_ID_SETUP_SHORTCUT, this);
	pbtn->SetSpeechText("설정");
	x += 88 + 10;

	makeLabelText(Rectangle(x+60, y+60, 120, 20), "바 로 가 기", pContainer, 20, Color::COLOR_WHITE);

	pbtn = makeButtonImageText(Rectangle(x+70, y, 110, nBtnHeight-20), "appendix/btn_direct.png", "", "",
			Color(255,255,255,0), pContainer, FORM_ID_SHORTCUT, this);
	pbtn->SetSpeechText("바로가기");
	x += 264;
	pbtn = makeButtonImageText(Rectangle(x, y, 88, nBtnHeight), "appendix/icon_info.png", "", "",
			Color(255,255,255,0), pContainer, FORM_ID_INFO, this);
	pbtn->SetSpeechText("정보");

}
#endif

result FormStart::OnTerminating(void)
{
	// upgrade to 1.0.3
	AppLogDebug("현재 위치를 저장합니다 (lat/lng = %S/%S)", AppBaseForm::__currPosLatitude.GetPointer(), AppBaseForm::__currPosLongitude.GetPointer());
	RegistrySet("lastLatitude",  AppBaseForm::__currPosLatitude);
	RegistrySet("lastLongitude", AppBaseForm::__currPosLongitude);

	if (__pHttpDaumGeoCode) {
		delete __pHttpDaumGeoCode;
	}

	if (__pHttpGoogleWeather) {
		delete __pHttpGoogleWeather;
	}

	if (__pHttpKmaWeather) {
		__pHttpKmaWeather->Term();
		delete __pHttpKmaWeather;
	}

	return AppBaseForm::OnTerminating();
}

void FormStart::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_APPCONTROL_SETTINGS_BAD:
		break;
	case ID_MESSAGEBOX_APPCONTROL_SETTINGS:
    	AppLogDebug("GPS 활성화 설정이 OFF 이기에, APPCONTROL_PROVIDER_SETTINGS 기능을 수행합니다~");
    	UtilApp::AppProviderSetting("Location", this);
		break;
	case ID_UPDATE_WEATHER:
		invokeReverseGeoCoding();
		break;
	case ID_SETUP_WEATHER:
		AppBaseForm::OnActionPerformed(source, FORM_ID_SETUP_WEATHER);
		break;
	case REQUEST_GOTO_BACK:
		AppLog("It does not work at Home Form!");
		break;
	case FORM_ID_WEB:
		SetKeyValue("웹_title", "기상예보 방송");
		SetKeyValue("웹_URL", "http://m.weather.kr");
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	case FORM_ID_SEOUL:
		SetKeyValue("웹_title", "모바일 서울");
		SetKeyValue("웹_URL", "http://m.seoul.go.kr");
		AppBaseForm::OnActionPerformed(source, FORM_ID_WEB);
		break;
	case FORM_ID_WORLD:
		//PopupProgressHelper::Show("당신의 음성명령을 기다리고 있습니다...", this, ID_TEST_PROGRESS_POPUP);
		break;
	case FORM_ID_UTILS:
		//AppLogDebug("STT test");
		//STT();
		break;
	case FORM_ID_MEASURES:
		AppLogDebug("TTS test");
		//TTS("당신의 음성명령을 기다리고 있습니다");
		break;
	case FORM_ID_SHORTCUT:
		gotoShortcut();
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

bool FormStart::AtWakeup()
{
	AppLog("FormStart::AtWakeup()");

	InitIntent();
	invokeReverseGeoCoding();

	return true;
}

void FormStart::AtTimer(HandleTimer * pTimerHandler)
{
	// check for GPS is stable ...
	if (pTimerHandler == __pTimerWaitGps) {
		if (AppBaseForm::__currPosStable == false) {
			AppLogDebug("still waiting for stable GPS signal ...");
			updateProgress();
			return;
		}

		switch (__stateGetWeather) {
		case READY:
			invokeReverseGeoCoding();
			break;
		case QUERYING:
			updateProgress();
			break;
		case DONE:
			DeleteTimer(__pTimerWaitGps);
			__pTimerWaitGps = null;
			break;
		case ERROR:
			AppLogDebug("GeoCoding 서버 에러 ... 재시도 합니다~");
			invokeReverseGeoCoding();
			break;
		default:
			break;
		}
	}
	else {
		if (__pTimerShowAds == pTimerHandler) {
			/*
			bool bShow = (__pPanelAds->GetShowState());
			__pPanelAds->SetShowState(!bShow);
			__pPanelTopMenu->RequestRedraw();
			__pPanelAds->RequestRedraw();
			 */

			bool bStateChanged = false;

			__nCountShowAds++;

			if (__nCountShowAds % 3 == 0) {
				__pPanelAds->SetShowState(true);
				bStateChanged = true;
			}
			else {
				bool bShow = (__pPanelAds->GetShowState());
				if (bShow == true) {
					__pPanelAds->SetShowState(false);
					bStateChanged = true;
				}
			}

			if (bStateChanged == true) {
				__pPanelTopMenu->RequestRedraw();
				__pPanelAds->RequestRedraw();
			}

		}
		AppBaseForm::AtTimer(pTimerHandler);
	}
}

void FormStart::AtLocation()
{
	AppLogDebug("위치 변화가 감지되었습니다.");
	invokeReverseGeoCoding();
}

void FormStart::updateProgress()
{
	__nCountWaitGPS++;
	__pProgressWaitGPS->SetValue((__nCountWaitGPS * 5) % 100);
	__pProgressWaitGPS->RequestRedraw();
}

bool FormStart::invokeReverseGeoCoding()
{
	__stateGetWeather = QUERYING;

	__pLabelProgress->SetText("현재 위치 정보를 가져오고 있습니다.");
	__pLabelProgress->RequestRedraw();

	bool bOK = __pHttpDaumGeoCode->GetReverse(__currPosLatitude, __currPosLongitude);

	if (__pTimerWaitGps) {
		__pTimerWaitGps->Stop();
	}

	return bOK;
}

bool FormStart::invokeWeather()
{
	__stateGetWeather = QUERYING;

	__pLabelProgress->SetText("현재 위치의 날씨 정보를 가져오고 있습니다.");
	__pLabelProgress->RequestRedraw();

	bool bOK = false;

#if 0
	bOK = __pHttpGoogleWeather->GetWeather(__pHttpDaumGeoCode->__nameFormatted);
#else
	bOK = __pHttpKmaWeather->GetWeather(__pHttpDaumGeoCode->__nameFormatted);
#endif

	return bOK;
}

void FormStart::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	if (pHttp == __pHttpDaumGeoCode->GetHandler()) {
		if (onHttpDaumGeoCode(pBuffer)) {
			//__pLabelWeatherAddr->SetText(__pHttpGoogleWeather->__forcastCity);
			//__pLabelWeatherDate->SetText(__pHttpGoogleWeather->__forcastDate);

			__pLabelWeatherAddr->SetText(__pHttpDaumGeoCode->__nameFormatted);
			__pLabelWeatherDate->SetText(UtilApp::GetNowDate());
			__pLabelWeatherAddr->RequestRedraw();
			__pLabelWeatherDate->RequestRedraw();

			invokeWeather();
		}
		else {
			__pLabelProgress->SetText("현재 위치 정보를 가져오지 못하였습니다. 재시도 바랍니다.");
			__pLabelProgress->RequestRedraw();
		}
	}
	else if (pHttp == __pHttpGoogleWeather->GetHandler()) {
		//
		__stateGetWeather = DONE;

		if (__pTimerWaitGps) {
			__pTimerWaitGps->Start(500);
		}

		if (__pHttpGoogleWeather->ParseWeather(pBuffer))
			showWeatherGoogle();
		else {
			//invokeReverseGeoCoding();
			//invokeWeather();
		}

	}
	else if (pHttp == __pHttpKmaWeather->GetHandler()) {
		__stateGetWeather = DONE;

		if (__pTimerWaitGps) {
			__pTimerWaitGps->Start(500);
		}

		if (__pHttpKmaWeather->ParseWeather(pBuffer))
			showWeatherKma();
		else
			showWeatherKma();

	}
}

void FormStart::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	__stateGetWeather = ERROR;

	__pLabelProgress->SetText(message);
	__pLabelProgress->RequestRedraw();
}

bool FormStart::onHttpDaumGeoCode(ByteBuffer* pBuffer)
{
	__pHttpDaumGeoCode->ParseReverse(pBuffer, null);

	AppLogDebug("위치정보 (다음 Reverse GEO coding API) : %S, %S, %S - %S [%S]",
			__pHttpDaumGeoCode->__name1.GetPointer(),
			__pHttpDaumGeoCode->__name2.GetPointer(),
			__pHttpDaumGeoCode->__name3.GetPointer(),
			__pHttpDaumGeoCode->__nameCode.GetPointer(),
			__pHttpDaumGeoCode->__nameFormatted.GetPointer()
			 );

	return !(__pHttpDaumGeoCode->__nameFormatted.IsEmpty());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormStart::startLocationService()
{
	AppController * pAppC = (AppController*) __pNaviagationController;
	pAppC->StartLocationService();
}

void FormStart::checkupGpsAuthority()
{
    String key("GPSEnabled");
    bool gpsEnabled = false;

    SettingInfo::GetValue(key, gpsEnabled);

    if (gpsEnabled == true) {
    	AppLogDebug("GPS 활성화 설정이 ON 이기에, GPS 수신 기능 시작합니다.");
    	startLocationService();
    }
    else {
    	AppLogDebug("GPS 활성화 설정이 OFF 이기에, APPCONTROL_PROVIDER_SETTINGS 기능을 수행합니다~");
		UtilMessageBox::ConfirmWithCallback("", "GPS 활성화 설정이 필요합니다. 설정에서 Appendix (부록)의 GPS 기능을 켜 주세요.",
				this, ID_MESSAGEBOX_APPCONTROL_SETTINGS);
    }
}

void FormStart::OnAppControlCompleted(const Osp::Base::String& appControlId, const Osp::Base::String& operationId, const Osp::Base::Collection::IList* pResultList)
{
	if (appControlId.Equals(APPCONTROL_PROVIDER_SETTINGS)) {
		Osp::Base::String* pStrResult = (Osp::Base::String*)pResultList->GetAt(0);
		if (pStrResult->Equals(APPCONTROL_RESULT_SUCCEEDED)) {
			pStrResult = (Osp::Base::String*)pResultList->GetAt(1);

			if (pStrResult->Equals(String(L"category:Location"))){
				pStrResult = (Osp::Base::String*)pResultList->GetAt(2);

				if (pStrResult->Equals(String(L"GPSEnabled"))){

			    	AppLogDebug("GPS 활성화 설정을 ON 했기에, GPS 수신 기능 시작합니다.");
			    	startLocationService();

				}else{
			    	AppLogDebug("GPS 활성화가 반드시 필요합니다.");
					UtilMessageBox::ConfirmWithCallback("", "GPS 활성화 설정 안 하신 관계로 Appendix (부록)에서 현재위치 관련 기능이 정상적으로 동작하지 않습니다.",
							this, ID_MESSAGEBOX_APPCONTROL_SETTINGS_BAD);
				}
			}
		}

	}

//    String key("GPSEnabled");
//    bool gpsEnabled = false;
//
//    SettingInfo::GetValue(key, gpsEnabled);
//
//    if (gpsEnabled == true) {
//    	AppLogDebug("GPS 활성화 설정을 ON 했기에, GPS 수신 기능 시작합니다.");
//    	start_locationDetection();
//    }
//    else {
//		//UtilMessageBox::ConfirmWithCallback("", "GPS 활성화가 반드시 필요합니다.", this, ID_MESSAGEBOX_APPCONTROL_SETTINGS);
//		//UtilMessageBox::ConfirmWithCallback(L"", L"GPS 활성화가 반드시 필요합니다.", this, 0);
//    	AppLogDebug("GPS 활성화가 반드시 필요합니다.");
//    }

}
