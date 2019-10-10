/*
 * FormAxAddr.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxAddr.h"

FormAxAddr::FormAxAddr()
{
}

FormAxAddr::~FormAxAddr()
{
}

result FormAxAddr::OnInitializing(void)
{
	result r = AppBaseForm::OnInitializing();

	setFooterFocus();

	return r;
}

void FormAxAddr::buildHeader()
{
	AppBaseForm::buildHeader();
}

void FormAxAddr::buildFooter()
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
	footerItem1.SetText("지번검색");
	pFooter->AddItem(footerItem1);

	FooterItem footerItem2;
	footerItem2.Construct(FOOTER_ID_ITEM2);
	footerItem2.SetText("도로명검색");
	pFooter->AddItem(footerItem2);

	FooterItem footerItem3;
	footerItem3.Construct(FOOTER_ID_ITEM3);
	footerItem3.SetText("주소변환");
	pFooter->AddItem(footerItem3);

	FooterItem footerItem4;
	footerItem4.Construct(FOOTER_ID_ITEM4);
	footerItem4.SetText("북 마크");
	pFooter->AddItem(footerItem4);

	pFooter->AddActionEventListener(*this);
}

void FormAxAddr::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	bool bTabCommand = true;

	switch (actionId) {
	case FOOTER_ID_ITEM1:
		actionId = FORM_ID_ADDRESS;
		break;
	case FOOTER_ID_ITEM2:
		actionId = FORM_ID_ADDRESS_ROAD;
		break;
	case FOOTER_ID_ITEM3:
		actionId = FORM_ID_ADDRESS_WEB;
		break;
	case FOOTER_ID_ITEM4:
		actionId = FORM_ID_ADDRESS_BOOKMARK;
		break;
	default:
		bTabCommand = false;
		break;
	}

	if (GetFormID() == actionId) {
		AppLogDebug("현재 자신의 폼이므로 무시합니다...");
		return;
	}

	//
	if (bTabCommand) { // 탭으로 옆의 화면으로 이동하고자 할 경우, 이미 생성되어 있으면 ...
		SetPush(NO_PUSH); 	// 폼 자신을 네비게이션 스택에 넣지 않는다.
	}

	AppBaseForm::OnActionPerformed(source, actionId);
}

void FormAxAddr::setFooterFocus()
{
	Footer* pFooter = GetFooter();
	if (pFooter == null) {
		return;
	}

	switch (GetFormID()) {
	case FORM_ID_ADDRESS:
		pFooter->SetItemSelected(0);
		break;
	case FORM_ID_ADDRESS_ROAD:
		pFooter->SetItemSelected(1);
		break;
	case FORM_ID_ADDRESS_WEB:
		pFooter->SetItemSelected(2);
		break;
	case FORM_ID_ADDRESS_BOOKMARK:
		pFooter->SetItemSelected(3);
		break;
	default:
		break;
	}
}

