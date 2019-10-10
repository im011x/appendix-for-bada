/*
 * DataBusSeoul.cpp
 *
 *  Created on: 2012. 7. 11.
 *      Author: benny
 */

#include "DataBusSeoul.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;


static int reorderType(const String & routeType)
{
	int nRouteType = 99;
	Integer::Parse(routeType, nRouteType);

	switch (nRouteType) {
	case 0:  // 공용
		nRouteType = 8;
		break;
	case 1: // 공항
		nRouteType = 7;
		break;
	case 2: // 없음
		nRouteType = 99;
		break;
	case 3: // 간선
		nRouteType = 3;
		break;
	case 4: // 지선
		nRouteType = 2;
		break;
	case 5: // 순환
		nRouteType = 1;
		break;
	case 6: // 광역
		nRouteType = 4;
		break;
	case 7: // 인천
		nRouteType = 5;
		break;
	case 8: // 경기
		nRouteType = 6;
		break;
	default:
		break;
	}


	return nRouteType;
}

result CompareBusSeoul_Arrive::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const BusSeoul_ArrivingInfo * p1 = static_cast <const BusSeoul_ArrivingInfo*>(&obj1);
	const BusSeoul_ArrivingInfo * p2 = static_cast <const BusSeoul_ArrivingInfo*>(&obj2);

	//int nCompareType = String::Compare(p1->__routeType, p2->__routeType);
	int nCompareType = Integer::Compare(reorderType(p1->__routeType), reorderType(p2->__routeType));

	if (nCompareType < 0) {
		cmp = -1;
	}
	else if (nCompareType > 0) {
		cmp = 1;
	}
	else {
		cmp = String::Compare(p1->__rtNm, p2->__rtNm);
	}

	return E_SUCCESS;
}


void BusSeoul_ArrivingInfo::Overwrite(BusSeoul_ArrivingInfo * psrc)
{
	__arsId 		= psrc->__arsId;
	__busRouteId 	= psrc->__busRouteId;
	__busType1 		= psrc->__busType1;
	__busType2 		= psrc->__busType2;
	__firstTm 		= psrc->__firstTm;
	__gpsX 			= psrc->__gpsX;
	__gpsY 			= psrc->__gpsY;
	__isArrive1 	= psrc->__isArrive1;
	__isArrive2 	= psrc->__isArrive2;
	__isLast1 		= psrc->__isLast1;
	__isLast2 		= psrc->__isLast2;
	__lastTm 		= psrc->__lastTm;
	__plainNo1 		= psrc->__plainNo1;
	__plainNo2 		= psrc->__plainNo2;
	__routeType 	= psrc->__routeType;
	__rtNm 			= psrc->__rtNm;
	__sectOrd1 		= psrc->__sectOrd1;
	__sectOrd2 		= psrc->__sectOrd2;
	__stId 			= psrc->__stId;
	__stNm 			= psrc->__stNm;
	__staOrd 		= psrc->__staOrd;
	__stationNm1 	= psrc->__stationNm1;
	__stationNm2 	= psrc->__stationNm2;
	__stationTp 	= psrc->__stationTp;
	__term 			= psrc->__term;
	__traSpd1 		= psrc->__traSpd1;
	__traSpd2 		= psrc->__traSpd2;
	__traTime1 		= psrc->__traTime1;
	__traTime2 		= psrc->__traTime2;
	__vehId1 		= psrc->__vehId1;
	__vehId2 		= psrc->__vehId2;
	__datetime1 	= psrc->__datetime1;
	__datetime2 	= psrc->__datetime2;
}

result CompareBusSeoul_Line::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const BusSeoul_Line * p1 = static_cast <const BusSeoul_Line*>(&obj1);
	const BusSeoul_Line * p2 = static_cast <const BusSeoul_Line*>(&obj2);

	int nCompareType = String::Compare(p1->__routeType, p2->__routeType);
	//int nCompareType = Integer::Compare(reorderType(p1->__routeType), reorderType(p2->__routeType));
	// 노선검색 fast scroll 까지 바꾸어야 한다 ...

	if (nCompareType < 0) {
		cmp = -1;
	}
	else if (nCompareType > 0) {
		cmp = 1;
	}
	else {
		cmp = String::Compare(p1->__busRouteNm, p2->__busRouteNm);
	}

	return E_SUCCESS;
}

void BusSeoul_ArrivingByLine::Overwrite(BusSeoul_ArrivingByLine * psrc)
{
	//AppLogDebug("### updated with new info (ex. order : %S -> %S and datetime : %S -> %S)",
	//		__sectOrd1.GetPointer(), psrc->__sectOrd1.GetPointer(),
	//		__traTime1.GetPointer(), psrc->__traTime1.GetPointer());

	__arsId                       	= psrc->__arsId;
	__avgCf1                      	= psrc->__avgCf1;
	__avgCf2                      	= psrc->__avgCf2;
	__brdrde_Num1                 	= psrc->__brdrde_Num1;
	__brdrde_Num2                 	= psrc->__brdrde_Num2;
	__brerde_Div1                 	= psrc->__brerde_Div1;
	__brerde_Div2                 	= psrc->__brerde_Div2;
	__busRouteId                  	= psrc->__busRouteId;
	__busType1                    	= psrc->__busType1;
	__busType2                    	= psrc->__busType2;
	__dir                         	= psrc->__dir;
	__expCf1                      	= psrc->__expCf1;
	__expCf2                      	= psrc->__expCf2;
	__exps1                       	= psrc->__exps1;
	__exps2                       	= psrc->__exps2;
	__firstTm                     	= psrc->__firstTm;
	__full1                       	= psrc->__full1;
	__full2                       	= psrc->__full2;
	__goal1                       	= psrc->__goal1;
	__goal2                       	= psrc->__goal2;
	__isArrive1                   	= psrc->__isArrive1;
	__isArrive2                   	= psrc->__isArrive2;
	__isLast1                     	= psrc->__isLast1;
	__isLast2                     	= psrc->__isLast2;
	__kalCf1                      	= psrc->__kalCf1;
	__kalCf2                      	= psrc->__kalCf2;
	__kals1                       	= psrc->__kals1;
	__kals2                       	= psrc->__kals2;
	__lastTm                      	= psrc->__lastTm;
	__mkTm                        	= psrc->__mkTm;
	__namin2Sec1                  	= psrc->__namin2Sec1;
	__namin2Sec2                  	= psrc->__namin2Sec2;
	__neuCf1                      	= psrc->__neuCf1;
	__neuCf2                      	= psrc->__neuCf2;
	__neus1                       	= psrc->__neus1;
	__neus2                       	= psrc->__neus2;
	__nmain2Ord1                  	= psrc->__nmain2Ord1;
	__nmain2Ord2                  	= psrc->__nmain2Ord2;
	__nmain2Stnid1                	= psrc->__nmain2Stnid1;
	__nmain2Stnid2                	= psrc->__nmain2Stnid2;
	__nmain3Ord1                  	= psrc->__nmain3Ord1;
	__nmain3Ord2                  	= psrc->__nmain3Ord2;
	__nmain3Sec1                  	= psrc->__nmain3Sec1;
	__nmain3Sec2                  	= psrc->__nmain3Sec2;
	__nmain3Stnid1                	= psrc->__nmain3Stnid1;
	__nmain3Stnid2                	= psrc->__nmain3Stnid2;
	__nmainOrd1                   	= psrc->__nmainOrd1;
	__nmainOrd2                   	= psrc->__nmainOrd2;
	__nmainSec1                   	= psrc->__nmainSec1;
	__nmainSec2                   	= psrc->__nmainSec2;
	__nmainStnid1                 	= psrc->__nmainStnid1;
	__nmainStnid2                 	= psrc->__nmainStnid2;
	__nstnId1                     	= psrc->__nstnId1;
	__nstnId2                     	= psrc->__nstnId2;
	__nstnOrd1                    	= psrc->__nstnOrd1;
	__nstnOrd2                    	= psrc->__nstnOrd2;
	__nstnSec1                    	= psrc->__nstnSec1;
	__nstnSec2                    	= psrc->__nstnSec2;
	__nstnSpd1                    	= psrc->__nstnSpd1;
	__nstnSpd2                    	= psrc->__nstnSpd2;
	__plainNo1                    	= psrc->__plainNo1;
	__plainNo2                    	= psrc->__plainNo2;
	__repTm1                      	= psrc->__repTm1;
	__repTm2                      	= psrc->__repTm2;
	__rerdie_Div1                 	= psrc->__rerdie_Div1;
	__rerdie_Div2                 	= psrc->__rerdie_Div2;
	__reride_Num1                 	= psrc->__reride_Num1;
	__reride_Num2                 	= psrc->__reride_Num2;
	__routeType                   	= psrc->__routeType;
	__rtNm                        	= psrc->__rtNm;
	__sectOrd1                    	= psrc->__sectOrd1;
	__sectOrd2                    	= psrc->__sectOrd2;
	__stId                        	= psrc->__stId;
	__stNm                        	= psrc->__stNm;
	__staOrd                      	= psrc->__staOrd;
	__stationNm1                  	= psrc->__stationNm1;
	__stationNm2                  	= psrc->__stationNm2;
	__term                        	= psrc->__term;
	__traSpd1                     	= psrc->__traSpd1;
	__traSpd2                     	= psrc->__traSpd2;
	__traTime1                    	= psrc->__traTime1;
	__traTime2                    	= psrc->__traTime2;
	__vehId1                      	= psrc->__vehId1;
	__vehId2                      	= psrc->__vehId2;

	__datetime1 	= psrc->__datetime1;
	__datetime2 	= psrc->__datetime2;
}


result CompareBusSeoul_StopNear::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const BusSeoul_StopInfo * p1 = static_cast <const BusSeoul_StopInfo*>(&obj1);
	const BusSeoul_StopInfo * p2 = static_cast <const BusSeoul_StopInfo*>(&obj2);

	cmp = Float::Compare(p1->__distance, p2->__distance);

	return E_SUCCESS;
}

result CompareBusSeoul_StopName::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const BusSeoul_StopInfo * p1 = static_cast <const BusSeoul_StopInfo*>(&obj1);
	const BusSeoul_StopInfo * p2 = static_cast <const BusSeoul_StopInfo*>(&obj2);

	cmp = String::Compare(p1->__arsId, p2->__arsId);

	return E_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BusSeoul_StopByLine::BusSeoul_StopByLine()
{
	__pArrPositionInfo = new ArrayList();
	__pArrPositionInfo->Construct();
}

BusSeoul_StopByLine::~BusSeoul_StopByLine()
{
	__pArrPositionInfo->RemoveAll(false);
	delete __pArrPositionInfo;
}
