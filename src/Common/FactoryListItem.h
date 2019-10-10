/*
 * FactoryListItem.h
 *
 *  Created on: 2012. 7. 10.
 *      Author: benny
 */

#ifndef FACTORYLISTITEM_H_
#define FACTORYLISTITEM_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>
#include "BLabel.h"
#include "DataBusSeoul.h"
#include "DataAddr.h"
#include "DataCurrency.h"
#include "DatabasePhonebook.h"
#include "DatabaseAddress.h"

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;


class FactoryListItem {
public:
	FactoryListItem();
	virtual ~FactoryListItem();

private:
	static BLabel * makeBLabel(Rectangle rect, const String & showText, int textSize, const Color& textColor, const Color& backgroundColor, HorizontalAlignment horizonAlign, VerticalAlignment verticalAlign, LabelTextStyle textStyle, TextWrap wrap = TEXT_WRAP_NONE);

protected:
	static bool attachRichText(CustomItem* pTo, int nTo,
			Rectangle rect, const String & text, int sizeText=30, const Color& textColor=Color::COLOR_BLACK, const Color& backgroundColor=Color(0,0,0,0), HorizontalAlignment horizonAlign=ALIGNMENT_LEFT, VerticalAlignment verticalAlign=ALIGNMENT_MIDDLE, LabelTextStyle textStyle=LABEL_TEXT_STYLE_NORMAL, TextWrap wrap = TEXT_WRAP_NONE);

public:
	static CustomItem * makeListItem_String(int index, int itemWidth, const String & sText, ListAnnexStyle style);
	static CustomItem * makeListItem_String2(int index, int itemWidth, const String & sHead, const String & sText, ListAnnexStyle style);

	// 노선 정보
	static CustomItem * makeListItem_StopArriveInfo(int index, int itemWidth, BusSeoul_ArrivingInfo * pArriveInfo, Bitmap * pbmpBus);
	static CustomItem * makeListItem_StopLinesInfo(int index, int itemWidth, BusSeoul_LineDetail * pinfo);
	static CustomItem * makeListItem_BusLine(int index, int itemWidth,  BusSeoul_Line* pinfo, bool nShowType);

	// 정류소 정보
	static CustomItem * makeListItem_BusStopByLine(int index, int itemWidth,  BusSeoul_StopByLine* pinfo, Bitmap * pbmp, Bitmap * pbmpBus);
	static CustomItem * makeListItem_StopNear(int index, int itemWidth,  BusSeoul_StopInfo* pInfo, bool showDistance = true);
	static CustomItem * makeListItem_StopInfo(int index, int itemWidth,  BusSeoul_StopInfo* pInfo);

	// 버스도착정보
	static CustomItem * makeListItem_LineArriveInfo(int index, int itemWidth, BusSeoul_ArrivingByLine * pArriveInfo, Bitmap * pbmpBus);

	// 주소
	static CustomItem * makeListItem_Address(int index, int itemWidth,  DataAddr* pinfo);
	static CustomItem * makeListItem_AddressFreq(int index, int itemWidth,  AddressItem* pinfo);

	static CustomItem * makeListItem_Currency(int index, int itemWidth,  DataCurrency* pinfo);

	static CustomItem * makeListItem_PublicPhoneCategory(int index, int itemWidth,  String* pstr);
	static CustomItem * makeListItem_PublicPhones(int index, int itemWidth,  PhoneItem* pinfo);

};

#endif /* FACTORYLISTITEM_H_ */
