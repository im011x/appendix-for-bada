/*
 * BLabel.cpp
 *
 *  Created on: 2012. 2. 14.
 *      Author: SONG
 */

#include "BLabel.h"
#include <FApp.h>

BLabel::BLabel() : __pEnrichedText(null), __pTextElement(null), __pBitmapBackground(null), __pColorBackground(null)
{
	// defalut alignment
	//__alignHorz = TEXT_ALIGNMENT_CENTER;
	__alignHorz = TEXT_ALIGNMENT_LEFT;
	__alignVert = TEXT_ALIGNMENT_MIDDLE;

	__dimArc.SetSize(0,0);
	__ptMargin.SetPosition(0,0);

	__styleLabel = LABEL_TEXT_STYLE_NORMAL;

	__defaultSize = BASECONTROL_DEFAULT_HEIGHT;
	__defaultColor = Color::COLOR_BLACK;
}

BLabel::~BLabel()
{
	//AppLogDebug("#### deleting BLabel");
	delete __pTextElement;
	delete __pEnrichedText;
	delete __pBitmapBackground;
	delete __pColorBackground;
}

result 	BLabel::Construct(const Osp::Graphics::Rectangle& rect, const Osp::Base::String& text, int size, Color textColor, HorizontalAlignment alignHorz, VerticalAlignment alignVert, TextWrap wrap, FontStyle fontStyle)
{
	BaseContainer::Construct(); 		// container function
	BaseContainer::SetBounds(rect);		// set the position and control

	//
	__pEnrichedText = new EnrichedText();

	__pEnrichedText->Construct(Dimension(rect.width, rect.height));
	__pEnrichedText->SetTextWrapStyle(wrap);
	__pEnrichedText->SetTextAbbreviationEnabled(true);

	//
	if (!text.IsEmpty()) {
		__pTextElement = new TextElement();
		__pTextElement->Construct(text);
		__pTextElement->SetFont(*GetFont(size));
		__pTextElement->SetTextColor(textColor);

		__pEnrichedText->Add(*__pTextElement);

		SetText(text, textColor, size, fontStyle);
	}

	__defaultSize = size;
	__defaultColor = textColor;
	__defaultFont = fontStyle;

	//
	SetTextVerticalAlignment(alignVert);
	SetTextHorizontalAlignment(alignHorz);

	return E_SUCCESS;
}

void BLabel::SetText(const Osp::Base::String& text)
{
	SetText(text, __defaultColor, __defaultSize, __defaultFont);
}

void BLabel::SetText(const Osp::Base::String& text, Color textColor, int size, FontStyle fontStyle)
{
	if (__pTextElement)
		__pTextElement->SetText(text);
	else {
		if (!text.IsEmpty()) {
			__pTextElement = new TextElement();
			__pTextElement->Construct(text);
			__pTextElement->SetFont(*GetFont(size, fontStyle));
			__pTextElement->SetTextColor(textColor);

			__pEnrichedText->Add(*__pTextElement);
		}
	}
}

void BLabel::SetTextSize(int height)
{
	if (!__pTextElement)
		return;

	__pTextElement->SetFont(*GetFont(height));
}

void BLabel::SetTextSize(const Osp::Base::String& text, int height)
{
	SetText(text);
	SetTextSize(height);
}

void BLabel::SetTextConfig(int size, LabelTextStyle label_style)
{
	if (!__pTextElement)
		return;

	FontStyle font_style = FONT_STYLE_PLAIN;

	switch (label_style) {
	case LABEL_TEXT_STYLE_BOLD:
		font_style = (FontStyle) (FONT_STYLE_PLAIN | FONT_STYLE_BOLD);
		break;
	case LABEL_TEXT_STYLE_ITALIC:
		font_style = (FontStyle) (FONT_STYLE_PLAIN | FONT_STYLE_ITALIC);
		break;
	case LABEL_TEXT_STYLE_NORMAL:
	default:
		break;
	}

	__styleLabel = label_style;

	__pTextElement->SetFont(*GetFont(size, font_style));
}

void BLabel::SetBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap)
{
	if (__pBitmapBackground)
		delete __pBitmapBackground;

	Rectangle rect(0,0, bitmap.GetWidth(), bitmap.GetHeight());

	__pBitmapBackground = new Bitmap();
	__pBitmapBackground->Construct(bitmap, rect);
}

void BLabel::SetBackgroundBitmap(const Osp::Graphics::Bitmap* pbitmap)
{
	SetBackgroundBitmap(*pbitmap);
}

void BLabel::SetBackgroundImage(const Osp::Base::String & filename)
{
	//AppLogDebug("BLabel::SetBackgroundImage - %S", filename.GetPointer());
	Osp::App::AppResource * pAppResource = Osp::App::Application::GetInstance()->GetAppResource();
	__pBitmapBackground = pAppResource->GetBitmapN(filename);
}

void BLabel::SetTextVerticalAlignment(VerticalAlignment alignment)
{
	switch (alignment) {
	case ALIGNMENT_TOP:
		__alignVert = TEXT_ALIGNMENT_TOP;
		break;
	case ALIGNMENT_MIDDLE:
		__alignVert = TEXT_ALIGNMENT_MIDDLE;
		break;
	case ALIGNMENT_BOTTOM:
		__alignVert = TEXT_ALIGNMENT_BOTTOM;
		break;
	default:
		__alignVert = TEXT_ALIGNMENT_MIDDLE;
		break;
	}

	if (__pEnrichedText)
		__pEnrichedText->SetVerticalAlignment(__alignVert);
}

void BLabel::SetTextHorizontalAlignment(HorizontalAlignment alignment)
{
	switch (alignment) {
	case ALIGNMENT_LEFT:
		__alignHorz = TEXT_ALIGNMENT_LEFT;
		break;
	case ALIGNMENT_CENTER:
	__alignHorz = TEXT_ALIGNMENT_CENTER;
		break;
	case ALIGNMENT_RIGHT:
	__alignHorz = TEXT_ALIGNMENT_RIGHT;
		break;
	default:
		__alignHorz = TEXT_ALIGNMENT_CENTER;
		break;
	}

	if (__pEnrichedText)
		__pEnrichedText->SetHorizontalAlignment(__alignHorz);
}

void BLabel::SetBackgroundColor(const Osp::Graphics::Color& color)
{
	if (__pColorBackground)
		delete __pColorBackground;

	__pColorBackground = new Color(color);
}

void BLabel::SetTextColor(const Osp::Graphics::Color& color)
{
	if (__pTextElement)
		__pTextElement->SetTextColor(color);
}

void BLabel::SetWrap(TextWrap wrap)
{
	if (__pEnrichedText)
		__pEnrichedText->SetTextWrapStyle(wrap);
}


Osp::Base::String BLabel::GetText(void) const
{
	if (__pTextElement)
		return __pTextElement->GetText();

	return String("");
}

HorizontalAlignment	BLabel::GetTextHorizontalAlignment(void) const
{
	return (HorizontalAlignment) (__alignHorz - 1);
}

VerticalAlignment BLabel::GetTextVerticalAlignment(void) const
{
	return (VerticalAlignment) (__alignVert - 1);
}

Osp::Graphics::Color BLabel::GetBackgroundColor(void) const
{
	if (__pColorBackground)
		return *__pColorBackground;

	return Color::COLOR_VIOLET;
}

Osp::Graphics::Color BLabel::GetTextColor(void) const
{
	if (__pTextElement)
		return __pTextElement->GetTextColor();

	return Color::COLOR_YELLOW;
}

int BLabel::GetTextSize(void) const
{
	return GetFontSize();
}

LabelTextStyle BLabel::GetTextStyle(void) const
{
	return __styleLabel;
}

result BLabel::OnDraw(void)
{
	if (!IsVisible()) {
		//AppLogDebug("!!!! 보이지 않는 상태이니까 ... pass !!!!");
		return E_SUCCESS;
	}

	Canvas * pCanvas = GetCanvasN();

	if (!pCanvas) {
		AppLogDebug("!!!! GetCanvasN 실패 !!!!");
		//return E_SYSTEM;
		return E_SUCCESS;
	}

	//
	draw(pCanvas);

	//
	delete pCanvas;
	pCanvas = null;

	return E_SUCCESS;
}

void BLabel::draw(Canvas * pCanvas)
{
	Rectangle rt = this->GetBounds();
	rt.x = rt.y = 0;

	// background 이미지 처리
	if (__pBitmapBackground) {
		//pCanvas->DrawBitmap(Point(0,0), *__pBitmapBackground);
		pCanvas->DrawBitmap(rt, *__pBitmapBackground);
	}
	else if (__pColorBackground) {  // background 이미지 없고 색상 있으면 색상 처리
		//pCanvas->FillRectangle(*__pColorBackground, rt);
		pCanvas->FillRoundRectangle(*__pColorBackground, rt, __dimArc);
	}

	//
	//pCanvas->DrawText(Point(0,0), *__pEnrichedText);
	if (__pEnrichedText->GetHorizontalAlignment() == TEXT_ALIGNMENT_LEFT) {
		//pCanvas->DrawText(__ptMargin, *__pEnrichedText);
		Dimension dimSz = __pEnrichedText->GetSize();

		Dimension dimSz2;
		dimSz2.width  = dimSz.width  - __ptMargin.x - (__ptMargin.x/2);
		dimSz2.height = dimSz.height - __ptMargin.y;
		__pEnrichedText->SetSize(dimSz2);

		pCanvas->DrawText(__ptMargin, *__pEnrichedText);
		__pEnrichedText->SetSize(dimSz);
	}
	else if (__pEnrichedText->GetHorizontalAlignment() == TEXT_ALIGNMENT_CENTER) {
		pCanvas->DrawText(Point(0,0), *__pEnrichedText);
	}
	else {
		Dimension dimSz = __pEnrichedText->GetSize();
		Dimension dimSzRight;
		dimSzRight.width  = dimSz.width  - __ptMargin.x - 10;
		dimSzRight.height = dimSz.height - __ptMargin.y;
		__pEnrichedText->SetSize(dimSzRight);
		pCanvas->DrawText(Point(10,0), *__pEnrichedText);
		__pEnrichedText->SetSize(dimSz);
	}
}

