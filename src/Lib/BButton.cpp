/*
 * BButton.cpp
 *
 *  Created on: 2012. 2. 15.
 *      Author: SONG
 */

#include "BButton.h"

int 						BButton::__ButtonSensitivity = 2; // 0: 일반, 1:조금 높임, 2 : 아주 높임! 즉 TrouchReleased() 이외의 부분에서도 처리 여부
BCanAccssibility *			BButton::__pButtonTTS = null;

BButton::BButton() :
	__pActionHandler(null)
	, __nActionID(-1)
	, __colorNoraml(Color::COLOR_BLACK)
	, __colorPressed(Color::COLOR_WHITE)
	, __colorDisabled(Color::COLOR_GREY)
	, __colorHighlighted(Color::COLOR_VIOLET)
	, __colorBorderOnNoImage(Color::COLOR_BLACK)
{
	 // 버튼을 토글 기능으로 사용할 것 인가? 말것인가? 즉 선택 상태 고정 기능 사용여부
	__bStay = false;
	__bSelected = false;

	__bPressed = false;


	__pNormalBitmapPosition = null;
	__pPressedBitmapPosition = null;
	__pDisabledBitmapPosition = null;
	__pNormalBitmap = null;
	__pPressedBitmap = null;
	__pDisabledBitmap = null;

	__pNormalBitmapPosition2 = null;
	__pNormalBitmap2 = null;
	__pPressedBitmapPosition2 = null;
	__pPressedBitmap2 = null;
	__pDisabledBitmapPosition2 = null;
	__pDisabledBitmap2 = null;

	__pNormalBackgroundBitmap = null;
	__pPressedBackgroundBitmap = null;
	__pHighlightedBackgroundBitmap = null;

	//__ptPressedMargin.SetPosition(3,3);
	__ptPressedMargin.SetPosition(0,0);

	//
	__bPressedLongTouch = false;
}

BButton::~BButton()
{
	delete __pNormalBitmapPosition;
	delete __pPressedBitmapPosition;
	delete __pDisabledBitmapPosition;
	delete __pNormalBitmap;
	delete __pPressedBitmap;
	delete __pDisabledBitmap;

	delete __pNormalBitmapPosition2;
	delete __pNormalBitmap2;
	delete __pPressedBitmapPosition2;
	delete __pPressedBitmap2;
	delete __pDisabledBitmapPosition2;
	delete __pDisabledBitmap2;

	delete __pNormalBackgroundBitmap;
	delete __pPressedBackgroundBitmap;
	delete __pHighlightedBackgroundBitmap;
}

result BButton::Construct(const Osp::Graphics::Rectangle& rect, const Osp::Base::String& text, int size, const Osp::Graphics::Color& color
		, HorizontalAlignment alignHorz, VerticalAlignment alignVert, FontStyle fontStyle)
{
	result r = BLabel::Construct(rect, text, size, color, alignHorz, alignVert, TEXT_WRAP_NONE, fontStyle);

	if (r != E_SUCCESS) {
		return r;
	}

	__colorNoraml = color;

	// 일단 버튼 라운드를 처리한다.
	SetRound(10,10);

	this->AddTouchEventListener(*this);

	SetDragEnabled(true);
	SetDropEnabled(true);
	this->AddDragDropEventListener(*this);

	return E_SUCCESS;
}

void BButton::AddActionEventListener(const Osp::Ui::IActionEventListener& listener)
{
	__pActionHandler = (Osp::Ui::IActionEventListener *) &listener;
}

void BButton::AddActionEventListener(Osp::Ui::IActionEventListener* plistener)
{
	__pActionHandler = plistener;
}

void BButton::set_position_bitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap, Point **ppDestPoint, Bitmap **ppDestBitmap)
{
	// 일단 position
	if (!(*ppDestPoint)) {
		*ppDestPoint = new Point();
	}

	(*ppDestPoint)->SetPosition(position);

	// 이단 bitmap
	if ((*ppDestBitmap))
		delete (*ppDestBitmap);

	Rectangle rect(0,0, bitmap.GetWidth(), bitmap.GetHeight());

	(*ppDestBitmap) = new Bitmap();
	(*ppDestBitmap)->Construct(bitmap, rect);

	//(*ppDestBitmap)->Scale(Osp::Graphics::Dimension(2,2));
}

void BButton::SetNormalBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pNormalBitmapPosition, &__pNormalBitmap);
}

void BButton::SetPressedBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pPressedBitmapPosition, &__pPressedBitmap);
}

void BButton::SetDisabledBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pDisabledBitmapPosition, &__pDisabledBitmap);
}

void BButton::SetNormalBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pNormalBitmapPosition2, &__pNormalBitmap2);
}

void BButton::SetPressedBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pPressedBitmapPosition2, &__pPressedBitmap2);
}

void BButton::SetDisabledBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap)
{
	//AppLogDebug("position = %d/%d", position.x, position.y);
	set_position_bitmap(position, bitmap, &__pDisabledBitmapPosition2, &__pDisabledBitmap2);
}


void BButton::SetNormalBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap)
{
	if (__pNormalBackgroundBitmap)
		delete __pNormalBackgroundBitmap;

	Rectangle rect(0,0, bitmap.GetWidth(), bitmap.GetHeight());

	__pNormalBackgroundBitmap = new Bitmap();
	__pNormalBackgroundBitmap->Construct(bitmap, rect);
}

void BButton::SetPressedBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap)
{
	if (__pPressedBackgroundBitmap)
		delete __pPressedBackgroundBitmap;

	Rectangle rect(0,0, bitmap.GetWidth(), bitmap.GetHeight());

	__pPressedBackgroundBitmap = new Bitmap();
	__pPressedBackgroundBitmap->Construct(bitmap, rect);
}

void BButton::SetHighlightedBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap)
{
	if (__pHighlightedBackgroundBitmap)
		delete __pHighlightedBackgroundBitmap;

	Rectangle rect(0,0, bitmap.GetWidth(), bitmap.GetHeight());

	__pHighlightedBackgroundBitmap = new Bitmap();
	__pHighlightedBackgroundBitmap->Construct(bitmap, rect);
}

result BButton::SetColor(ButtonStatus status, const Osp::Graphics::Color& color)
{
	switch (status) {
	case BUTTON_STATUS_NORMAL:
		SetNormalTextColor(color);
		break;
	case BUTTON_STATUS_DISABLED:
		SetDisabledTextColor(color);
		break;
	case BUTTON_STATUS_PRESSED:
		SetPressedTextColor(color);
		break;
	case BUTTON_STATUS_HIGHLIGHTED:
		SetHighlightedTextColor(color);
		break;
	}

	return E_SUCCESS;
}

Osp::Graphics::Color BButton::GetColor(ButtonStatus status) const
{
	switch (status) {
	case BUTTON_STATUS_NORMAL:
		return GetNormalTextColor();
	case BUTTON_STATUS_DISABLED:
		return GetDisabledTextColor();
	case BUTTON_STATUS_PRESSED:
		return GetPressedTextColor();
	case BUTTON_STATUS_HIGHLIGHTED:
		return GetHighlightedTextColor();
	}

	return GetNormalTextColor();
}

void BButton::SetNormalTextColor(const Osp::Graphics::Color& color)
{
	__colorNoraml = color;

	// 일단 자동으로 반전 이미지 색상 설정
	byte r,g,b,a;
	__colorNoraml.GetColorComponents(r,g,b,a);
	r ^= 0xff;
	g ^= 0xff;
	b ^= 0xff;
	SetPressedTextColor(Color(r,g,b,a));
}

void BButton::OnTouchPressed(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo)
{
	__bPressedLongTouch = false;

	if (!IsEnabled()) {
		return;
	}

	__bPressed = true;

	// 선택 여부
	__bSelected = !__bSelected;

	RequestRedraw();
}

void BButton::OnTouchReleased(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo)
{
	AppLogDebug("OnTouchReleased");
	fire(0);
}

void BButton::SetSensitivityHighest()
{
	// TouchReleased() + OnTouchFocusOut() + OnTouchDropped() 에서 처리
	BButton::__ButtonSensitivity = 2;
}

void BButton::SetSensitivityHigh()
{
	// TouchReleased() + OnTouchFocusOut() 에서 처리
	BButton::__ButtonSensitivity = 1;
}

void BButton::SetSensitivityNormal()
{
	// TouchReleased() 에서만 처리
	BButton::__ButtonSensitivity = 0;
}


void BButton::fire(int n)
{
	bool doAct = false;

	if (__bPressed) {
		doAct = true;
		__bPressed = false;
	}

	RequestRedraw();

	if (__bPressedLongTouch) {
		return;
	}

	if (!IsEnabled()) {
		return;
	}

	if (n > BButton::__ButtonSensitivity) {
		return;
	}

	if (doAct) {
		if (__pActionHandler && __nActionID >= 0) {
			__pActionHandler->OnActionPerformed(*this, __nActionID);
		}
	}
}

void BButton::OnTouchFocusIn(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo)
{
	__bPressed = true;
	RequestRedraw();
}

void BButton::OnTouchFocusOut(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo)
{
	AppLogDebug("OnTouchFocusOut");
	fire(1);
}

void BButton::OnTouchMoved(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo)
{
//	AppLogDebug("!!!");
}

void BButton::OnTouchLongPressed(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo)
{
	AppLogDebug("버튼 텍스트를 음성 출력합니다...");
	__bPressedLongTouch = true;

	String btnText = GetText();
	if (!btnText.IsEmpty()) {
		if (BButton::__pButtonTTS == null) {
			BButton::__pButtonTTS = new BCanAccssibility();
		}
		BButton::__pButtonTTS->TTS(btnText);
	}
	else {
		if (!__textSpeech.IsEmpty()) {
			BButton::__pButtonTTS->TTS(__textSpeech);
		}
	}

}

void BButton::OnTouchDragged(const Osp::Ui::Control& source, const Osp::Graphics::Point& startPosition, const Osp::Graphics::Point& currentPosition)
{
}

void BButton::OnTouchDropped(const Osp::Ui::Control& source, const Osp::Graphics::Point& startPosition, const Osp::Graphics::Point& currentPosition)
{
	AppLogDebug("OnTouchDropped");
	fire(2);
}

//void BButton::OnFocusGained(const Osp::Ui::Control& source)
//{
//	AppLogDebug("");
//}
//
//void BButton::OnFocusLost(const Osp::Ui::Control& source)
//{
//	AppLogDebug("");
//}

#if 0
result BButton::OnDraw(void)
{
	if (!IsVisible()) {
		return E_SUCCESS;
	}

	Canvas * pCanvas = GetCanvasN();

	if (!pCanvas) {
		return E_SUCCESS;
	}

	// 상태에 따라 보여줄 이미지 결정
	Bitmap * pBitmapBG = __pNormalBackgroundBitmap;
	Bitmap * pBitmapFG = __pNormalBitmap;
	Point  * pPointFG = __pNormalBitmapPosition;
	Color  * pColor = &__colorNoraml;
	Point 	ptMargin = __ptMargin;

	Color colorText = GetTextColor(); 	// 원래 Text 색상

	if (__bPressed) {  // 무언가 눌렸을 경우
		pBitmapBG = __pPressedBackgroundBitmap;
		pBitmapFG = __pPressedBitmap;
		pPointFG = __pPressedBitmapPosition;
		pColor = &__colorPressed;

		ptMargin += __ptPressedMargin;

		byte r,g,b,a;
		colorText.GetColorComponents(r,g,b,a);
		r ^= 0xff;
		g ^= 0xff;
		b ^= 0xff;
		SetTextColor(Color(r,g,b,a));
	}
	else {
		if (__bSelected) {
			if (__bStay) {
				pBitmapBG = __pHighlightedBackgroundBitmap;
				pColor = &__colorHighlighted;
			}
			//pColor = &__colorHighlighted;
		}

	}

	if (!IsEnabled()) {
		pBitmapFG = __pDisabledBitmap;
		pPointFG = __pDisabledBitmapPosition;
		pColor = &__colorDisabled;
	}

	// 다시 한번 더 체크
	if (pBitmapBG == null)
		pBitmapBG = __pNormalBackgroundBitmap;
	if (pBitmapFG == null)
		pBitmapFG = __pNormalBitmap;


	// 일단 좌표 얻어 오고 ...
	Rectangle rt = this->GetBounds();
	rt.x = rt.y = 0;

	//
	// background 이미지 처리
	if (pBitmapBG) {
		pCanvas->DrawBitmap(Point(0,0), *pBitmapBG);
	}
	else if (__pColorBackground) {
		pCanvas->FillRoundRectangle(*__pColorBackground, rt, __dimArc);
	}
	else {  // background 이미지 없고 색상 있으면 색상 처리
		//pCanvas->FillRoundRectangle(*pColor, rt, __dimArc);
		pCanvas->SetLineWidth(3);
		pCanvas->SetForegroundColor(Color::COLOR_WHITE);
		pCanvas->DrawRectangle(rt);
	}

	// foreground 이미지 처리
	if (pBitmapFG) {
		pCanvas->DrawBitmap(*pPointFG, *pBitmapFG);
	}

	//
	pCanvas->DrawText(ptMargin, *__pEnrichedText);

	if (__bPressed) {  // 무언가 눌렸을 경우 바꾼 것 원상 회복 !
		SetTextColor(colorText);
	}

	//
	delete pCanvas;
	pCanvas = null;

	return E_SUCCESS;
}
#endif

result BButton::OnDraw(void)
{
	if (!IsVisible()) {
		return E_SUCCESS;
	}

	Canvas * pCanvas = GetCanvasN();

	if (!pCanvas) {
		return E_SUCCESS;
	}

	// 상태에 따라 보여줄 이미지 결정
	Bitmap * pBitmapBG = __pNormalBackgroundBitmap;

	Bitmap * pBitmapFG = __pNormalBitmap;
	Point  * pPointFG = __pNormalBitmapPosition;

	Bitmap * pBitmapFG2 = __pNormalBitmap2;
	Point  * pPointFG2 = __pNormalBitmapPosition2;

	Color  * pColorText = &__colorNoraml;
	Point 	ptMargin = __ptMargin;

	if (__bPressed) {  // 무언가 눌렸을 경우
		pBitmapBG = __pPressedBackgroundBitmap;

		pBitmapFG = __pPressedBitmap;
		pPointFG = __pPressedBitmapPosition;

		pBitmapFG2 = __pPressedBitmap2;
		pPointFG2 = __pPressedBitmapPosition2;

		pColorText = &__colorPressed;

		ptMargin += __ptPressedMargin;
	}
	else {
		if (__bSelected) {
			if (__bStay) {
				pBitmapFG = __pPressedBitmap;
				pBitmapBG = __pHighlightedBackgroundBitmap;
				pColorText = &__colorHighlighted;
			}
			//pColorText = &__colorHighlighted;
		}

	}

	if (!IsEnabled()) {
		pBitmapFG = __pDisabledBitmap;
		pPointFG = __pDisabledBitmapPosition;

		pBitmapFG2 = __pDisabledBitmap2;
		pPointFG2 = __pDisabledBitmapPosition2;

		pColorText = &__colorDisabled;
	}

	// 다시 한번 더 체크
	if (pBitmapBG == null)
		pBitmapBG = __pNormalBackgroundBitmap;
	if (pBitmapFG == null) {
		pBitmapFG = __pNormalBitmap;
		pPointFG = __pNormalBitmapPosition;
	}
	if (pBitmapFG2 == null) {
		pBitmapFG2 = __pNormalBitmap2;
		pPointFG2 = __pNormalBitmapPosition2;
	}

	// 일단 좌표 얻어 오고 ...
	Rectangle rt = this->GetBounds();
	rt.x = rt.y = 0;

	//
	// background 이미지 처리
	if (pBitmapBG) {
		//pCanvas->DrawBitmap(Point(0,0), *pBitmapBG);
		pCanvas->DrawBitmap(rt, *pBitmapBG);
	}
	else if (__pColorBackground) {
		pCanvas->FillRoundRectangle(*__pColorBackground, rt, __dimArc);
	}
	else {  // background 이미지 없고 색상 있으면 색상 처리
		//pCanvas->FillRoundRectangle(*pColorText, rt, __dimArc);
		pCanvas->SetLineWidth(3);
		pCanvas->SetForegroundColor(__colorBorderOnNoImage);
		pCanvas->DrawRoundRectangle(rt, __dimArc);
	}

	// foreground 이미지 처리
	if (pBitmapFG) {
		pCanvas->DrawBitmap(*pPointFG, *pBitmapFG);
		//AppLogDebug("FG 이미지1을 그리는데 위치는 %d,%d ", pPointFG->x, pPointFG->y);
	}

	// second foreground 이미지 처리
	if (pBitmapFG2) {
		pCanvas->DrawBitmap(*pPointFG2, *pBitmapFG2);
		//AppLogDebug("FG 이미지2을 그리는데 위치는 %d,%d ", pPointFG->x, pPointFG->y);
	}

	if (pColorText && !GetText().IsEmpty())  {
		SetTextColor(*pColorText);
	}
	//
	if (__pEnrichedText) {
		pCanvas->DrawText(ptMargin, *__pEnrichedText);
		//AppLogDebug("텍스트 그리는데 위치는 %d,%d ", ptMargin.x, ptMargin.y);
	}
	//
	delete pCanvas;
	pCanvas = null;

	return E_SUCCESS;
}
