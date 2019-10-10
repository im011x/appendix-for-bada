/*
 * ControlMaker.h
 *
 *  Created on: 2012. 4. 6.
 *      Author: SONG
 */

#ifndef CONTROLMAKER_H_
#define CONTROLMAKER_H_

#include <FBase.h>
#include <FUi.h>
#include <FWeb.h>
#include "BButton.h"
#include "BLabel.h"
#include "BCanAccssibility.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

typedef 	BButton 		MyButton;
typedef 	BLabel  		MyLabel;


typedef enum
{
	TEXT_HEIGHT_MICRO  = 20,
	TEXT_HEIGHT_SMALL  = 25,
	TEXT_HEIGHT_NORMAL = 30,
	TEXT_HEIGHT_LARGE  = 35,
	TEXT_HEIGHT_HUGE   = 40
} TextHeight;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class BcContextItem : public Object
{
public:
	virtual ~BcContextItem() {};
	BcContextItem(const String & name, int value);
	BcContextItem(int value, const String & normal, const String & pressed="", const String & hightlight="");
	BcContextItem(const String & name, int value, const String & normal, const String & pressed="", const String & hightlight="");

public:
	int 		__addType;
	int 		__actionId;
	String 		__showText;
	String 		__imageNormal;
	String 		__imagePressed;
	String 		__imageHighlight;

public:
	int 	GetType()  {  return __addType; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class ControlMaker : public BCanAccssibility
{
public:
	ControlMaker();
	virtual ~ControlMaker();

protected:
	static 	Osp::Base::Collection::HashMap *		__PBitmapPool;

public:
	static void 		INIT_Pool();
	static void 		FINI_Pool();

protected:
	Bitmap * 	loadBitmapN(const String & imagePath);
	Bitmap * 	loadBitmapPool(const String & imagePath);

public:
	MyButton * 	makeButton(Rectangle rect, const String & showText, Container * bottom, int actionId, IActionEventListener * listener);
	MyButton * 	makeButton(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener);
	MyButton * 	makeButtonPool(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener);
	MyButton * 	makeStateButton(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener, bool selected = false);

	MyButton * 	makeButtonImageText(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, const Osp::Graphics::Color& color,
			Container * bottom, int actionId, IActionEventListener * listener);

	MyButton *  makeButton(Rectangle rect,
			const String & imageNormal, const String & imagePressed,
			Container * bottom,
			const String & showText,
			const String & normalFG, const String & pressedFG,
			Point marginFG,
			const String & normalFG2, const String & pressedFG2,
			Point marginFG2,
			int actionId, IActionEventListener * listener,
			TextHeight textHeight,
			Point marginText,
			Color textColor,
			HorizontalAlignment alignHorz, VerticalAlignment alignVert, FontStyle fontStyle
			);

	MyButton * 	makeButtonX(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener);
	MyButton * 	makeStateButtonX(Rectangle rect, const String & showText, Container * bottom, int actionId, IActionEventListener * listener, bool selected = false);



	MyLabel * 	makeLabelText(Rectangle rect, const String & showText, Container * bottom,
								int textSize=30, const Color& textColor=Color::COLOR_BLACK, const Color& backgroundColor=Color(0,0,0,0),
								const HorizontalAlignment horizonAlign = ALIGNMENT_CENTER, const VerticalAlignment verticalAlign = ALIGNMENT_MIDDLE);
	MyLabel * 	makeLabelTextLeft(Rectangle rect, const String & showText, Container * bottom,
								int textSize=30, const Color& textColor=Color::COLOR_BLACK, const Color& backgroundColor=Color(0,0,0,0),
								const VerticalAlignment verticalAlign = ALIGNMENT_MIDDLE);
	MyLabel * 	makeLabelTextRight(Rectangle rect, const String & showText, Container * bottom,
								int textSize=30, const Color& textColor=Color::COLOR_BLACK, const Color& backgroundColor=Color(0,0,0,0),
								const VerticalAlignment verticalAlign = ALIGNMENT_MIDDLE);
	MyLabel * 	makeLabelImage(Rectangle rect, const String & imageName, Container * bottom, bool keepBitmap = true);



	Panel * 	makePanel(Rectangle rect, Container * bottom, const String & imageName = "", bool keepBitmap = false);
	ScrollPanel * makeScrollPanel(Rectangle rect, Container * bottom, const String & imageName = "", bool keepBitmap = false);

	//
	ListView * makeListView(Rectangle rect, Container * bottom, IListViewItemProvider * pItemProvider = null, IListViewItemEventListener * pListHandler = null, bool itemDivider = true, IScrollEventListener * pScroller=null);
	GroupedListView * makeGroupedListView(Rectangle rect, Form * bottom, IGroupedListViewItemProvider * pItemProvider, IGroupedListViewItemEventListener * pListHandler, bool itemDivider, IScrollEventListener * pScroller=null);
	ListContextItem * makeContextMenu(ArrayList * pArr);

	//
	ButtonItem * makeButtonItem(const String & imageNormal, const String & imagePressed, int actionId);
	ButtonItem * makeButtonItem(const String & showText, int actionId);

	//
	Osp::Web::Controls::Web * makeWebControl(Rectangle rect, Container * bottom, const String& url="");

	Osp::Ui::Controls::SearchBar * 	makeSearchBar(Rectangle rect, Container * bottom,
			Osp::Ui::Controls::ISearchBarEventListener * pSearchHandler, Osp::Ui::IKeypadEventListener *pKeypadHandler,
			KeypadAction keypadAction, const String& guideText="");

	Osp::Ui::Controls::EditField* makeEditField(Rectangle rect, EditFieldStyle style = EDIT_FIELD_STYLE_NORMAL, const String & sGuide = "",
			Container * bottom=null, IActionEventListener * pCmdHandler=null, int maxChars = 10,
			int nLeftBtnID = 0, int nRightBtnID = 0, const String & sLeftBtn = "취소", const String & sRightBtn = "확인",
			IScrollPanelEventListener * pScrollHandler=null, ITextEventListener * pTextHandler=null, InputStyle inStyle = INPUT_STYLE_OVERLAY);

	Osp::Ui::Controls::Slider * makeSlider(Rectangle rect, Container * bottom=null, int minValue=0, int maxValue=100,
			IAdjustmentEventListener * pAdjListener = null, ISliderEventListener * pSliderListener = null,
			const String & title = "", BackgroundStyle bgStyle = BACKGROUND_STYLE_NONE);

	Osp::Ui::Controls::Progress * makeProgress(Rectangle rect, Container * bottom=null, int minValue=0, int maxValue=100, int currValue=0);

	TextBox * makeTextBox(Rectangle rect, const String & showText, Container * bottom,
			const Color& textColor = Color(255,255,255), const Color& backgroundColor = Color(0,0,0,0),
			int textSize=30, HorizontalAlignment horizonAlign = ALIGNMENT_LEFT,
			VerticalAlignment verticalAlign = ALIGNMENT_TOP, bool bRoundBorder = true, bool bFontBold = false);

	TabBar * makeTabBar(Point startXY, Container * bottom, IActionEventListener * listener);
	bool 	 makeTabbarItem(TabBar * pTabbar, const String & title, int actionId);

};

#endif /* CONTROLMAKER_H_ */
