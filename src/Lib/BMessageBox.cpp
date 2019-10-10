/*
 * BMessageBox.cpp
 *
 *  Created on: 2012. 2. 29.
 *      Author: SONG
 */

#include "BMessageBox.h"
#include "BLabel.h"
#include "BButton.h"

Color 		BMessageBox::__TitleColor;

BMessageBox::BMessageBox() : __pActionListener(null), __pHandler(null), __nID(0)
{
	__dimSize.width = 400;
	__dimSize.height = 400;

	__alignMessageHorz = ALIGNMENT_CENTER;
	__alignMessageVert = ALIGNMENT_MIDDLE;
}

BMessageBox::~BMessageBox()
{
	//AppLogDebug("자동 삭제 될 것으로 예상. 확인해 보고 지울 것!");
	//this->RemoveAllControls();
}

void BMessageBox::addTitle(Rectangle rect, const String & title)
{
	rect.x += 5;
	rect.width -= 25;

	BLabel * pLabelTitle = new BLabel();
	pLabelTitle->Construct(rect, title,
			BaseContainer::BASECONTROL_HEIGHT_LARGE,
			Color::COLOR_WHITE,
			ALIGNMENT_CENTER,
			ALIGNMENT_MIDDLE,
			TEXT_WRAP_NONE,
			FONT_STYLE_PLAIN);

	//pLabelTitle->SetBackgroundColor(Color::COLOR_BLUE);
	pLabelTitle->SetBackgroundColor(Color(0,83, 144));
	pLabelTitle->SetRound(7,7);

	AddControl(*pLabelTitle);

	//AppLogDebug("제목 창의 위치와 크기 = %d,%d - %d,%d", rect.x, rect.y, rect.width, rect.height);
}

void BMessageBox::addMessage(Rectangle rect, const String & message)
{
	// 보정
	rect.x += 5;

	BLabel * pLabel = new BLabel();
	pLabel->Construct(rect, message,
			BaseContainer::BASECONTROL_HEIGHT_SMALL,
			Color::COLOR_WHITE,
			__alignMessageHorz,
			__alignMessageVert,
			TEXT_WRAP_CHARACTER_WRAP,
			FONT_STYLE_PLAIN);

	/*
	Color color = GetColor();
	byte r = color.GetRed();
	byte g = color.GetGreen();
	byte b = color.GetBlue();
	byte a = color.GetAlpha();

	r += 10;
	g += 10;
	b += 10;
	color.SetColorComponents(r,g,b,a);
	pLabel->SetBackgroundColor(color);
	*/

	AddControl(*pLabel);

	//AppLogDebug("메시지 창의 위치와 크기 = %d,%d - %d,%d", rect.x, rect.y, rect.width, rect.height);
}

void BMessageBox::addButton(Rectangle rect, const String & title, const Osp::Ui::IActionEventListener* plistener, int nID)
{
	BButton * pBtn = new BButton();
	pBtn->Construct(rect, title);
	pBtn->SetActionId(nID);

#if 0
	if (plistener) {
		pBtn->AddActionEventListener(*plistener);
	}
	else {
		pBtn->AddActionEventListener(*this);
	}
#else
	pBtn->AddActionEventListener(*this);
#endif

	__pActionListener = (Osp::Ui::IActionEventListener*) plistener;

	pBtn->SetBackgroundColor(Color::COLOR_GREY);
	pBtn->SetTextColor(Color::COLOR_BLACK);
	pBtn->SetRound(3,3);  // 사각형 테두리

	AddControl(*pBtn);

	//AppLogDebug("버튼 %S의 위치와 크기 = %d,%d - %d,%d", title.GetPointer(), rect.x, rect.y, rect.width, rect.height);
	//AppLogDebug("버튼 추가 - 액션 ID = %d (0x%x) and __pActionListener = 0x%x", nID, nID, __pActionListener);
}

void BMessageBox::Wait(int seconds)
{
	Thread::Sleep(seconds * 1000);
	delete this;
}

void BMessageBox::Show(const String & title, const String & message)
{
	Construct(false, __dimSize);

	int space = 20;
	int y = 0;
	int h = __dimSize.height - (space*2); 	// 610 - 20*2 = 570

	if (title != "") {
		addTitle(Rectangle(0, y, __dimSize.width, BaseContainer::BASECONTROL_HEIGHT_LARGE+20), title);

		y += (BaseContainer::BASECONTROL_HEIGHT_LARGE + 20);  // y = 20 + (30+20) = 70
		h -= (BaseContainer::BASECONTROL_HEIGHT_LARGE + space);	// h = 570 - (30+20) = 520

	}

	addMessage(Rectangle(10, y, __dimSize.width-(10*4), h-20), message);

	SetShowState(true);
	Osp::Ui::Controls::Popup::Show();
}

void BMessageBox::ShowConfirm(const String & title, const String & message, const Osp::Ui::IActionEventListener* plistener, int confirmID, const String & confirmStr)
{
	Construct(false, __dimSize);

	int space = 20;
	int y = 0;
	int h = __dimSize.height - (space*3) - 50; 	// 610 - 20*3 - 50 = 500

	if (title != "") {
		addTitle(Rectangle(0, y, __dimSize.width, BaseContainer::BASECONTROL_HEIGHT_LARGE+20), title);

		y += (BaseContainer::BASECONTROL_HEIGHT_LARGE + 20);  // y = 20 + (30+20) = 70
		h -= (BaseContainer::BASECONTROL_HEIGHT_LARGE + space);	// h = 500 - (30+20) = 450

	}

	addMessage(Rectangle(10, y, __dimSize.width-(10*4), h-20), message);

	y += (h+20);

	//AppLogDebug("확인 버튼의 위치와 크기 = %d,%d - %d,%d", 10, y, __dimSize.width-(10*2), 50);

	addButton(Rectangle(15, y, __dimSize.width-(10*4), 50), confirmStr, plistener, confirmID);

	SetShowState(true);
	Osp::Ui::Controls::Popup::Show();
}

void BMessageBox::ShowYesNo(const String & title, const String & message, const Osp::Ui::IActionEventListener* plistener, int yesID, int noID, const String & yesStr, const String & noStr)
{
	Construct(false, __dimSize);

	int space = 20;
	int y = 0;
	int h = __dimSize.height - (space*3) - 50; 	// 610 - 20*3 - 50 = 500

	if (title != "") {
		addTitle(Rectangle(0, y, __dimSize.width, BaseContainer::BASECONTROL_HEIGHT_LARGE+20), title);

		y += (BaseContainer::BASECONTROL_HEIGHT_LARGE + 20);  // y = 20 + (30+20) = 70
		h -= (BaseContainer::BASECONTROL_HEIGHT_LARGE + space);	// h = 500 - (30+20) = 450

	}

	addMessage(Rectangle(10, y, __dimSize.width-(10*4), h-20), message);

	y += (h+20);

	//
	int x = 10;
	int w = (__dimSize.width-(10*5))/2; // 400

	addButton(Rectangle(x+5, y, w, 50), yesStr, plistener, yesID);
	addButton(Rectangle(x+w+x, y, w, 50), noStr, plistener, noID);


	SetShowState(true);
	Osp::Ui::Controls::Popup::Show();
}

void BMessageBox::ShowConfirm(const String & title, const String & message, IMessageBoxHandler * pHandler, const String & confirmStr)
{
	ShowConfirm(title, message, null, BMESSAGE_ID_CONFIRM, confirmStr);
	SetDelegate(pHandler);
}

void BMessageBox::ShowYesNo(const String & title, const String & message, IMessageBoxHandler * pHandler, const String & yesStr, const String & noStr)
{
	ShowYesNo(title, message, null, BMESSAGE_ID_YES, BMESSAGE_ID_NO, yesStr, noStr);
	SetDelegate(pHandler);
}

void BMessageBox::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	//AppLogDebug("action ID = %d", actionId);

	if (!__pHandler) {
		//AppLogDebug("action Handler 방식! action ID = %d and  __pActionListener 주소 = 0x%x", actionId, __pActionListener);
		if (__pActionListener) {
			__pActionListener->OnActionPerformed(source, actionId);
		}
		delete this;
		return;
	}

	//AppLogDebug("Delegate 방식!");

	switch (actionId) {
	case BMESSAGE_ID_CONFIRM:
		__pHandler->OnMessageBoxConfirm(__nID);
		break;
	case BMESSAGE_ID_YES:
		__pHandler->OnMessageBoxYes(__nID);
		break;
	case BMESSAGE_ID_NO:
		__pHandler->OnMessageBoxNo(__nID);
		break;
	default:
		break;
	}

	delete this;
}

