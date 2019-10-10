/*
 * BaseContainer.cpp
 *
 *  Created on: 2012. 2. 14.
 *      Author: SONG
 */

#include "BaseContainer.h"
#include <FSystem.h>

using namespace Osp::System;

int 			BaseContainer::ScreenWidth = 0;
int 			BaseContainer::ScreenHeight = 0;

Font	* 		BaseContainer::__pFontNoraml = null;
Font	* 		BaseContainer::__pFontSmall = null;
Font	* 		BaseContainer::__pFontLarge = null;
Font	* 		BaseContainer::__pFontHuge = null;

BaseContainer::BaseContainer() : __pFontSpecific(null)
{
	if (BaseContainer::__pFontNoraml == null) {
		int width = 0, height = 0;
		SystemInfo::GetValue("ScreenWidth", width);
		SystemInfo::GetValue("ScreenHeight", height);
		BaseContainer::SetScreenSize(width, height);

		if (width == 0 || height == 0) {
			AppLog("Cannot Get Screen Width and Height information. They are %d and %d.", width, height);
			AppLog("You must add SYSTEM_SERVICE privilege in your manifest file!!");
		}
	}

	__nFontSize = BASECONTROL_DEFAULT_HEIGHT;
	__styleFont = FONT_STYLE_PLAIN;
}

BaseContainer::~BaseContainer()
{
	if (__pFontSpecific)
		delete __pFontSpecific;
}

void BaseContainer::SetScreenSize(int w, int h)
{
	BaseContainer::ScreenWidth = w;
	BaseContainer::ScreenHeight = h;

	if (BaseContainer::__pFontNoraml != null) {
		delete BaseContainer::__pFontNoraml;
	}

	BaseContainer::__pFontNoraml = new Font();
	BaseContainer::__pFontNoraml->Construct("Samsung font", FONT_STYLE_PLAIN, BASECONTROL_DEFAULT_HEIGHT);

	BaseContainer::__pFontSmall = new Font();
	BaseContainer::__pFontLarge = new Font();
	BaseContainer::__pFontHuge = new Font();

	BaseContainer::__pFontSmall->Construct("Samsung font", FONT_STYLE_PLAIN, BASECONTROL_HEIGHT_SMALL);
	BaseContainer::__pFontLarge->Construct("Samsung font", FONT_STYLE_PLAIN, BASECONTROL_HEIGHT_LARGE);
	BaseContainer::__pFontHuge->Construct("Samsung font", FONT_STYLE_PLAIN, BASECONTROL_HEIGHT_HUGE);
}

result BaseContainer::Construct(void)
{
	result r = Container::Construct();

	if (r != E_SUCCESS) {
		return r;
	}

	// font 생성!
	Font* pFont = new Font();
	//pFont->Construct(FONT_STYLE_PLAIN, BASECONTROL_DEFAULT_HEIGHT);
	pFont->Construct("Samsung font", FONT_STYLE_PLAIN, BASECONTROL_DEFAULT_HEIGHT);

	return E_SUCCESS;
}

Font * BaseContainer::GetFont(int size, FontStyle style)
{
	//if (size == BASECONTROL_DEFAULT_HEIGHT && style == FONT_STYLE_PLAIN)  // in case of default
	//	return BaseContainer::__pFontNoraml;

	__nFontSize = size;
	__styleFont = style;

	if (style == FONT_STYLE_PLAIN) {
		switch (size) {
		case BASECONTROL_HEIGHT_NORMAL:
			return BaseContainer::__pFontNoraml;
		case BASECONTROL_HEIGHT_SMALL:
			return BaseContainer::__pFontSmall;
		case BASECONTROL_HEIGHT_LARGE:
			return BaseContainer::__pFontLarge;
		case BASECONTROL_HEIGHT_HUGE:
			return BaseContainer::__pFontHuge;
		}
	}

	if (__pFontSpecific)
		delete __pFontSpecific;

	__pFontSpecific = new Font();
	__pFontSpecific->Construct("Samsung font", style, size);

	return __pFontSpecific;
}

Rectangle BaseContainer::RECT(const Rectangle & rect) const
{
	Rectangle  rectA;

	rectA.x = rect.x * BaseContainer::ScreenWidth / 480;
	rectA.y = rect.y * BaseContainer::ScreenHeight / 800;
	rectA.width = rect.width * BaseContainer::ScreenWidth / 480;
	rectA.height = rect.height * BaseContainer::ScreenHeight / 800;

	return rectA;
}

result BaseContainer::SetBounds(const Osp::Graphics::Rectangle& rect)
{
	return Container::SetBounds(RECT(rect));
}

