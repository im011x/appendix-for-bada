/*
 * FormAxBus.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBus.h"
#include "BacNavigationController.h"

int 	FormAxBus::__lastTabIndex = 0;

FormAxBus::FormAxBus() : __tabID (0)
{
}

FormAxBus::~FormAxBus()
{
}

result FormAxBus::OnInitializing(void)
{
	result r = AppBaseForm::OnInitializing();

	setTabID();
	setBread();
	setFooterFocus();

	return r;
}

void FormAxBus::buildHeader()
{
	AppBaseForm::buildHeader();
}

void FormAxBus::buildFooter()
{
	Footer* pFooter = GetFooter();
	if (!pFooter) {
		return;
	}

	pFooter->SetStyle(FOOTER_STYLE_TAB);
	//pFooter->SetStyle(FOOTER_STYLE_BUTTON_ICON);
	//pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

	FooterItem footerItem1;
	footerItem1.Construct(FOOTER_ID_ITEM1);
	footerItem1.SetText("정류소");
	pFooter->AddItem(footerItem1);

	FooterItem footerItem2;
	footerItem2.Construct(FOOTER_ID_ITEM2);
	footerItem2.SetText("노선");
	pFooter->AddItem(footerItem2);

	FooterItem footerItem3;
	footerItem3.Construct(FOOTER_ID_ITEM3);
	footerItem3.SetText("자주쓰는");
	pFooter->AddItem(footerItem3);

	FooterItem footerItem4;
	footerItem4.Construct(FOOTER_ID_ITEM4);
	footerItem4.SetText("내 버스");
	pFooter->AddItem(footerItem4);

	//FooterItem footerItem5;
	//footerItem5.Construct(FOOTER_ID_ITEM5);
	//footerItem5.SetText("대중교통");
	//pFooter->AddItem(footerItem5);

	pFooter->AddActionEventListener(*this);
}

void FormAxBus::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	bool bTabCommand = true;

	switch (actionId) {
	case FOOTER_ID_ITEM1:
		actionId = FORM_ID_BUS_ABOUT_STOP;
		break;
	case FOOTER_ID_ITEM2:
		actionId = FORM_ID_BUS_ABOUT_LINE;
		break;
	case FOOTER_ID_ITEM3:
		actionId = FORM_ID_BUS_ABOUT_FREQ;
		break;
	case FOOTER_ID_ITEM4:
		actionId = FORM_ID_BUS_ABOUT_MYBUS;
		break;
	default:
		bTabCommand = false;
		break;
	}

	if (GetFormID() == actionId) {
		AppLogDebug("현재 자신의 폼이므로 무시합니다...");
		return;
	}

	if (bTabCommand) { // 탭으로 옆의 화면으로 이동하고자 할 경우, 이미 생성되어 있으면 ...
		SetPush(NO_PUSH); 	// 폼 자신을 네비게이션 스택에 넣지 않는다.
	}

	Footer* pFooter = GetFooter();
	if (pFooter && bTabCommand) {
		if (pFooter->GetSelectedItemIndex() == __tabID) {
			AppLogDebug("기존의 빵폼을 찾으러 갑니다.");
			GotoBread("breadBusTabHome");
			return;
		}
		else {
			AppLogDebug("현존하는 모든 폼을 없애고 ... 새로운 폼을 생성하러 갑니다.");
			Flush(false);
		}
	}

	AppBaseForm::OnActionPerformed(source, actionId);
}

void FormAxBus::setFooterFocus()
{
	Footer* pFooter = GetFooter();
	if (pFooter == null) {
		return;
	}

	switch (GetFormID()) {
	case FORM_ID_BUS_ABOUT_STOP:
		__lastTabIndex = 0;
		break;
	case FORM_ID_BUS_ABOUT_LINE:
		__lastTabIndex = 1;
		break;
	case FORM_ID_BUS_ABOUT_FREQ:
		__lastTabIndex = 2;
		break;
	case FORM_ID_BUS_ABOUT_MYBUS:
		__lastTabIndex = 3;
		break;
	default:
		break;
	}

	pFooter->SetItemSelected(__lastTabIndex);
}

void FormAxBus::setBread()
{
	switch (GetFormID()) {
	case FORM_ID_BUS_ABOUT_STOP:
	case FORM_ID_BUS_ABOUT_LINE:
	case FORM_ID_BUS_ABOUT_FREQ:
	case FORM_ID_BUS_ABOUT_MYBUS:
		SetBreadName("breadBusTabHome");
		break;
	default:
		break;
	}
}

void FormAxBus::setTabID()
{
	switch (GetFormID()) {
	case FORM_ID_BUS_ABOUT_STOP:
	case FORM_ID_BUS_STOPNEAR:
	case FORM_ID_BUS_STOPLINES:
	case FORM_ID_BUS_STOPINFO:
	case FORM_ID_BUS_STOPMAP:
	case FORM_ID_BUS_STOPNAMES:
		__tabID = 0;
		break;
	case FORM_ID_BUS_ABOUT_LINE:
	case FORM_ID_BUS_LINEINFO:
	case FORM_ID_BUS_LINEMAP:
	case FORM_ID_BUS_LINESTOPS:
		__tabID = 1;
		break;
	case FORM_ID_BUS_ABOUT_FREQ:
		__tabID = 2;
		break;
	case FORM_ID_BUS_ABOUT_MYBUS:
		__tabID = 3;
		break;
	default:
		break;
	}

}

Bitmap * FormAxBus::GetBitmapBus(const String & routeType)
{
	Bitmap * pbmpBus = null;

	int theType = 0;

	Integer::Parse(routeType, theType);
	switch (theType) {
	case 0:
		//lineType = "공용버스";
		break;
	case 1:		// "공항버스"
		pbmpBus = loadBitmapPool("appendix/bus_airport.png");
		break;
	case 3:		// "간선버스"
		pbmpBus = loadBitmapPool("appendix/bus_blue.png");
		break;
	case 4:		// "지선버스"
		pbmpBus = loadBitmapPool("appendix/bus_green.png");
		break;
	case 5:		// "순환버스"
		pbmpBus = loadBitmapPool("appendix/bus_yello.png");
		break;
	case 6:		// "광역버스"
		pbmpBus = loadBitmapPool("appendix/bus_red.png");
		break;
	case 7:
		//lineType = "인천버스";
		break;
	case 8:
		//lineType = "경기버스";
		break;
	case 9:
		//lineType = "기타(폐지)";
		break;
	default:
		//lineType = "기타";
		break;
	}

	if (pbmpBus == null) {
		pbmpBus = loadBitmapPool("appendix/bus_general.png");
	}

	return pbmpBus;
}
