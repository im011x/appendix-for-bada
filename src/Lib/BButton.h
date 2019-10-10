/*
 * BButton.h
 *
 *  Created on: 2012. 2. 15.
 *      Author: SONG
 */

#ifndef BENNY_BUTTON_H_
#define BENNY_BUTTON_H_

#include "BLabel.h"
#include "BCanAccssibility.h"

class _EXPORT_ BButton
	: public BLabel
	, public Osp::Ui::ITouchEventListener
	, public Osp::Ui::IDragDropEventListener
{
public:
	BButton();
	virtual ~BButton();

protected:
	IActionEventListener 	* 		__pActionHandler;
	int 							__nActionID;

	bool 			__bStay;  // 버튼을 토글 기능으로 사용할 것 인가? 말것인가? 즉 선택 상태 고정 기능 사용여부
	bool 			__bSelected;
	bool 			__bPressed;

	Point 			__ptPressedMargin;
	//
	Osp::Graphics::Color 		__colorNoraml;
	Osp::Graphics::Color 		__colorPressed;
	Osp::Graphics::Color 		__colorDisabled;
	Osp::Graphics::Color 		__colorHighlighted;
	Osp::Graphics::Color 		__colorBorderOnNoImage;

	Osp::Graphics::Point*		__pNormalBitmapPosition;
	Osp::Graphics::Bitmap*		__pNormalBitmap;
	Osp::Graphics::Point*		__pPressedBitmapPosition;
	Osp::Graphics::Bitmap*		__pPressedBitmap;
	Osp::Graphics::Point*		__pDisabledBitmapPosition;
	Osp::Graphics::Bitmap*		__pDisabledBitmap;

	Osp::Graphics::Point*		__pNormalBitmapPosition2;
	Osp::Graphics::Bitmap*		__pNormalBitmap2;
	Osp::Graphics::Point*		__pPressedBitmapPosition2;
	Osp::Graphics::Bitmap*		__pPressedBitmap2;
	Osp::Graphics::Point*		__pDisabledBitmapPosition2;
	Osp::Graphics::Bitmap*		__pDisabledBitmap2;

	Osp::Graphics::Bitmap*		__pNormalBackgroundBitmap;
	Osp::Graphics::Bitmap*		__pPressedBackgroundBitmap;
	Osp::Graphics::Bitmap*		__pHighlightedBackgroundBitmap;

	// 버튼의 감도
	static 		int 			__ButtonSensitivity; // 0: 일반, 1:조금 높임, 2 : 아주 높임! 즉 TrouchReleased() 이외의 부분에서도 처리 여부

	static 		BCanAccssibility *	__pButtonTTS;
	bool 							__bPressedLongTouch;

	String 						__textSpeech;


public:
	result Construct(const Osp::Graphics::Rectangle& rect, const Osp::Base::String& text, int size=BASECONTROL_DEFAULT_HEIGHT, const Osp::Graphics::Color& color=Color::COLOR_BLACK,
			HorizontalAlignment alignHorz=ALIGNMENT_CENTER, VerticalAlignment alignVert=ALIGNMENT_MIDDLE, FontStyle fontStyle=FONT_STYLE_PLAIN);

	// 버튼이 선택되었음을 유지하는 기능
	void 	SetStaySelection(bool b)  	{  __bStay = b; };
	bool 	GetStaySelection()  		{  return __bStay; };

	// 현재 버튼 상태가 선택인지 아닌지 여부
	void 	SetSelection(bool b) 	{ __bSelected = b; };
	bool 	GetSelected() 			{ return __bSelected; };

	void 	SetPressedMargin(const Point& marginPt) 		{ __ptPressedMargin = marginPt; };
	void 	SetPressedMargin(int x, int y) 					{ __ptPressedMargin.SetPosition(x,y); };

	void 	SetSensitivityHighest();
	void 	SetSensitivityHigh();
	void 	SetSensitivityNormal();

private:
	void 	set_position_bitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap, Point **pDestPoint, Bitmap **pDestBitmap);
	void 	fire(int nSensitivity);

public:
	// Button functions
	void AddActionEventListener(const Osp::Ui::IActionEventListener& listener);
	void AddActionEventListener(Osp::Ui::IActionEventListener* plistener);
	void SetActionId(int actionId)  {  __nActionID = actionId;  };
	int  GetActionId(void) const 	{ return __nActionID; };

	void SetPressedTextColor(const Osp::Graphics::Color& color) 	{ __colorPressed = color; };
	Osp::Graphics::Color GetPressedTextColor(void) const 			{ return __colorPressed;  };
	void SetDisabledTextColor(const Osp::Graphics::Color& color) 	{ __colorDisabled = color; };
	Osp::Graphics::Color GetDisabledTextColor(void) const 			{ return __colorDisabled;  };
	void SetHighlightedTextColor(const Osp::Graphics::Color& color) { __colorHighlighted = color; };
	Osp::Graphics::Color GetHighlightedTextColor(void) const 		{ return __colorHighlighted;  };

	void SetNormalBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);
	void SetPressedBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);
	void SetDisabledBitmap(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);

	void SetNormalBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap);
	void SetPressedBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap);
	void SetHighlightedBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap);

	result SetColor(ButtonStatus status, const Osp::Graphics::Color& color);
	Osp::Graphics::Color GetColor(ButtonStatus status) const;

	// BButton ONLY !!!
	void SetNormalTextColor(const Osp::Graphics::Color& color);  // __colorNoraml - 일반 text Color 설정외에 반전색으로 __colorPressed 자동 설정한다.
	Osp::Graphics::Color GetNormalTextColor(void) const 		{ return __colorNoraml;  };
	//
	void SetBackgroundBitmap(const Osp::Graphics::Bitmap& bitmap) { SetNormalBackgroundBitmap(bitmap); };
	void SetForegroundColor(const Osp::Graphics::Color& color)	{ __colorBorderOnNoImage = color; };  // 버튼에 이미지 없는 경우 테두리 색
	//
	void SetNormalBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);
	void SetPressedBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);
	void SetDisabledBitmap2(const Osp::Graphics::Point& position, const Osp::Graphics::Bitmap& bitmap);

	//
	void SetSpeechText(const String & speech) 	{ __textSpeech = speech; };

public:
	// ITouchEventListener
	virtual void OnTouchPressed(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo & touchInfo);
	virtual void OnTouchReleased(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchFocusIn(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchFocusOut(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchMoved(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo);
	virtual void OnTouchLongPressed(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo);

	virtual void OnTouchDoublePressed(const Osp::Ui::Control& source, const Osp::Graphics::Point& currentPosition, const Osp::Ui::TouchEventInfo& touchInfo) {};

	//
	virtual void OnTouchDragged(const Osp::Ui::Control& source, const Osp::Graphics::Point& startPosition, const Osp::Graphics::Point& currentPosition);
	virtual void OnTouchDropped(const Osp::Ui::Control& source, const Osp::Graphics::Point& startPosition, const Osp::Graphics::Point& currentPosition);

	virtual void OnFocusGained(const Osp::Ui::Control& source);
	virtual void OnFocusLost(const Osp::Ui::Control& source);


public:
	// Container
	virtual result OnDraw(void);

};

#endif /* BENNY_BUTTON_H_ */
