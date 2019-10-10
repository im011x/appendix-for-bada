/*
 * BLabel.h
 *
 *  Created on: 2012. 2. 14.
 *      Author: SONG
 */

#ifndef BENNY_LABEL_H_
#define BENNY_LABEL_H_

#include "BaseContainer.h"

class _EXPORT_ BLabel : public BaseContainer
{
public:
	BLabel();
	virtual ~BLabel();

protected:
	EnrichedText 	* 		__pEnrichedText;
	TextElement 	* 		__pTextElement;

	Bitmap * 				__pBitmapBackground;
	Color  * 				__pColorBackground;

	Dimension 				__dimArc; 		// round 처리
	Point 					__ptMargin; 	// margin 처리
	LabelTextStyle 			__styleLabel;

private:
	TextHorizontalAlignment 	__alignHorz;
	TextVerticalAlignment 		__alignVert;

	int 						__defaultSize;
	Color 						__defaultColor;
	FontStyle  					__defaultFont;

public:
	// mimic Label member functions
	result 	Construct(const Osp::Graphics::Rectangle& rect, const Osp::Base::String& text, int size = BASECONTROL_DEFAULT_HEIGHT,
							Color textColor=Color::COLOR_BLACK, HorizontalAlignment alignHorz=ALIGNMENT_LEFT, VerticalAlignment alignVert=ALIGNMENT_MIDDLE,
							TextWrap wrap = TEXT_WRAP_NONE, FontStyle fontStyle=FONT_STYLE_PLAIN);

	//
	EnrichedText 	* 				GetRichText() 	{ 	return __pEnrichedText; };


	Osp::Base::String 				GetText(void) const;
	HorizontalAlignment				GetTextHorizontalAlignment(void) const;
	VerticalAlignment				GetTextVerticalAlignment(void) const;
	virtual Osp::Graphics::Color 	GetBackgroundColor(void) const;
	virtual Osp::Graphics::Color 	GetTextColor(void) const;
	int 							GetTextSize(void) const;
	LabelTextStyle 					GetTextStyle(void) const;

	void 							SetText(const Osp::Base::String& text);
	void 							SetText(const Osp::Base::String& text, Color textColor, int size=BASECONTROL_DEFAULT_HEIGHT, FontStyle fontStyle=FONT_STYLE_PLAIN);
	void 							SetBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap);
	void 							SetTextVerticalAlignment(VerticalAlignment alignment);
	void 							SetTextHorizontalAlignment(HorizontalAlignment alignment);
	virtual void 					SetBackgroundColor(const Osp::Graphics::Color& color);
	virtual void 					SetTextColor(const Osp::Graphics::Color& color);
	void 							SetTextConfig(int size, LabelTextStyle style);

	// added function
	void 							SetWrap(TextWrap wrap = TEXT_WRAP_CHARACTER_WRAP);

	void 							SetRound(const Dimension& arcDim) 		{ __dimArc = arcDim; };
	void 							SetRound(int x, int y) 					{ __dimArc.SetSize(x,y); };
	void 							SetMargin(const Point& marginPt) 		{ __ptMargin = marginPt; };
	void 							SetMargin(int x, int y) 				{ __ptMargin.SetPosition(x,y); };

	void 							SetBackgroundBitmap(const Osp::Graphics::Bitmap* pbitmap);
	void 							SetBackgroundImage(const Osp::Base::String & filename);

	// added function - style 에 NORMAL 주면 안되네 ...
	void 							SetTextSize(int height);
	void 							SetTextSize(const Osp::Base::String& text, int height);

public:
	// Container
	virtual result OnDraw(void);

protected:
	void 		draw(Canvas * pCanvas);

};

#endif /* BENNY_LABEL_H_ */
