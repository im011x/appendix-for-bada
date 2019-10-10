/*
 * FormAxBusAboutFreq.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusAboutFreq.h"

FormAxBusAboutFreq::FormAxBusAboutFreq() : __pTab(null), __pMyLine(null), __pMyStop(null), __pUsage(null)
{
}

FormAxBusAboutFreq::~FormAxBusAboutFreq()
{
}

bool FormAxBusAboutFreq::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("자주쓰는");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxBusAboutFreq::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();

	__pTab = makeTabBar(Point(0,0), bottomPanel, this);
	makeTabbarItem(__pTab, "[정류소]", ID_TABBAR_FAVORATE_STOP);
	makeTabbarItem(__pTab, "[노   선]", ID_TABBAR_FAVORATE_LINE);
	makeTabbarItem(__pTab, "[안   내]", ID_TABBAR_OTHERS);

	int y = __pTab->GetHeight();

	__pMyStop = new PanelAxBusMyStop(this);
	__pMyStop->Initialize(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, this);

	__pMyLine = new PanelAxBusMyLine(this);
	__pMyLine->Initialize(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, this);

	__pUsage = new PanelAxFreqUsage(this);
	__pUsage->Initialize(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, this);


	int nTab = GetValueByKey("자주쓰는_활성탭");
	SetShow(nTab);

	return r;
}

void FormAxBusAboutFreq::SetShow(int nTab)
{
	__pTab->SetItemSelected(nTab);

	switch (nTab) {
	case 0:
		__pMyStop->SetShowState(true);
		__pMyLine->SetShowState(false);
		__pUsage->SetShowState(false);
		break;
	case 1:
		__pMyStop->SetShowState(false);
		__pMyLine->SetShowState(true);
		__pUsage->SetShowState(false);
		break;
	case 2:
		__pMyStop->SetShowState(false);
		__pMyLine->SetShowState(false);
		__pUsage->Load();
		__pUsage->SetShowState(true);
		break;
	default:
		__pMyStop->SetShowState(false);
		__pMyLine->SetShowState(false);
		__pUsage->SetShowState(false);
		break;
	}

	this->RequestRedraw(true);
}

result FormAxBusAboutFreq::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}

void FormAxBusAboutFreq::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)	{
	case ID_MESSAGEBOX_CONFIRM:
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		break;
	case ID_MESSAGEBOX_UPDATE:
		SetPush(NO_PUSH);  // 다시 로드하기 전에 스택에 쌓지 않도록 하고 ...
		SetKeyValue("자주쓰는_활성탭", __pTab->GetSelectedItemIndex());
		GotoForm(GetFormID()); // 자시 자신의 폼을 다시 로드한다~
		break;
	case ID_TABBAR_FAVORATE_STOP:
		//AppLogDebug("자주쓰는 정류소 판넬을 보이게 합니다.");
		SetShow(0);
		break;
	case ID_TABBAR_FAVORATE_LINE:
		//AppLogDebug("자주쓰는 버스 노선 판넬을 보이게 합니다.");
		SetShow(1);
		break;
	case ID_TABBAR_OTHERS:
		//AppLogDebug("안내를 보이게 합니다.");
		SetShow(2);
		break;

		/*
	case ID_CONTEXT_LINEINFO:
		 AppLogDebug("LINEINFO를 보이게 합니다.");
		break;
	case ID_CONTEXT_LINESTOPS:
		 AppLogDebug("LINESTOPS를 보이게 합니다.");
		break;
	case ID_CONTEXT_LINEMAP:
		 AppLogDebug("LINEMAP를 보이게 합니다.");
		break;
		 */
	default:
		FormAxBus::OnActionPerformed(source, actionId);
		break;
	}
}

bool FormAxBusAboutFreq::AtWakeup()
{
	AppLog("AtWakeup()");
	InitIntent();
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

