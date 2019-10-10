/*
 * ControlMaker.cpp
 *
 *  Created on: 2012. 4. 6.
 *      Author: SONG
 */

#include "ControlMaker.h"
#include <FApp.h>

using namespace Osp::App;
using namespace Osp::Web::Controls;

Osp::Base::Collection::HashMap *		ControlMaker::__PBitmapPool = null;  // A

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BcContextItem::BcContextItem(const String & name, int value)
{
	__addType = 0;

	__showText = name;
	__actionId = value;
}

BcContextItem::BcContextItem(int value, const String & normal, const String & pressed, const String & hightlight)
{
	__addType = 1;

	__actionId = value;
	__imageNormal = normal;
	__imagePressed = pressed;
	__imageHighlight = hightlight;
}

BcContextItem::BcContextItem(const String & name, int value, const String & normal, const String & pressed, const String & hightlight)
{
	__addType = 2;

	__showText = name;
	__actionId = value;

	__imageNormal = normal;
	__imagePressed = pressed;
	__imageHighlight = hightlight;
}



/*
BCContextMenu::BCContextMenu()
{
	contextType = 0;
}

BCContextMenu::~BCContextMenu()
{
}

void BCContextMenu::SetContext(const String & name, int value)
{
	showText = name;
	actionId = value;
}

//

BCContextMenuBitmap::BCContextMenuBitmap(int type)
{
	contextType = type;
}

BCContextMenuBitmap::~BCContextMenuBitmap()
{

}

void BCContextMenuBitmap::SetContext(const String & name1, const String & name2, const String & name3, int value)
{
	nameBitmapNormal = name1;
	nameBitmapPressed = name2;
	nameBitmapHighlight = name3;
	actionId = value;
}

void BCContextMenuBitmap::SetContext(const String & name1, const String & name2, const String & name3, const String & name, int value)
{
	nameBitmapNormal = name1;
	nameBitmapPressed = name2;
	nameBitmapHighlight = name3;
	showText = name;
	actionId = value;
}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

ControlMaker::ControlMaker()
{
}

ControlMaker::~ControlMaker()
{
}

void ControlMaker::INIT_Pool()
{
	if (__PBitmapPool) {
		delete __PBitmapPool;
	}

	__PBitmapPool = new Osp::Base::Collection::HashMap();
	__PBitmapPool->Construct(100);
}

void ControlMaker::FINI_Pool()
{
	if (__PBitmapPool) {
		__PBitmapPool->RemoveAll(true);
		delete __PBitmapPool;
		__PBitmapPool = null;
	}
}

MyButton * ControlMaker::makeButton(Rectangle rect, const String & showText, Container * bottom, int actionId, IActionEventListener * listener)
{
	MyButton * pbutton = new MyButton();
	pbutton->Construct(rect, showText);

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	return pbutton;
}

Bitmap * ControlMaker::loadBitmapN(const String & imagePath)
{
	//AppLogDebug("ControlMaker::loadBitmapN - %S", imagePath.GetPointer());
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	return pAppResource->GetBitmapN(imagePath);
}

MyButton * ControlMaker::makeButton(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener)
{
	MyButton * pbutton = new MyButton();
	pbutton->Construct(rect, showText);

	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapN(imageNormal);
		if (pbmp) {
			pbutton->SetNormalBackgroundBitmap(*pbmp);
			delete pbmp;
		}
	}
	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapN(imagePressed);
		if (pbmp) {
			pbutton->SetPressedBackgroundBitmap(*pbmp);
			delete pbmp;
		}
	}

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	return pbutton;
}

MyButton * 	ControlMaker::makeButtonImageText(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, const Osp::Graphics::Color& color,
		Container * bottom, int actionId, IActionEventListener * listener)
{
	MyButton * pbutton = new MyButton();
	pbutton->Construct(rect, showText, 25, color);

	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapN(imageNormal);
		if (pbmp) {
			pbutton->SetNormalBackgroundBitmap(*pbmp);
			delete pbmp;
		}
	}
	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapN(imagePressed);
		if (pbmp) {
			pbutton->SetPressedBackgroundBitmap(*pbmp);
			delete pbmp;
		}
	}

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	return pbutton;
}

Bitmap * ControlMaker::loadBitmapPool(const String & imagePath)
{
	AppAssert(__PBitmapPool);

	bool bExist = false;
	Osp::Graphics::Bitmap* pbitmap =  null;

	__PBitmapPool->ContainsKey(imagePath, bExist);

	if (bExist) {
		pbitmap =  static_cast <Osp::Graphics::Bitmap*> (__PBitmapPool->GetValue (imagePath));
	}

	if (pbitmap == null) {
		pbitmap = loadBitmapN(imagePath);

		if (pbitmap != null) {
			String * pNewName = new String(imagePath);
			__PBitmapPool->Add(*pNewName, *pbitmap);
		}
	}

	return pbitmap;
}

MyButton * ControlMaker::makeButtonPool(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener)
{
	MyButton * pbutton = new MyButton();
	pbutton->Construct(rect, showText);

	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapPool(imageNormal);
		if (pbmp) {
			pbutton->SetNormalBackgroundBitmap(*pbmp);
			//delete pbmp;  // A
		}
	}
	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapPool(imagePressed);
		if (pbmp) {
			pbutton->SetPressedBackgroundBitmap(*pbmp);
			//delete pbmp;  // A
		}
	}

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	return pbutton;
}

MyButton * ControlMaker::makeStateButton(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener, bool selected)
{
	MyButton * pbutton = new MyButton();
	pbutton->Construct(rect, showText);

	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapPool(imageNormal);
		if (pbmp) {
			pbutton->SetNormalBackgroundBitmap(*pbmp);
		}
	}
	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapPool(imagePressed);
		if (pbmp) {
			pbutton->SetPressedBackgroundBitmap(*pbmp);
			pbutton->SetHighlightedBackgroundBitmap(*pbmp);
		}
	}

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	pbutton->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pbutton->SetMargin(70, 0);

	pbutton->SetStaySelection(true);
	pbutton->SetSelection(selected);

	return pbutton;
}

MyButton * ControlMaker::makeButton(Rectangle rect,
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
		)
{
	BButton * pbutton = new BButton();
	pbutton->Construct(rect, showText, (int) textHeight, textColor, alignHorz, alignVert, fontStyle);
	pbutton->SetMargin(marginText);

	// background image
	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapPool(imageNormal);
		if (pbmp) {
			pbutton->SetNormalBackgroundBitmap(*pbmp);
		}
	}

	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapPool(imagePressed);
		if (pbmp) {
			pbutton->SetPressedBackgroundBitmap(*pbmp);
		}
	}

	// foreground image
	if (normalFG != "") {
		Bitmap * pbmp = loadBitmapPool(normalFG);
		if (pbmp) {
			pbutton->SetNormalBitmap(marginFG, *pbmp);
		}
	}

	if (pressedFG != "") {
		Bitmap * pbmp = loadBitmapPool(pressedFG);
		if (pbmp) {
			pbutton->SetPressedBitmap(marginFG, *pbmp);
		}
	}

	//
	if (normalFG2 != "") {
		Bitmap * pbmp = loadBitmapPool(normalFG2);
		if (pbmp) {
			pbutton->SetNormalBitmap2(marginFG2, *pbmp);
		}
	}

	if (pressedFG2 != "") {
		Bitmap * pbmp = loadBitmapPool(pressedFG2);
		if (pbmp) {
			pbutton->SetPressedBitmap2(marginFG2, *pbmp);
		}
	}

	if (bottom) {
		bottom->AddControl(*pbutton);
	}

	if (listener) {
		pbutton->SetActionId(actionId);
		pbutton->AddActionEventListener(*listener);
	}

	return pbutton;
}

MyButton * ControlMaker::makeButtonX(Rectangle rect, const String & imageNormal, const String & imagePressed, const String & showText, Container * bottom, int actionId, IActionEventListener * listener)
{
	return makeButton(rect
			//, "", ""
			, "appendix/transparent.png", "appendix/transparent.png"
			, bottom, showText
			//, imageNormal, imagePressed
			, "appendix/transparent.png", "appendix/transparent.png"
			, Point(10,10)
			, "", "", Point(0,0)
			, actionId, listener, TEXT_HEIGHT_LARGE
			, Point(110, 0), Color(0,0,0)
			, ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, FONT_STYLE_PLAIN);
}

MyButton * ControlMaker::makeStateButtonX(Rectangle rect, const String & showText, Container * bottom, int actionId, IActionEventListener * listener, bool selected)
{
	MyButton * pbutton = makeButton(rect
			, "appendix/transparent.png", "appendix/transparent.png"
			, bottom, showText
			, "appendix/check_no.png", "appendix/check_yes.png"
			, Point(0,0)
			, "", "", Point(0,0)
			, actionId, listener, TEXT_HEIGHT_LARGE
			, Point(70, 0), Color(0,0,0)
			, ALIGNMENT_LEFT, ALIGNMENT_MIDDLE, FONT_STYLE_PLAIN);

	pbutton->SetTextHorizontalAlignment(ALIGNMENT_LEFT);
	pbutton->SetMargin(70, 0);

	pbutton->SetStaySelection(true);
	pbutton->SetSelection(selected);

	return pbutton;
}


MyLabel * ControlMaker::makeLabelText(Rectangle rect, const String & showText, Container * bottom, int textSize, const Color& textColor, const Color& backgroundColor, HorizontalAlignment horizonAlign, VerticalAlignment verticalAlign)
{
	MyLabel * pLabel = new MyLabel();
	pLabel->Construct(rect, showText);

	pLabel->SetTextConfig(textSize, LABEL_TEXT_STYLE_NORMAL);
	pLabel->SetTextColor(textColor);
	pLabel->SetBackgroundColor(backgroundColor);
	pLabel->SetTextHorizontalAlignment(horizonAlign);
	pLabel->SetTextVerticalAlignment(verticalAlign);

	if (bottom)
		bottom->AddControl(*pLabel);

	return pLabel;
}

MyLabel * ControlMaker::makeLabelTextLeft(Rectangle rect, const String & showText, Container * bottom, int textSize, const Color& textColor, const Color& backgroundColor, const VerticalAlignment verticalAlign)
{
	return makeLabelText(rect, showText, bottom, textSize, textColor, backgroundColor, ALIGNMENT_LEFT, verticalAlign);
}

MyLabel * ControlMaker::makeLabelTextRight(Rectangle rect, const String & showText, Container * bottom, int textSize, const Color& textColor, const Color& backgroundColor, const VerticalAlignment verticalAlign)
{
	return makeLabelText(rect, showText, bottom, textSize, textColor, backgroundColor, ALIGNMENT_RIGHT, verticalAlign);
}

MyLabel * ControlMaker::makeLabelImage(Rectangle rect, const String & imageName, Container * bottom, bool keepBitmap)
{
	MyLabel* plabel = new MyLabel();
	plabel->Construct(rect, "");

	if (imageName != "") {
		if (keepBitmap) {
			Bitmap * pbmp = loadBitmapPool(imageName);
			if (pbmp) {
				plabel->SetBackgroundBitmap(*pbmp);
			}
		}
		else {
			Bitmap * pbmp = loadBitmapN(imageName);
			if (pbmp) {
				plabel->SetBackgroundBitmap(*pbmp);
				delete pbmp;
			}
		}
	}

	if (bottom) {
		bottom->AddControl(*plabel);
	}

	return plabel;
}

Panel * ControlMaker::makePanel(Rectangle rect, Container * bottom, const String & imageName, bool keepBitmap)
{
	Panel * pPanel = new Panel();
	pPanel->Construct(rect);

	if (bottom)
		bottom->AddControl(*pPanel);

	if (imageName != "") {
		makeLabelImage(Rectangle(0,0,pPanel->GetWidth(), pPanel->GetHeight()), imageName, pPanel, keepBitmap);
	}

	return pPanel;
}

ScrollPanel * ControlMaker::makeScrollPanel(Rectangle rect, Container * bottom, const String & imageName, bool keepBitmap)
{
	ScrollPanel * pPanel = new ScrollPanel();
	pPanel->Construct(rect);

	if (bottom)
		bottom->AddControl(*pPanel);

	if (imageName != "") {
		makeLabelImage(Rectangle(0,0,pPanel->GetWidth(), pPanel->GetHeight()), imageName, pPanel, keepBitmap);
	}

	return pPanel;
}

ListView * ControlMaker::makeListView(Rectangle rect, Container * bottom, IListViewItemProvider * pItemProvider, IListViewItemEventListener * pListHandler, bool itemDivider, IScrollEventListener * pScroller)
{
	ListView * pList = new ListView();

	if (bottom) {
		Panel * btm = new Panel();
		btm->Construct(rect);
		bottom->AddControl(*btm);

		rect.x = 0;
		rect.y = 0;
		pList->Construct(rect, itemDivider, true);
		btm->AddControl(*pList);
	}

	if (pItemProvider)
		pList->SetItemProvider(*pItemProvider);

	if (pListHandler)
		pList->AddListViewItemEventListener(*pListHandler);

	if (pScroller)
		pList->AddScrollEventListener(*pScroller);

	pList->SetTextOfEmptyList("Appendix - 조회중입니다.");
	pList->SetSweepEnabled(true);

	return pList;
}

GroupedListView * ControlMaker::makeGroupedListView(Rectangle rect, Form * bottom, IGroupedListViewItemProvider * pItemProvider, IGroupedListViewItemEventListener * pListHandler, bool itemDivider, IScrollEventListener * pScroller)
{
	GroupedListView * pList = new GroupedListView();

	/*
	if (bottom) {
		Panel * btm = new Panel();
		btm->Construct(rect);
		bottom->AddControl(*btm);

		rect.x = 0;
		rect.y = 0;
		pList->Construct(rect, GROUPED_LIST_VIEW_STYLE_INDEXED , itemDivider, true);
		btm->AddControl(*pList);
	}
	 */

	if (bottom) {
		pList->Construct(rect, GROUPED_LIST_VIEW_STYLE_INDEXED, itemDivider, true);  //GROUPED_LIST_VIEW_STYLE_SECTION 혹은 GROUPED_LIST_VIEW_STYLE_INDEXED
		bottom->AddControl(*pList);
	}

	if (pItemProvider)
		pList->SetItemProvider(*pItemProvider);

	if (pListHandler)
		pList->AddGroupedListViewItemEventListener(*pListHandler);

	if (pScroller)
		pList->AddScrollEventListener(*pScroller);

	pList->SetTextOfEmptyList("Appendix - 조회중입니다.");

	pList->SetSweepEnabled(true);

	return pList;
}

ListContextItem * ControlMaker::makeContextMenu(ArrayList * pArr)
{
	ListContextItem * pContext = new ListContextItem();
	pContext->Construct();

	for (int i=0; i<pArr->GetCount(); i++) {
		BcContextItem* pItem = static_cast<BcContextItem*> (pArr->GetAt(i));

		switch (pItem->GetType()) {
		case 0:
			pContext->AddElement(pItem->__actionId, pItem->__showText);
			break;
		case 1:
			//pContext->AddElement(pItem->__actionId, pItem->__imageNormal, pItem->__imagePressed, pItem->__imageHighlight);
			break;
		case 2:
			//pContext->AddElement(pItem->__actionId, pItem->__showText, pItem->__imageNormal, pItem->__imagePressed, pItem->__imageHighlight);
			break;
		default:
			AppAssert(0);
			break;
		}

	}

	return pContext;
}

ButtonItem * ControlMaker::makeButtonItem(const String & imageNormal, const String & imagePressed, int actionId)
{
	Osp::Ui::Controls::ButtonItem* pbutton = new ButtonItem();

	pbutton->Construct(BUTTON_ITEM_STYLE_ICON, actionId);

	if (imageNormal != "") {
		Bitmap * pbmp = loadBitmapPool(imageNormal);
		if (pbmp) {
			pbutton->SetIcon(BUTTON_ITEM_STATUS_NORMAL, pbmp);
		}
	}

	if (imagePressed != "") {
		Bitmap * pbmp = loadBitmapPool(imagePressed);
		if (pbmp) {
			pbutton->SetIcon(BUTTON_ITEM_STATUS_PRESSED, pbmp);
		}
	}

	return pbutton;
}

ButtonItem * ControlMaker::makeButtonItem(const String & showText, int actionId)
{
	Osp::Ui::Controls::ButtonItem* pbutton = new ButtonItem();

	pbutton->Construct(BUTTON_ITEM_STYLE_TEXT, actionId);

	if (showText != "") {
		pbutton->SetText(showText);
	}

	return pbutton;
}

Web * ControlMaker::makeWebControl(Rectangle rect, Container * bottom, const String& url)
{
	Web* pWeb = new Web();
	pWeb->Construct(rect);
	if( bottom )
		bottom->AddControl(*pWeb);

	//pWeb->SetFocus();

	if (!url.IsEmpty())
		pWeb->LoadUrl(url);

	return pWeb;
}

Osp::Ui::Controls::SearchBar * ControlMaker::makeSearchBar(Rectangle rect, Container * bottom, Osp::Ui::Controls::ISearchBarEventListener * pSearchHandler, Osp::Ui::IKeypadEventListener *pKeypadHandler, KeypadAction keypadAction, const String& guideText)
{
	Osp::Ui::Controls::SearchBar * pBar = new Osp::Ui::Controls::SearchBar();

	pBar->Construct(rect, true, keypadAction);

	if( bottom ) {
		bottom->AddControl(*pBar);
	}

	if (pSearchHandler) {
		pBar->AddSearchBarEventListener(*pSearchHandler);
	}

	if (pKeypadHandler) {
		pBar->AddKeypadEventListener(*pKeypadHandler);
	}

	if (!guideText.IsEmpty()) {
		pBar->SetGuideText(guideText);
	}

	return pBar;
}

Osp::Ui::Controls::EditField * ControlMaker::makeEditField(Rectangle rect, EditFieldStyle style, const String & sGuide, Container * bottom,
		IActionEventListener * pCmdHandler, int maxChars, int nLeftBtnID, int nRightBtnID, const String & sLeftBtn, const String & sRightBtn,
		IScrollPanelEventListener * pScrollHandler, ITextEventListener * pTextHandler, InputStyle inStyle)
{
	Osp::Ui::Controls::EditField* pEditField = new EditField();
	pEditField->Construct(rect, style, inStyle, false, maxChars);

	if (bottom) {
		bottom->AddControl(*pEditField);
	}

	if (pScrollHandler)
		pEditField->AddScrollPanelEventListener(*pScrollHandler);

	if (pTextHandler)
		pEditField->AddTextEventListener(*pTextHandler);

	if (pCmdHandler)
		pEditField->AddActionEventListener(*pCmdHandler);

	if (nLeftBtnID > 0) {
		pEditField->SetOverlayKeypadCommandButton(COMMAND_BUTTON_POSITION_LEFT, sLeftBtn, nLeftBtnID);
	}
	if (nRightBtnID > 0) {
		pEditField->SetOverlayKeypadCommandButton(COMMAND_BUTTON_POSITION_RIGHT, sRightBtn, nRightBtnID);
	}

	pEditField->SetGuideText(sGuide);

	//
	pEditField->SetColor(EDIT_STATUS_NORMAL , Color::COLOR_WHITE);


	return pEditField;
}

Osp::Ui::Controls::Slider * ControlMaker::makeSlider(Rectangle rect, Container * bottom, int minValue, int maxValue,
		IAdjustmentEventListener * pAdjListener, ISliderEventListener * pSliderListener,
		const String & title, BackgroundStyle bgStyle)
{
	Osp::Ui::Controls::Slider* pSlider = new Osp::Ui::Controls::Slider();
	pSlider->Construct(rect, bgStyle, false, minValue, maxValue, GROUP_STYLE_NONE);

	if (bottom) {
		bottom->AddControl(*pSlider);
	}

	if (pAdjListener) {
		pSlider->AddAdjustmentEventListener(*pAdjListener);
	}

	if (pSliderListener) {
		pSlider->AddSliderEventListener(*pSliderListener);
	}

	if (!title.IsEmpty()) {
		pSlider->SetTitleText(title);
	}

	return pSlider;
}

Osp::Ui::Controls::Progress * ControlMaker::makeProgress(Rectangle rect, Container * bottom, int minValue, int maxValue, int currValue)
{
	Osp::Ui::Controls::Progress * pProgress = new Osp::Ui::Controls::Progress();
	pProgress->Construct(rect, minValue, maxValue);
	pProgress->SetValue(currValue);

	if (bottom) {
		bottom->AddControl(*pProgress);
	}

	return pProgress;
}


TextBox * ControlMaker::makeTextBox(Rectangle rect, const String & showText, Container * bottom,
		const Color& textColor, const Color& backgroundColor, int textSize,
		HorizontalAlignment horizonAlign, VerticalAlignment verticalAlign, bool bRoundBorder,
		bool bFontBold)
{
	TextBox* pTextBox = new TextBox();
	pTextBox->Construct(rect, (bRoundBorder ? TEXT_BOX_BORDER_ROUNDED : TEXT_BOX_BORDER_NONE));
	pTextBox->SetEnabled(false);
	pTextBox->SetTextColor(TEXT_BOX_TEXT_COLOR_NORMAL, textColor);
	pTextBox->SetTextColor(TEXT_BOX_TEXT_COLOR_HIGHLIGHTED, textColor);
	pTextBox->SetTextColor(TEXT_BOX_TEXT_COLOR_DISABLED, textColor);
	pTextBox->SetTextColor(TEXT_BOX_TEXT_COLOR_BLOCKED, textColor);
	pTextBox->SetTextColor(TEXT_BOX_TEXT_COLOR_LINK, textColor);
	pTextBox->SetColor(TEXT_BOX_STATUS_NORMAL, backgroundColor);
	pTextBox->SetColor(TEXT_BOX_STATUS_HIGHLIGHTED, backgroundColor);
	pTextBox->SetColor(TEXT_BOX_STATUS_DISABLED, backgroundColor);
	int style = FONT_STYLE_PLAIN;
	if( bFontBold )
		style |= FONT_STYLE_BOLD;
	Font font;
	font.Construct(L"Samsung font", style, textSize);
	pTextBox->SetFont(font);
	pTextBox->SetText(showText);
	pTextBox->SetTextSize(textSize);
	bottom->AddControl(*pTextBox);

	return pTextBox;
}

TabBar * ControlMaker::makeTabBar(Point startXY, Container * bottom, IActionEventListener * listener)
{
	TabBar * pTabbar = new TabBar();

	pTabbar->Construct(startXY.x, startXY.y, 480);

	if (bottom) {
		bottom->AddControl(*pTabbar);
	}

	if (listener) {
		pTabbar->AddActionEventListener(*listener);
	}

	return pTabbar;
}

bool ControlMaker::makeTabbarItem(TabBar * pTabbar, const String & title, int actionId)
{
	TabBarItem item;
	item.Construct(title, actionId);
	pTabbar->AddItem(item);

	return true;
}
