/*
 * GroupBusLine.cpp
 *
 *  Created on: 2012. 7. 17.
 *      Author: benny
 */

#include "GroupBusLine.h"

GroupBusLine::GroupBusLine() : pArr(null)
{
	pArr = new ArrayList();
	pArr->Construct();
}

GroupBusLine::~GroupBusLine()
{
}

void GroupBusLine::SetName(const String & key)
{
	sName = GetBusTypeName(key);

}

String GroupBusLine::GetBusTypeName(const String & key)
{
	int theType;
	String lineType;

	Integer::Parse(key, theType);
	switch (theType) {
	case 0:
		lineType = "공용버스";
		break;
	case 1:
		lineType = "공항버스";
		break;
	case 3:
		lineType = "간선버스";
		break;
	case 4:
		lineType = "지선버스";
		break;
	case 5:
		lineType = "순환버스";
		break;
	case 6:
		lineType = "광역버스";
		break;
	case 7:
		lineType = "인천버스";
		break;
	case 8:
		lineType = "경기버스";
		break;
	case 9:
		lineType = "기타(폐지)";
		break;
	default:
		lineType = "기타";
		break;
	}

	return lineType;
}

Color GroupBusLine::GetBusColor(const String & key)
{
	int theType;
	Color color = Color::COLOR_GREY;

	Integer::Parse(key, theType);
	switch (theType) {
	case 1:
		color = Color(250, 160, 30);  // orange color
		break;
	case 3:
		color = Color::COLOR_BLUE;
		break;
	case 4:
		color = Color::COLOR_GREEN;
		break;
	case 5:
		color = Color::COLOR_YELLOW;
		break;
	case 6:
		color = Color::COLOR_RED;
		break;
	case 7:
		color = Color::COLOR_CYAN;
		break;
	case 8:
		color = Color::COLOR_MAGENTA;
		break;
	default:
		color = Color::COLOR_BLACK;
		break;
	}

	return color;
}
