/*
 * FormAxBusStop.cpp
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#include "FormAxBusAboutStopOLD.h"

FormAxBusAboutStopOLD::FormAxBusAboutStopOLD()
{
}

FormAxBusAboutStopOLD::~FormAxBusAboutStopOLD()
{
}

bool FormAxBusAboutStopOLD::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("정류소 검색");

	return true;
}

result FormAxBusAboutStopOLD::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.
	int 	nScreenWidth  = dimBottomSize.width;
	int 	nScreenHeight = dimBottomSize.height;  // offset

	int 	nNumCol = 3; 		// 배치할 버튼 (가로) 갯수
	int 	nNumRow = 4; 		// 배치할 버튼 (세로) 갯수
	int 	nBtnWidth  = 100;  	// 버튼의 크기
	int 	nBtnHeight = 100;
	int 	nSpaceX = 45;
	int 	nOffsetY = 44/2;

	int 	nStepX = (nScreenWidth - nSpaceX * 2 - nBtnWidth * nNumCol) / (nNumCol-1) + nBtnWidth;
	int 	nStepY = (nScreenHeight - nBtnHeight * nNumRow) / (nNumRow+1) + nBtnHeight;

	int 	x = nSpaceX;
	int 	y = nStepY - nBtnHeight + nOffsetY;

	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "내 주변",  bottomPanel, ID_STOP_NEAR, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "명칭검색",  bottomPanel, ID_STOP_NAME, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "코드검색",  bottomPanel, ID_STOP_CODE, this);
	x = nSpaceX;
	y += nStepY;

	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "지도",  bottomPanel, ID_STOP_MAP, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "동 이름",  bottomPanel, ID_STOP_DONG, this);
	x += nStepX;
	makeButton(Rectangle(x, y, nBtnWidth, nBtnHeight), "우편번호",  bottomPanel, ID_STOP_ZIP, this);
	x = nSpaceX;
	y += nStepY;



	return r;
}

result FormAxBusAboutStopOLD::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}

void FormAxBusAboutStopOLD::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	int nextFormID = 0;

	switch(actionId)
	{
	case ID_STOP_NEAR:
		nextFormID = FORM_ID_BUS_STOPNEAR;
		AppLogDebug("현재 위치를 파라미터로 만들어 넘김니다...(X/Y = %S/%S)", __currPosLatitude.GetPointer(), __currPosLongitude.GetPointer());
		SetKeyValue("정류소검색_위치_lat", __currPosLatitude);
		SetKeyValue("정류소검색_위치_lng", __currPosLongitude);
		break;
	case ID_STOP_NAME:
		nextFormID = FORM_ID_BUS_STOPSEARCH;
		SetKeyValue("정류소검색방법", 0);
		break;
	case ID_STOP_CODE:
		nextFormID = FORM_ID_BUS_STOPSEARCH;
		SetKeyValue("정류소검색방법", 1);
		break;
	case ID_STOP_MAP:
		nextFormID = FORM_ID_BUS_STOPMAP;
		break;
	case ID_STOP_DONG:
		nextFormID = FORM_ID_BUS_STOPSEARCH;
		SetKeyValue("정류소검색방법", 2);
		break;
	case ID_STOP_ZIP:
		nextFormID = FORM_ID_BUS_STOPSEARCH;
		SetKeyValue("정류소검색방법", 3);
		break;
	default:
		break;
	}

	FormAxBus::OnActionPerformed(source, ((nextFormID > 0) ? nextFormID : actionId));
}

