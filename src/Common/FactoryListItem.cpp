/*
 * FactoryListItem.cpp
 *
 *  Created on: 2012. 7. 10.
 *      Author: benny
 */

#include "FactoryListItem.h"
#include "GroupBusLine.h"
#include "UtilApp.h"

FactoryListItem::FactoryListItem()
{
}

FactoryListItem::~FactoryListItem()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BLabel * FactoryListItem::makeBLabel(Rectangle rect, const String & showText, int textSize, const Color& textColor, const Color& backgroundColor, HorizontalAlignment horizonAlign, VerticalAlignment verticalAlign, LabelTextStyle textStyle, TextWrap wrap)
{
	BLabel * pLabel = new BLabel();
	pLabel->Construct(rect, showText);

	pLabel->SetTextConfig(textSize, textStyle);  // LABEL_TEXT_STYLE_NORMAL
	pLabel->SetTextColor(textColor);
	pLabel->SetBackgroundColor(backgroundColor);
	pLabel->SetTextHorizontalAlignment(horizonAlign);
	pLabel->SetTextVerticalAlignment(verticalAlign);

	pLabel->SetWrap(wrap);

	return pLabel;
}

bool FactoryListItem::attachRichText(CustomItem* pTo, int nTo, Rectangle rect, const String & text, int sizeText, const Color& textColor, const Color& backgroundColor, HorizontalAlignment horizonAlign, VerticalAlignment verticalAlign, LabelTextStyle textStyle, TextWrap wrap)
{
	BLabel * pLabel = makeBLabel(Rectangle(0, 0, rect.width, rect.height), text, sizeText, textColor, backgroundColor, horizonAlign, verticalAlign, textStyle, wrap);

	if (pLabel == null || pLabel->GetRichText() == null) {
		AppLogDebug("attachRichText 실패!");
		return false;
	}

	EnrichedText * pRich = pLabel->GetRichText();
	pTo->AddElement(rect, nTo, *pRich);

	delete pLabel;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_String(int index, int itemWidth, const String & sText, ListAnnexStyle style)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 80), style);

	int itemIndex = 0;
	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 80), sText, 30);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_String2(int index, int itemWidth, const String & sHead, const String & sText, ListAnnexStyle style)
{
	int h = 60;

	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, h), style);

	Bitmap * pBmpBlue = UtilApp::GetBitmap2N("appendix/bg_434x190_blue.png");
	Bitmap * pBmpWhite = UtilApp::GetBitmap2N("appendix/bg_434x190_white.png");

	int itemIndex = 0;

	pItem->AddElement(Rectangle(0,0, itemWidth/2-50, h), itemIndex++, *pBmpBlue);
	delete pBmpBlue;
	pItem->AddElement(Rectangle(itemWidth/2-50,0, itemWidth/2+50, h), itemIndex++, *pBmpWhite);
	delete pBmpWhite;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth/2-10, h), sHead, 25, Color::COLOR_WHITE, Color(0,0,0,0), ALIGNMENT_LEFT);
	attachRichText(pItem, itemIndex++, Rectangle( itemWidth/2,  0, itemWidth/2-100, h), sText, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);

	return pItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스 노선 관련
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_BusLine(int index, int itemWidth,  BusSeoul_Line* pinfo, bool bShowType)
{
	CustomItem * pItem = new CustomItem();

	pItem->Construct(Dimension(itemWidth, (pinfo->__cdate.IsEmpty() ? 80 : 110)), LIST_ANNEX_STYLE_NORMAL);

	String lineInfo;
	String lineType = GroupBusLine::GetBusTypeName(pinfo->__routeType);
	Color color = GroupBusLine::GetBusColor(pinfo->__routeType);

	lineInfo.Format(511, L"배차 %S분 (%S - %S)", pinfo->__term.GetPointer(), pinfo->__stStationNm.GetPointer(), pinfo->__edStationNm.GetPointer());

	// CustomItem 에 AddElement() 하는 아래와 같은 방법이 일반적이다. 하지만 ... not beautiful ~
	//pItem->AddElement(Rectangle( 10, 10, itemWidth-10, 30), 0, pinfo->__busRouteNm, true);
	//pItem->AddElement(Rectangle( 30, 50, itemWidth-40, 30), 1, lineInfo, true);

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle(10,  0, itemWidth/2-20, 50), pinfo->__busRouteNm, 	30, color);
	if (bShowType) {
		attachRichText(pItem, itemIndex++, Rectangle(itemWidth/2+10, 10, itemWidth/2-20, 40), lineType, 30, color, Color(0,0,0,0), ALIGNMENT_RIGHT);
	}

	attachRichText(pItem, itemIndex++, Rectangle(50, 40, itemWidth-60, 40), lineInfo,  		25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);

	if (!pinfo->__cdate.IsEmpty()) {
		attachRichText(pItem, itemIndex++, Rectangle(50, 80, itemWidth-60, 30), pinfo->__cdate,  	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	}

	return pItem;
}

#if 0
// 세로 배치
CustomItem * FactoryListItem::makeListItem_BusStopByLine(int index, int itemWidth,  BusSeoul_StopByLine* pinfo, Bitmap * pbmp)
{
	int height = 70;
	if (index > 0) {
		height = 100;
	}

	height += pinfo->__pArrPositionInfo->GetCount() * 100;  //

	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, height), LIST_ANNEX_STYLE_NORMAL);

	String sStopName, sStopCode, sDistance;
	String sCode = pinfo->__stationNo;
	if (sCode.GetLength() > 2)
		sCode.Insert('-', 2);

	sDistance.Format(255, L"%S m", pinfo->__fullSectDist.GetPointer());  // 정류소간 거리
	sStopName.Format(255, L"[%S] %S", pinfo->__seq.GetPointer(), pinfo->__stationNm.GetPointer());
	sStopCode.Format(255, L"[%S]", sCode.GetPointer());
	//sStopCode.Format(255, L"%S / 운행시간 : %S ~ %S", sCode.GetPointer(), pinfo->__beginTm.GetPointer(), pinfo->__lastTm.GetPointer());


	int posY = 0;
	int indexItem = 0;

	if (index > 0) {
		attachRichText(pItem, indexItem++, Rectangle( 10,  0, itemWidth-50, 30), sDistance, 	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
		posY =  30;
	}

	attachRichText(pItem, indexItem++, Rectangle( 10, posY, itemWidth-100, 40), sStopName, 	30, Color::COLOR_RED);
	posY += 40;

	attachRichText(pItem, indexItem++, Rectangle( 10, posY, itemWidth/2-20, 30), sStopCode, 	25);
	posY += 30;

	if (pbmp) {
		pItem->AddElement(Rectangle(itemWidth-40, 0, 30, height), indexItem++, *pbmp);
	}

	for (int i=0; i<pinfo->__pArrPositionInfo->GetCount(); i++) {
		BusSeoul_BusPosition * pCurrPos = static_cast<BusSeoul_BusPosition*>(pinfo->__pArrPositionInfo->GetAt(i));

		String runningBus = pCurrPos->__plainNo;

		float distSect, distRoute;
		Float::Parse(pCurrPos->__sectDist, distSect);
		Float::Parse(pCurrPos->__fullSectDist, distRoute);
		int y = (int) ((float) height * distSect / distRoute);

		if (y > height-26) {
			y = height-26;
		}

		attachRichText(pItem, indexItem++, Rectangle( itemWidth/2-(50*i), y, itemWidth/2-30, 25), runningBus, 	25, Color::COLOR_BLUE);
	}

	return pItem;
}
#endif

// 횡 - 가로 배치
CustomItem * FactoryListItem::makeListItem_BusStopByLine(int index, int itemWidth,  BusSeoul_StopByLine* pinfo, Bitmap * pbmp, Bitmap * pbmpBus)
{
	int height = 70;

	height += pinfo->__pArrPositionInfo->GetCount() * 30;

	if (index > 0) {
		height += 30;
	}

	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, height), LIST_ANNEX_STYLE_NORMAL);

	String sStopName, sStopCode, sDistance;
	String sCode = pinfo->__stationNo;
	if (sCode.GetLength() > 2)
		sCode.Insert('-', 2);

	sDistance.Format(255, L"%S m", pinfo->__fullSectDist.GetPointer());  // 정류소간 거리
	sStopName.Format(255, L"[%S] %S", pinfo->__seq.GetPointer(), pinfo->__stationNm.GetPointer());
	sStopCode.Format(255, L"[%S]", sCode.GetPointer());
	//sStopCode.Format(255, L"%S / 운행시간 : %S ~ %S", sCode.GetPointer(), pinfo->__beginTm.GetPointer(), pinfo->__lastTm.GetPointer());

	int posY = 0;
	int indexItem = 0;

	for (int i=0; i<pinfo->__pArrPositionInfo->GetCount(); i++) {
		BusSeoul_BusPosition * pCurrPos = static_cast<BusSeoul_BusPosition*>(pinfo->__pArrPositionInfo->GetAt(i));

		String runningBus = pCurrPos->__plainNo;
		String statusBus;

		if (pCurrPos->__busType == "1") { // 저상버스
			runningBus.Append("(저상)");
		}
		else if (pCurrPos->__busType == "2") { // 굴절버스
			runningBus.Append("(굴절)");
		}

		float distSect, distRoute;
		Float::Parse(pCurrPos->__sectDist, distSect);
		Float::Parse(pCurrPos->__fullSectDist, distRoute);
		int width = itemWidth/2-70;
		int x = (int) ((float) width * distSect / distRoute) + itemWidth/4+10;

		statusBus.Format(128, L"%.1fm전", distRoute-distSect);
		if (pCurrPos->__stopFlag == "1") {
			statusBus.Append(" 정차 ");
		}

		pItem->AddElement(Rectangle( x, posY, 70, 20), indexItem++, *pbmpBus);

		//posY += 30;

		attachRichText(pItem, indexItem++, Rectangle(5, posY, itemWidth/4+40, 25), runningBus, 20, Color::COLOR_BLUE);
		attachRichText(pItem, indexItem++, Rectangle(itemWidth*3/4-10, posY, itemWidth/4+10, 25), statusBus, 20, Color::COLOR_BLUE, Color(0,0,0,0), ALIGNMENT_RIGHT);
		posY += 30;
	}

	if (index > 0) {
		pItem->AddElement(Rectangle( 50, posY+13, 130, 3), indexItem++, *pbmp);
		pItem->AddElement(Rectangle(300, posY+13, 130, 3), indexItem++, *pbmp);
		String sIndexPrev, sIndexNow;
		sIndexPrev.Format(16, L"[%2d]", index);
		sIndexNow.Format(16, L"[%2d]", index+1);

		attachRichText(pItem, indexItem++, Rectangle(           10,  posY,            60, 30), sIndexPrev, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
		attachRichText(pItem, indexItem++, Rectangle(           70,  posY, itemWidth-140, 30), sDistance,  25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_CENTER);
		attachRichText(pItem, indexItem++, Rectangle( itemWidth-70,  posY,            60, 30), sIndexNow,  25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
		posY += 30;
	}

	attachRichText(pItem, indexItem++, Rectangle( 10, posY, itemWidth-100, 40), sStopName, 	30, Color::COLOR_RED);
	posY += 40;

	attachRichText(pItem, indexItem++, Rectangle( 10, posY, itemWidth/2-20, 30), sStopCode, 	25);
	posY += 30;

	return pItem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스 정류소 관련
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_StopArriveInfo(int index, int itemWidth, BusSeoul_ArrivingInfo * pArv, Bitmap * pbmpBus)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 120), LIST_ANNEX_STYLE_NORMAL);

	Color colorLine = GroupBusLine::GetBusColor(pArv->__routeType);
	String sInterval = "배차:";
	sInterval.Append(pArv->__term);
	sInterval.Append("분");

	int nTime1, nTime2, nOrd1, nOrd2, nOrd, nArrived1, nArrived2;
	Integer::Parse(pArv->__sectOrd1, nOrd1);
	Integer::Parse(pArv->__sectOrd2, nOrd2);
	Integer::Parse(pArv->__staOrd, nOrd);
	Integer::Parse(pArv->__isArrive1, nArrived1);
	Integer::Parse(pArv->__isArrive2, nArrived2);

	//
	TimeSpan tmSpan1 = pArv->__datetime1.GetTime();
	TimeSpan tmSpan2 = pArv->__datetime2.GetTime();
	TimeSpan tmSpanNow = UtilApp::GetNow().GetTime();

	tmSpan1 = tmSpan1 - tmSpanNow;
	tmSpan2 = tmSpan2 - tmSpanNow;

	nTime1 = tmSpan1.GetTicks() / 1000;
	nTime2 = tmSpan2.GetTicks() / 1000;

	//AppLogDebug("%S - 남은 시간[1]: %d/%S, [2]: %d/%S",
	//		pArv->__rtNm.GetPointer(), nTime1, pArv->__traTime1.GetPointer(), nTime2, pArv->__traTime2.GetPointer());

	String sInfo1, sInfo2, sInfo11, sInfo21;
	int nInfo1Size = 25;

	if (nTime1 <= 0) {
		sInfo1.Format(255, L"곧 도착하거나 이미 지나갔습니다");
		nInfo1Size = 22;
	}
	else if (nTime1 < 60) {
		sInfo1.Format(255, L"     %2d초 후/ (%d)번째전 ", nTime1%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd1));
		if (nArrived1 == 1)
			sInfo1.Append("도착");
		else
			sInfo1.Append("출발");
	}
	else {
		sInfo1.Format(255, L"%2d분 %2d초 후/ (%d)번째전 ", nTime1/60, nTime1%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd1));
		if (nArrived1 == 1)
			sInfo1.Append("도착");
		else
			sInfo1.Append("출발");
	}
	//

	sInfo2.Format(255, L"%2d분 %2d초 후/ (%d)번째전 ", nTime2/60, nTime2%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd2));
	if (nArrived2 == 1)
		sInfo2.Append("도착");
	else
		sInfo2.Append("출발");

	String sBusStat1, sBusStat2;
	//////////////////////////////////////////////////////////////////
	// re-calculate
	if (pArv->__stationNm1.IsEmpty() || pArv->__vehId1 == "0") {
		sInfo1.Format(255, L"도착예정 버스 없음");
		pArv->__plainNo1 = " ";
	}
	else {
		sInfo11.Format(255, L" (%SKm/h,%S)", pArv->__traSpd1.GetPointer(), pArv->__stationNm1.GetPointer());

		if (pArv->__busType1 == "1") {
			sBusStat1 = "저상";
		}
		else if (pArv->__busType1 == "2") {
			sBusStat1 = "굴절";
		}
		if (pArv->__isLast1 == "1") {
			sBusStat1.Append("(막차)");
		}
	}

	if (pArv->__stationNm2.IsEmpty() || pArv->__vehId2 == "0") {
		sInfo2 = " ";
		pArv->__plainNo2 = " ";
	}
	else {
		sInfo21.Format(255, L" (%SKm/h,%S)", pArv->__traSpd2.GetPointer(), pArv->__stationNm2.GetPointer());

		if (pArv->__busType2 == "1") {
			sBusStat2 = "저상";
		}
		else if (pArv->__busType2 == "2") {
			sBusStat2 = "굴절";
		}
		if (pArv->__isLast2 == "1") {
			sBusStat2.Append("(막차)");
		}
	}

	//
	int count = 0;
	attachRichText(pItem, count++, Rectangle(  5,   0, 180, 60), pArv->__rtNm, 	40, colorLine, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_BOLD);
	attachRichText(pItem, count++, Rectangle(  5,  60,  90, 25), sInterval, 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	//attachRichText(pItem, count++, Rectangle(  0,  85, 90, 25), sInterval, 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	pItem->AddElement(Rectangle( 10, 85, 70, 20), count++, *pbmpBus);

	if (!sBusStat1.IsEmpty()) {
		attachRichText(pItem, count++, Rectangle(100,  0, 85, 40), sBusStat1, 	20, Color::COLOR_BLUE, Color(0,0,0,0), ALIGNMENT_CENTER);
	}

	attachRichText(pItem, count++, Rectangle(100, 40, 85, 20), pArv->__plainNo1, 	16, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200,  0, itemWidth-205, 30), sInfo1, 	nInfo1Size, Color::COLOR_RED,	Color(0,0,0,0));
	attachRichText(pItem, count++, Rectangle(200, 30, itemWidth-205, 30), sInfo11, 	22, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);

	if (!sBusStat2.IsEmpty()) {
		attachRichText(pItem, count++, Rectangle(100, 60, 85, 40), sBusStat2, 	20, Color::COLOR_BLUE, Color(0,0,0,0), ALIGNMENT_CENTER);
	}
	attachRichText(pItem, count++, Rectangle(100,100, 85, 20), pArv->__plainNo2, 	16, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200, 60, itemWidth-205, 30), sInfo2, 	22, Color::COLOR_RED, 	Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200, 90, itemWidth-205, 30), sInfo21, 	20, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_StopLinesInfo(int index, int itemWidth, BusSeoul_LineDetail * pinfo)  // 정류소 경유 노선 정보
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 120), LIST_ANNEX_STYLE_NORMAL);

	String sInterval = "배차:";
	sInterval.Append(pinfo->__term);
	sInterval.Append("분");
	Color colorLine = GroupBusLine::GetBusColor(pinfo->__routeType);

	String sDate1Y, sDate1M, sDate1D, sDate2Y, sDate2M, sDate2D;
	String sTime1H, sTime1M, sTime1S, sTime2H, sTime2M, sTime2S;

	if (pinfo->__firstBusTm.GetLength() > 12) {
		pinfo->__firstBusTm.SubString( 0, 4, sDate1Y);
		pinfo->__firstBusTm.SubString( 4, 2, sDate1M);
		pinfo->__firstBusTm.SubString( 6, 2, sDate1D);
		pinfo->__firstBusTm.SubString( 8, 2, sTime1H);
		pinfo->__firstBusTm.SubString(10, 2, sTime1M);
		pinfo->__firstBusTm.SubString(12, 2, sTime1S);
	}

	if (pinfo->__lastBusTm.GetLength() > 12) {
		pinfo->__lastBusTm.SubString( 0, 4, sDate2Y);
		pinfo->__lastBusTm.SubString( 4, 2, sDate2M);
		pinfo->__lastBusTm.SubString( 6, 2, sDate2D);
		pinfo->__lastBusTm.SubString( 8, 2, sTime2H);
		pinfo->__lastBusTm.SubString(10, 2, sTime2M);
		pinfo->__lastBusTm.SubString(12, 2, sTime2S);
	}

	String sTm1, sTm2;
	sTm1.Format(32, L"첫차 %S:%S", sTime1H.GetPointer(), sTime1M.GetPointer());
	sTm2.Format(32, L"막차 %S:%S", sTime2H.GetPointer(), sTime2M.GetPointer());

	//Color colorTime = Color(200, 200, 200);
	Color colorTime = Color::COLOR_WHITE;
	String sTime1, sTime2;

	sTime1 = "현재 정류소의 첫차/막차 정보를";
	sTime2 = " 확인하려면 이곳을 누르세요~";

	if (!pinfo->__firstBusTmCurr.IsEmpty()) {
		pinfo->__firstBusTmCurr.SubString( 8, 2, sTime1H);
		pinfo->__firstBusTmCurr.SubString(10, 2, sTime1M);
		sTime1.Format(255, L"현재 정류소 오늘의 첫차 %S:%S", sTime1H.GetPointer(), sTime1M.GetPointer());
		colorTime = Color::COLOR_RED;
	}
	if (!pinfo->__lastBusTmCurr.IsEmpty()) {
		pinfo->__lastBusTmCurr.SubString( 0, 4, sDate2Y);
		pinfo->__lastBusTmCurr.SubString( 4, 2, sDate2M);
		pinfo->__lastBusTmCurr.SubString( 6, 2, sDate2D);
		pinfo->__lastBusTmCurr.SubString( 8, 2, sTime2H);
		pinfo->__lastBusTmCurr.SubString(10, 2, sTime2M);
		pinfo->__lastBusTmCurr.SubString(12, 2, sTime2S);
		sTime2.Format(255, L"막차는 %S월%S일 %S:%S", sDate2M.GetPointer(), sDate2D.GetPointer(), sTime2H.GetPointer(), sTime2M.GetPointer());
		colorTime = Color::COLOR_RED;
	}

	//
	int count = 0;
	attachRichText(pItem, count++, Rectangle(  5,   0, 180, 60), pinfo->__busRouteNm, 	40, colorLine, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_BOLD);
	attachRichText(pItem, count++, Rectangle(  5,  60,  90, 20), sInterval, 20, Color::COLOR_BLACK, Color(0,0,0,0));
	attachRichText(pItem, count++, Rectangle(  5,  80,  90, 20), sTm1, 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	attachRichText(pItem, count++, Rectangle(  5, 100,  90, 20), sTm2, 	20, Color::COLOR_BLACK, Color(0,0,0,0));

	//
	attachRichText(pItem, count++, Rectangle(110,  0, itemWidth-120, 30), sTime1,  25, colorTime, Color(0,0,0,0));
	attachRichText(pItem, count++, Rectangle(110, 30, itemWidth-120, 30), sTime2,  25, colorTime, Color(0,0,0,0));

	attachRichText(pItem, count++, Rectangle(110, 60, itemWidth-120, 30), "기점 : " + pinfo->__stStationNm, 22, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);
	attachRichText(pItem, count++, Rectangle(110, 90, itemWidth-120, 30), "종점 : " + pinfo->__edStationNm, 22, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_StopNear(int index, int itemWidth,  BusSeoul_StopInfo* pinfo, bool showDistance)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 80), LIST_ANNEX_STYLE_NORMAL);

	String sCode = pinfo->__arsId;
	if (sCode.GetLength() > 2)
		sCode.Insert('-', 2);

	String sAuxInfo;
	if (showDistance) {
		sAuxInfo.Format(128, L"[%S]              %.1f (m)", sCode.GetPointer(), pinfo->__distance * 7.5);
	}
	else {
		sAuxInfo.Format(128, L"[%S]", sCode.GetPointer());
	}

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,   0, itemWidth-20, 40), pinfo->__stationNm, 	30, Color::COLOR_RED);
	attachRichText(pItem, itemIndex++, Rectangle( 50,  40, itemWidth-70, 40), sAuxInfo, 30);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_StopInfo(int index, int itemWidth,  BusSeoul_StopInfo* pinfo)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 80), LIST_ANNEX_STYLE_NORMAL);

	String sCode = pinfo->__arsId;
	if (sCode.GetLength() > 2)
		sCode.Insert('-', 2);

	String sAuxInfo;
	if (pinfo->__cdate.IsEmpty()) {
		sAuxInfo.Format(128, L"[%S]", sCode.GetPointer());
	}
	else {
		sAuxInfo.Format(128, L"[%S]      %S", sCode.GetPointer(), pinfo->__cdate.GetPointer());
	}

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 40), pinfo->__stationNm, 	30, Color::COLOR_RED);
	attachRichText(pItem, itemIndex++, Rectangle( 50, 40, itemWidth-50, 40), sAuxInfo, 30);

	return pItem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스도착정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_LineArriveInfo(int index, int itemWidth, BusSeoul_ArrivingByLine * pArv, Bitmap * pbmpBus)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 120), LIST_ANNEX_STYLE_NORMAL);

	Color colorLine = GroupBusLine::GetBusColor(pArv->__routeType);
	String sInterval = "배차:";
	sInterval.Append(pArv->__term);
	sInterval.Append("분");

	int nTime1, nTime2, nOrd1, nOrd2, nOrd, nArrived1, nArrived2;
	//Integer::Parse(pArv->__traTime1, nTime1);
	//Integer::Parse(pArv->__traTime2, nTime2);
	Integer::Parse(pArv->__sectOrd1, nOrd1);
	Integer::Parse(pArv->__sectOrd2, nOrd2);
	Integer::Parse(pArv->__staOrd, nOrd);
	Integer::Parse(pArv->__isArrive1, nArrived1);
	Integer::Parse(pArv->__isArrive2, nArrived2);

	//
	//AppLogDebug("[%S] arriving time1 = %S", pArv->__rtNm.GetPointer(), pArv->__datetime1.ToString().GetPointer());
	//AppLogDebug("[%S] arriving time2 = %S", pArv->__rtNm.GetPointer(), pArv->__datetime2.ToString().GetPointer());
	TimeSpan tmSpan1 = pArv->__datetime1.GetTime();
	TimeSpan tmSpan2 = pArv->__datetime2.GetTime();
	TimeSpan tmSpanNow = UtilApp::GetNow().GetTime();

	tmSpan1 = tmSpan1 - tmSpanNow;
	tmSpan2 = tmSpan2 - tmSpanNow;

	nTime1 = tmSpan1.GetTicks() / 1000;
	nTime2 = tmSpan2.GetTicks() / 1000;

	//AppLogDebug("%S - 남은 시간[1]: %d/%S, [2]: %d/%S",
	//		pArv->__rtNm.GetPointer(), nTime1, pArv->__traTime1.GetPointer(), nTime2, pArv->__traTime2.GetPointer());

	String sInfo1, sInfo2, sInfo11, sInfo21;
	int nInfo1Size = 25;

	if (nTime1 < 0) {
		sInfo1.Format(255, L"곧 도착하거나 이미 지나갔습니다");
		nInfo1Size = 22;
	}
	else if (nTime1 < 60) {
		sInfo1.Format(255, L"     %2d초 후/ (%d)번째전 ", nTime1%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd1));
		if (nArrived1 == 1)
			sInfo1.Append("도착");
		else
			sInfo1.Append("출발");
	}
	else {
		sInfo1.Format(255, L"%2d분 %2d초 후/ (%d)번째전 ", nTime1/60, nTime1%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd1));
		if (nArrived1 == 1)
			sInfo1.Append("도착");
		else
			sInfo1.Append("출발");
	}
	//

	sInfo2.Format(255, L"%2d분 %2d초 후/ (%d)번째전 ", nTime2/60, nTime2%60, Osp::Base::Utility::Math::Abs(nOrd-nOrd2));
	if (nArrived2 == 1)
		sInfo2.Append("도착");
	else
		sInfo2.Append("출발");

	String sBusStat1, sBusStat2;
	//////
	// re-calculate
	if (pArv->__stationNm1.IsEmpty() || pArv->__vehId1 == "0") {
		sInfo1.Format(255, L"도착예정 버스 없음");
		pArv->__plainNo1 = " ";
	}
	else {
		sInfo11.Format(255, L" (%SKm/h,%S)", pArv->__traSpd1.GetPointer(), pArv->__stationNm1.GetPointer());

		if (pArv->__busType1 == "1") {
			sBusStat1 = "저상";
		}
		else if (pArv->__busType1 == "2") {
			sBusStat1 = "굴절";
		}
		if (pArv->__isLast1 == "1") {
			sBusStat1.Append("(막차)");
		}
	}
	if (pArv->__stationNm2.IsEmpty() || pArv->__vehId2 == "0") {
		sInfo2 = " ";
		pArv->__plainNo2 = " ";
	}
	else {
		sInfo21.Format(255, L" (%SKm/h,%S)", pArv->__traSpd2.GetPointer(), pArv->__stationNm2.GetPointer());

		if (pArv->__busType2 == "1") {
			sBusStat2 = "저상";
		}
		else if (pArv->__busType2 == "2") {
			sBusStat2 = "굴절";
		}
		if (pArv->__isLast2 == "1") {
			sBusStat2.Append("(막차)");
		}
	}

#ifdef _DEBUG_DONE
	ByteBuffer * pBB1  = Osp::Base::Utility::StringUtil::StringToUtf8N(sInfo1);
	ByteBuffer * pBB11 = Osp::Base::Utility::StringUtil::StringToUtf8N(sInfo11);
	AppLogDebug("    버스 1 정보 = [%s]/[%s]", pBB1->GetPointer(), pBB11->GetPointer());
	delete pBB1;
	delete pBB11;
#endif

	//
	int count = 0;
	attachRichText(pItem, count++, Rectangle(  5,   0, 180, 60), pArv->__rtNm, 	40, colorLine, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_BOLD);
	attachRichText(pItem, count++, Rectangle(  5,  60,  90, 20), sInterval, 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	//attachRichText(pItem, count++, Rectangle(  5,  80,  90, 40), pArv->__dir + "행", 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	pItem->AddElement(Rectangle( 10, 85, 70, 20), count++, *pbmpBus);

	//attachRichText(pItem, count++, Rectangle(100,  0, 85, 40), "버스이미지1", 		20, Color::COLOR_BLACK, Color(0,0,0,0));
	if (sBusStat1.IsEmpty()) {
		attachRichText(pItem, count++, Rectangle(100,  0, 85, 40), pArv->__dir + "행", 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	}
	else {
		attachRichText(pItem, count++, Rectangle(100,  0, 85, 40), sBusStat1, 	20, Color::COLOR_BLACK, Color(0,0,0,0));
	}
	attachRichText(pItem, count++, Rectangle(100, 40, 85, 20), pArv->__plainNo1, 	16, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200,  0, itemWidth-205, 30), sInfo1, 	nInfo1Size, Color::COLOR_RED,	Color(0,0,0,0));
	attachRichText(pItem, count++, Rectangle(200, 30, itemWidth-205, 30), sInfo11, 	22, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);

	if (sBusStat2.IsEmpty()) {
		//attachRichText(pItem, count++, Rectangle(100, 60, 85, 40), "버스이미지2", 		20, Color::COLOR_BLACK, Color(0,0,0,0));
	}
	else {
		attachRichText(pItem, count++, Rectangle(100, 60, 85, 40), sBusStat2, 		20, Color::COLOR_BLACK, Color(0,0,0,0));
	}
	attachRichText(pItem, count++, Rectangle(100,100, 85, 20), pArv->__plainNo2, 	16, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200, 60, itemWidth-205, 30), sInfo2, 	22, Color::COLOR_RED, 	Color(0,0,0,0), ALIGNMENT_RIGHT);
	attachRichText(pItem, count++, Rectangle(200, 90, itemWidth-205, 30), sInfo21, 	20, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_ITALIC);

	return pItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 우정사업본부 - 주소 관련
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_Address(int index, int itemWidth,  DataAddr* pinfo)
{
	int itemHeight = (pinfo->__roadAddress.IsEmpty() ?  90 : 150);

	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, itemHeight), LIST_ANNEX_STYLE_NORMAL);

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 30), pinfo->__zipCode, 		30, Color::COLOR_RED);
	attachRichText(pItem, itemIndex++, Rectangle( 50, 30, itemWidth-70, 60), pinfo->__address,  	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_NORMAL, TEXT_WRAP_CHARACTER_WRAP);

	if (!pinfo->__roadAddress.IsEmpty())
		attachRichText(pItem, itemIndex++, Rectangle( 50, 90, itemWidth-70, 60), pinfo->__roadAddress, 	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_NORMAL, TEXT_WRAP_CHARACTER_WRAP);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_AddressFreq(int index, int itemWidth,  AddressItem* pinfo)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 150), LIST_ANNEX_STYLE_NORMAL);

	int itemIndex = 0;
	int y = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  y, itemWidth/2-20, 40), pinfo->__zip, 				30, Color::COLOR_RED);
	attachRichText(pItem, itemIndex++, Rectangle( itemWidth/2+10,  y, itemWidth/2-20, 40), pinfo->__cdate, 	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_NORMAL, TEXT_WRAP_CHARACTER_WRAP);
	y += 40;

	attachRichText(pItem, itemIndex++, Rectangle( 50, 40, itemWidth-70, 50), pinfo->__address, 	 	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_NORMAL, TEXT_WRAP_CHARACTER_WRAP);
	y += 50;

	if (!pinfo->__addressNew.IsEmpty()) {
		attachRichText(pItem, itemIndex++, Rectangle( 50, 90, itemWidth-70, 50), pinfo->__addressNew,  	25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, LABEL_TEXT_STYLE_NORMAL, TEXT_WRAP_CHARACTER_WRAP);
		y += 50;
	}

	return pItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 환율 - 네이버에서
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_Currency(int index, int itemWidth,  DataCurrency* pinfo)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 80), LIST_ANNEX_STYLE_DETAILED);

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 40), pinfo->__currencyName, 		30, Color::COLOR_RED);

	//attachRichText(pItem, itemIndex++, Rectangle( 80,  40, itemWidth-100, 40), "매매기준율 : " + pinfo->__currencyRate[0] + " 원", 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	//attachRichText(pItem, itemIndex++, Rectangle( itemWidth/2-40,  40, itemWidth/2+20, 40), "매매기준율 : " + pinfo->__currencyRate[0] + " 원", 25);

	String sRate;
	sRate.Format(128, L"매매기준율 : %8ls 원", pinfo->__currencyRate[0].GetPointer());
	//attachRichText(pItem, itemIndex++, Rectangle( itemWidth/2-20,  40, itemWidth/2+10, 40), sRate, 25);
	attachRichText(pItem, itemIndex++, Rectangle( 160,  40, itemWidth/2+10, 40), sRate, 25);

	return pItem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 공공전화번호 - 카테고리
//
///////////////////////////////////////////////////////////////////////////////////////////////////

CustomItem * FactoryListItem::makeListItem_PublicPhoneCategory(int index, int itemWidth,  String* pstr)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 90), LIST_ANNEX_STYLE_DETAILED);

	int itemIndex = 0;

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 90), *pstr, 30, Color::COLOR_RED);

	return pItem;
}

CustomItem * FactoryListItem::makeListItem_PublicPhones(int index, int itemWidth,  PhoneItem* pinfo)
{
	CustomItem * pItem = new CustomItem();
	pItem->Construct(Dimension(itemWidth, 90), LIST_ANNEX_STYLE_DETAILED);

	int itemIndex = 0;

	String phoneNum = pinfo->__num_area;
	if (!phoneNum.IsEmpty()) {
		phoneNum.Append("-");
	}
	phoneNum.Append(pinfo->__num_station);
	if (!phoneNum.IsEmpty()) {
		phoneNum.Append("-");
	}
	phoneNum.Append(pinfo->__num_last);

	attachRichText(pItem, itemIndex++, Rectangle( 10,  0, itemWidth-20, 50), pinfo->__owner_name, 30, Color::COLOR_RED);
	attachRichText(pItem, itemIndex++, Rectangle( 10, 50, itemWidth-100, 40), phoneNum, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_RIGHT);
	//attachRichText(pItem, itemIndex++, Rectangle(100, 50, itemWidth-20, 40), phoneNum, 30);

	return pItem;
}
