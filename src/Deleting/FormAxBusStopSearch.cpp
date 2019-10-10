/*
 * FormAxBusStopSearch.cpp
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#include "FormAxBusStopSearch.h"

static Osp::Base::String 	ArrSearchGuides[] = {
		"정류소 이름을 입력하세요"
		, "정류소 코드를 입력하세요"
		, "동 이름을 입력하세요"
		, "우편 번호를 입력하세요"
};


FormAxBusStopSearch::FormAxBusStopSearch() : __pSearchBar(null), __howSearchBusStop(0)
{
	__optionBtnStopName = null;
	__optionBtnStopCode = null;
	__optionBtnDongName = null;
	__optionBtnZipCode = null;
}

FormAxBusStopSearch::~FormAxBusStopSearch()
{
}

bool FormAxBusStopSearch::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER );

	SetTitle("정류소 - 검색");

	SetPush(NO_PUSH);  // navigation history 에 저장하지 않는다. 즉 back 경우 이 화면은 없었던 것처럼 skip 처리된다.

	return true;
}

result FormAxBusStopSearch::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	__howSearchBusStop = GetValueByKey("정류소검색방법");
	AppLogDebug("정류소검색방법 = %d", __howSearchBusStop);

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int 	nScreenWidth  = dimBottomSize.width;
	//int 	nScreenHeight = dimBottomSize.height;  // offset

	int 	x = 10;
	int 	y = 0;

	//
	__pSearchBar = makeSearchBar(Rectangle(0, y, nScreenWidth, 80), bottomPanel, this, this, KEYPAD_ACTION_DONE, ArrSearchGuides[__howSearchBusStop]);
	y += 90;

	//
	makeButton(Rectangle(10, y, nScreenWidth-20, 65), "한국어 음성인식", bottomPanel, ID_SEARCH_WITH_STT, this);
	y += 80;

	//
	__optionBtnStopName = makeStateButton(Rectangle(x, y, nScreenWidth/2-20, 80),
			"common/checkbox_normal.png", "common/checkbox_selected.png", "정류소 이름", bottomPanel, ID_SEARCH_OPTION_STOPNAME, this, (__howSearchBusStop == 0));
	__optionBtnStopCode = makeStateButton(Rectangle(nScreenWidth/2+x, y, nScreenWidth/2-20, 80),
			"common/checkbox_normal.png", "common/checkbox_selected.png", "정류소 코드", bottomPanel, ID_SEARCH_OPTION_STOPCODE, this, (__howSearchBusStop == 1));
	y += 90;

	__optionBtnDongName = makeStateButton(Rectangle(x, y, nScreenWidth/2-20, 80),
			"common/checkbox_normal.png", "common/checkbox_selected.png", "동 이름", bottomPanel, ID_SEARCH_OPTION_DONGNAME, this, (__howSearchBusStop == 2));
	__optionBtnZipCode = makeStateButton(Rectangle(nScreenWidth/2+x, y, nScreenWidth/2-20, 80),
			"common/checkbox_normal.png", "common/checkbox_selected.png", "우편번호", bottomPanel, ID_SEARCH_OPTION_ZIPCODE, this, (__howSearchBusStop == 3));
	y += 110;

	//
	//makeButton(Rectangle(10, y, nScreenWidth-20, 65), "검색", bottomPanel, ID_SEARCH_DO, this);
	//y += 80;

	//
	__pSearchBar->AddActionEventListener(*this);
	__pSearchBar->SetButton("검색", ID_SEARCH_DO);

	return r;
}

result FormAxBusStopSearch::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}

void FormAxBusStopSearch::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_SEARCH_OPTION_STOPNAME:
		__howSearchBusStop = 0;
		__optionBtnStopName->SetSelection(true);
		__optionBtnStopCode->SetSelection(false);
		__optionBtnDongName->SetSelection(false);
		__optionBtnZipCode->SetSelection(false);
		__pSearchBar->SetGuideText(ArrSearchGuides[0]);
		break;
	case ID_SEARCH_OPTION_STOPCODE:
		__howSearchBusStop = 1;
		__optionBtnStopName->SetSelection(false);
		__optionBtnStopCode->SetSelection(true);
		__optionBtnDongName->SetSelection(false);
		__optionBtnZipCode->SetSelection(false);
		__pSearchBar->SetGuideText(ArrSearchGuides[1]);
		break;
	case ID_SEARCH_OPTION_DONGNAME:
		__howSearchBusStop = 2;
		__optionBtnStopName->SetSelection(false);
		__optionBtnStopCode->SetSelection(false);
		__optionBtnDongName->SetSelection(true);
		__optionBtnZipCode->SetSelection(false);
		__pSearchBar->SetGuideText(ArrSearchGuides[2]);
		break;
	case ID_SEARCH_OPTION_ZIPCODE:
		__howSearchBusStop = 3;
		__optionBtnStopName->SetSelection(false);
		__optionBtnStopCode->SetSelection(false);
		__optionBtnDongName->SetSelection(false);
		__optionBtnZipCode->SetSelection(true);
		__pSearchBar->SetGuideText(ArrSearchGuides[3]);
		break;

	case ID_SEARCH_WITH_STT:
	{
		AppLogDebug("음성인식 모듈을 호출합니다.");

		__pSearchBar->SetText("우리집");
		__pSearchBar->SetMode(SEARCH_BAR_MODE_INPUT);
		__pSearchBar->RequestRedraw();
	}
		break;
	case ID_SEARCH_DO:
	{
		String sKeyword = __pSearchBar->GetText();
		sKeyword.Trim();

		if (!sKeyword.IsEmpty()) {
			AppLogDebug("선택 옵션으로 버스 정류소 검색을 시작합니다. - 키워드 = [%S]", sKeyword.GetPointer());

			sKeyword.Replace("-", "");

			SetKeyValue("정류소검색방법", __howSearchBusStop);
			SetKeyValue("정류소검색_키", sKeyword);

			switch (__howSearchBusStop) {
			case 0: // name
				GotoForm(FORM_ID_BUS_STOPNAMES);
				break;
			case 1: // code
				GotoForm(FORM_ID_BUS_STOPLINES);
				break;
			case 2: // dong
				GotoForm(FORM_ID_BUS_STOPADDR2NEAR);
				break;
			case 3: // zip code
				GotoForm(FORM_ID_BUS_STOPZIP2NEAR);
				break;
			}
			return;
		}
	}
		break;
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}

	this->RequestRedraw(true);
}

void FormAxBusStopSearch::OnSearchBarModeChanged(Osp::Ui::Controls::SearchBar& source, Osp::Ui::Controls::SearchBarMode mode)
{
	//AppLogDebug("OnSearchBarModeChanged - mode = %d", mode);

	/***
	Rectangle clientRect;

	if(mode == SEARCH_BAR_MODE_INPUT) {
		clientRect = GetClientAreaBounds();
		SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, false);
		__pSearchBar->SetContentAreaSize(Dimension(clientRect.width, clientRect.height));
	}
	else {
		SetActionBarsVisible(FORM_ACTION_BAR_FOOTER, true);
		clientRect = GetClientAreaBounds();
	}

	Draw();
	Show();
	***/
}

void FormAxBusStopSearch::OnKeypadActionPerformed(Osp::Ui::Control &source, Osp::Ui::KeypadAction keypadAction)
{
	AppLogDebug("OnKeypadActionPerformed");

	__pSearchBar->HideKeypad();

	this->RequestRedraw();
}
