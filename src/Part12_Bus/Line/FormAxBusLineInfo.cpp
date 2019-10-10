/*
 * FormAxBusLineInfo.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusLineInfo.h"
#include "FactoryListItem.h"
#include "GroupBusLine.h"
#include "DatabaseBus.h"

FormAxBusLineInfo::FormAxBusLineInfo() : __httpSeoulBus(null), __pArrSpecificLine(null)
{
	__pPanelTop = null;
	__pPanelMidL = null;
	__pPanelMidR = null;
	__pPanelBtm = null;

	__pLabelLineName = null;
	__pLabelLineType = null;
	__pLabelInfo1 = null;
	__pLabelInfo2 = null;
	__pLabelInfo3 = null;
	__pLabelInfo4 = null;
}

FormAxBusLineInfo::~FormAxBusLineInfo()
{
}

bool FormAxBusLineInfo::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("노선정보");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusLineInfo::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	String * pName = GetStringByKey("노선정보_번호이름");

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	// 좌표 계산 식을 만들어 본다.

	// 일단 각 영역을 판넬로 할당한다.
	__pPanelTop = makePanel(Rectangle(0, 0, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png", true);
	int x = 23;
	int y = 80 + 20;

	makeLabelText(Rectangle(x, y, 434, 30), "노선 일반정보", bottomPanel, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += (30 + 10);

	__pPanelMidL = makePanel(Rectangle(x, y, 434/3, 250), bottomPanel, "appendix/bg_434x190_blue.png", true);
	__pPanelMidR = makePanel(Rectangle(x+434/3, y, 434*2/3, 250), bottomPanel, "appendix/bg_434x190_white.png", true);
	y += (250 + 30);

	makeLabelText(Rectangle(x, y, 434, 30), "해당노선에 대한 조작", bottomPanel, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += (30 + 10);

	__pPanelBtm = makePanel(Rectangle(x, y, 434, 90), bottomPanel, "", true);
	y += (60 + 0);

	// 각 판넬을 채운다
	__pLabelLineName = makeLabelText(Rectangle( 10,  0, 290, 90),  *pName, __pPanelTop, 60, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	__pLabelLineType = makeLabelText(Rectangle(300,  0, 124, 90),  "",  __pPanelTop, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);

	int w = 434/3 - 20;
	makeLabelText(Rectangle(10,   0, w, 50), "기      점", __pPanelMidL, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10,  50, w, 50), "종      점", __pPanelMidL, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10, 100, w, 50), "배차간격", __pPanelMidL, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10, 150, w, 50), "운행거리", __pPanelMidL, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);
	makeLabelText(Rectangle(10, 200, w, 50), "운행시간", __pPanelMidL, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_CENTER);

	w = 434*2/3 - 20;
	__pLabelInfo1 = makeLabelText(Rectangle(10,   0, w, 50), "", __pPanelMidR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	__pLabelInfo2 = makeLabelText(Rectangle(10,  50, w, 50), "", __pPanelMidR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	__pLabelInfo3 = makeLabelText(Rectangle(10, 100, w, 50), "", __pPanelMidR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	__pLabelInfo4 = makeLabelText(Rectangle(10, 150, w, 50), "", __pPanelMidR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	__pLabelInfo5 = makeLabelText(Rectangle(10, 200, w, 50), "", __pPanelMidR, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);

	x = 0;
	makeButton(Rectangle(x, 5, 200, 80), "appendix/bg_434x190_blue.png", "",
			"즐겨찾기", __pPanelBtm, ID_DO_1, this);
	x += (200 + 24);
	makeButton(Rectangle(x, 5, 200, 80), "appendix/bg_434x190_blue.png", "",
			"노선정보", __pPanelBtm, ID_DO_2, this);

	invokeQuery();

	return r;
}

result FormAxBusLineInfo::OnTerminating(void)
{
	if (__httpSeoulBus)
		delete __httpSeoulBus;

	//AppLogDebug("deleting bus info which is in __pArrSpecificLine");
	if (__pArrSpecificLine) {
		__pArrSpecificLine->RemoveAll(true);
		delete __pArrSpecificLine;
		__pArrSpecificLine = null;
	}

	return FormAxBus::OnTerminating();
}

void FormAxBusLineInfo::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
	case ID_DO_1:
		AppLogDebug("즐겨찾기");
		saveDb();
		break;
	case ID_DO_2:
		AppLogDebug("노선정보");
		GotoForm(FORM_ID_BUS_LINESTOPS);
		break;
	case ID_DO_3:
		AppLogDebug("지도보기");
		GotoForm(FORM_ID_BUS_LINEMAP);
		break;
	case ID_MESSAGEBOX_CONFIRM:
		break;
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

void FormAxBusLineInfo::invokeQuery()
{
	String * pKey = GetStringByKey("노선정보_검색키");
	if (pKey == null) {
		UtilMessageBox::ConfirmWithCallback("", "노선정보 검색 키 값이 없습니다.", this, ID_MESSAGEBOX_NO_KEYWORD);
		return;
	}
	AppLogDebug("노선 정보 검색합니다. 키 = [%S]", pKey->GetPointer());

	if (__pArrSpecificLine == null) {
		__pArrSpecificLine = new ArrayList();
		__pArrSpecificLine->Construct();
	}

	if (__httpSeoulBus == null) {
		__httpSeoulBus = new HttpBusSeoul(this);
	}

	if (__httpSeoulBus->GetInfoByLine(*pKey))
		commShow();
}

void FormAxBusLineInfo::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	bool bOK = __parser.ParseXml(pBuffer, __pArrSpecificLine);

	commHide();

	if (bOK) {
		updateContent();
	}
	else {
		UtilMessageBox::ConfirmWithCallback("수신 에러", __parser.GetMessage(), this, ID_MESSAGEBOX_NO_DATA);
	}
}

void FormAxBusLineInfo::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();

	AppLogDebug("");
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

void FormAxBusLineInfo::updateContent()
{
	AppAssert(__pArrSpecificLine);

	BusSeoul_LineDetail* pSelectedLine = static_cast<BusSeoul_LineDetail*> (__pArrSpecificLine->GetAt(0));

	if (pSelectedLine == null) {
		AppLogDebug("이럴리가 없는데 ...");
		return;
	}

	String lineType = GroupBusLine::GetBusTypeName(pSelectedLine->__routeType);
	Color color = GroupBusLine::GetBusColor(pSelectedLine->__routeType);
	String term = pSelectedLine->__term;
	String kilo = pSelectedLine->__length;
	term.Append(" 분");
	kilo.Append(" Km");

	String timeSchedule;

	if (pSelectedLine->__firstBusTm.GetLength() == 14 && pSelectedLine->__lastBusTm.GetLength() == 14) { // 경기 버스의 경우 운행시간 정보 없다.
		String time1, time2;
		pSelectedLine->__firstBusTm.SubString(8, time1);
		pSelectedLine->__lastBusTm.SubString(8, time2);

		int nTime1, nTime2;
		Integer::Parse(time1, nTime1);
		Integer::Parse(time2, nTime2);

		timeSchedule.Format(255, L"%02d:%02d ~ %02d:%02d", nTime1/10000, (nTime1%10000)/100, nTime2/10000, (nTime2%10000)/100);
	}

	__pLabelLineName->SetText(pSelectedLine->__busRouteNm);
	__pLabelLineName->SetTextColor(color);
	__pLabelLineType->SetText(lineType);
	__pLabelLineType->SetTextColor(color);

	__pLabelInfo1->SetText(pSelectedLine->__stStationNm);
	__pLabelInfo2->SetText(pSelectedLine->__edStationNm);
	__pLabelInfo3->SetText(term);
	__pLabelInfo4->SetText(kilo);
	__pLabelInfo5->SetText(timeSchedule);



	this->RequestRedraw();
}

void FormAxBusLineInfo::saveDb()
{
	BusSeoul_LineDetail* pline = static_cast<BusSeoul_LineDetail*> (__pArrSpecificLine->GetAt(0));

	if (pline == null) {
		AppLogDebug("이럴리가 없는데 ...");
		return;
	}

	DatabaseBus db;

	bool bOK = db.InsertLine(pline->__busRouteId, pline->__busRouteNm, pline->__routeType, pline->__term, pline->__stStationNm, pline->__edStationNm);

	if (bOK)
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 성공", this, ID_MESSAGEBOX_CONFIRM);
	else
		UtilMessageBox::ConfirmWithCallback("", "노선정보 북마크에 저장 실패", this, ID_MESSAGEBOX_CONFIRM);

}

