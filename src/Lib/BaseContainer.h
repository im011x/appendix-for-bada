/*
 * BaseContainer.h
 *
 *  Created on: 2012. 2. 14.
 *      Author: SONG
 */

#ifndef BENNY_BASECONTAINER_H_
#define BENNY_BASECONTAINER_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>

using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Runtime;



/*
	다음과 같은 권한이 반드시 필요합니다.

<Privilege>
    <Name>SYSTEM_SERVICE</Name>
</Privilege>

*/



class BaseContainer : public Osp::Ui::Container
{
public:
	BaseContainer();
	virtual ~BaseContainer();

private:
	static int 		ScreenWidth;
	static int 		ScreenHeight;

protected:
	static Font	* 		__pFontNoraml;
	static Font	* 		__pFontSmall;
	static Font	* 		__pFontLarge;
	static Font	* 		__pFontHuge;
	//
	Font	* 			__pFontSpecific;
	int 				__nFontSize;
	FontStyle	 		__styleFont;

public:
	static 	const int 	BASECONTROL_DEFAULT_HEIGHT = 30;
	static 	const int 	BASECONTROL_HEIGHT_NORMAL = BASECONTROL_DEFAULT_HEIGHT;
	static 	const int 	BASECONTROL_HEIGHT_SMALL = 27;
	static 	const int 	BASECONTROL_HEIGHT_LARGE = 33;
	static 	const int 	BASECONTROL_HEIGHT_HUGE  = 37;

public:
	static void 	SetScreenSize(int w, int h);

protected:
	result 			Construct(void);
	Font	* 		GetFont(int size, FontStyle style = FONT_STYLE_PLAIN);
	int 			GetFontSize() const		{  return __nFontSize; };
	FontStyle	 	GetFontStyle() const	{  return __styleFont; };

public:
	Rectangle 	RECT(const Rectangle & rect) const;
	virtual result SetBounds(const Osp::Graphics::Rectangle& rect);

};

#endif /* BENNY_BASECONTAINER_H_ */
