/*
 * FormAxSetup.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxSetup.h"

FormAxSetup::FormAxSetup()
{
}

FormAxSetup::~FormAxSetup()
{
}

result FormAxSetup::OnInitializing(void)
{
	result r = AppBaseForm::OnInitializing();

	setFooterFocus();

	return r;
}

void FormAxSetup::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	bool bTabCommand = true;

	switch (actionId) {
	case FOOTER_ID_ITEM1:
		actionId = FORM_ID_SETUP_SHORTCUT;
		break;
	case FOOTER_ID_ITEM2:
		actionId = FORM_ID_SETUP_BUS;
		break;
	case FOOTER_ID_ITEM3:
		actionId = FORM_ID_SETUP_WEATHER;
		break;
//	case FOOTER_ID_ITEM4:
//		actionId = 0;
//		break;
	default:
		bTabCommand = false;
		break;
	}

	//
	if (bTabCommand) { // 탭으로 옆의 화면으로 이동하고자 할 경우, 이미 생성되어 있으면 ...
		SetPush(NO_PUSH); 	// 폼 자신을 네비게이션 스택에 넣지 않는다.
		//
		if (GetFormID() == actionId) {
			AppLogDebug("현재 자신의 폼이므로 무시합니다...");
			return;
		}
	}

	AppBaseForm::OnActionPerformed(source, actionId);
}

void FormAxSetup::buildFooter()
{
	Footer* pFooter = GetFooter();
	if (!pFooter) {
		return;
	}

	//pFooter->SetStyle(FOOTER_STYLE_SEGMENTED_TEXT);
	//pFooter->SetBackButton();

	pFooter->SetStyle(FOOTER_STYLE_TAB);

	FooterItem footerItem1;
	footerItem1.Construct(FOOTER_ID_ITEM1);
	footerItem1.SetText("바로가기");
	pFooter->AddItem(footerItem1);

	FooterItem footerItem2;
	footerItem2.Construct(FOOTER_ID_ITEM2);
	footerItem2.SetText("버스");
	pFooter->AddItem(footerItem2);

	FooterItem footerItem3;
	footerItem3.Construct(FOOTER_ID_ITEM3);
	footerItem3.SetText("날씨");
	pFooter->AddItem(footerItem3);

//	FooterItem footerItem4;
//	footerItem4.Construct(FOOTER_ID_ITEM4);
//	footerItem4.SetText("예약");
//	pFooter->AddItem(footerItem4);

	pFooter->AddActionEventListener(*this);
}

void FormAxSetup::setFooterFocus()
{
	Footer* pFooter = GetFooter();
	if (pFooter == null) {
		return;
	}

	AppLogDebug("set footer index ... current form ID = 0x%x", GetFormID());

	switch (GetFormID()) {
	case FORM_ID_SETUP_SHORTCUT:
		pFooter->SetItemSelected(0);
		break;
	case FORM_ID_SETUP_BUS:
		pFooter->SetItemSelected(1);
		break;
	case FORM_ID_SETUP_WEATHER:
		pFooter->SetItemSelected(2);
		break;
//	case M:
//		pFooter->SetItemSelected(3);
//		break;
	default:
		break;
	}
}

