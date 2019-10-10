/*
 * DataBusSeoul.h
 *
 *  Created on: 2012. 7. 11.
 *      Author: benny
 */

#ifndef DATABUSSEOUL_H_
#define DATABUSSEOUL_H_

#include <FBase.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 *
 */
class BusSeoul_ArrivingInfo : public Osp::Base::Object
{
public:
	Osp::Base::String 		__arsId;
	Osp::Base::String 		__busRouteId;
	Osp::Base::String 		__busType1;
	Osp::Base::String 		__busType2;
	Osp::Base::String 		__firstTm;
	Osp::Base::String 		__gpsX;
	Osp::Base::String 		__gpsY;
	Osp::Base::String 		__isArrive1;
	Osp::Base::String 		__isArrive2;
	Osp::Base::String 		__isLast1;
	Osp::Base::String 		__isLast2;
	Osp::Base::String 		__lastTm;
	Osp::Base::String 		__plainNo1;
	Osp::Base::String 		__plainNo2;
	Osp::Base::String 		__routeType;
	Osp::Base::String 		__rtNm;
	Osp::Base::String 		__sectOrd1;
	Osp::Base::String 		__sectOrd2;
	Osp::Base::String 		__stId;
	Osp::Base::String 		__stNm;
	Osp::Base::String 		__staOrd;
	Osp::Base::String 		__stationNm1;
	Osp::Base::String 		__stationNm2;
	Osp::Base::String 		__stationTp;
	Osp::Base::String 		__term;
	Osp::Base::String 		__traSpd1;
	Osp::Base::String 		__traSpd2;
	Osp::Base::String 		__traTime1;
	Osp::Base::String 		__traTime2;
	Osp::Base::String 		__vehId1;
	Osp::Base::String 		__vehId2;

	Osp::Base::DateTime 	__datetime1;
	Osp::Base::DateTime 	__datetime2;

public:
	void 	Overwrite(BusSeoul_ArrivingInfo * psrc);
};

class CompareBusSeoul_Arrive :  public Osp::Base::Collection::IComparer
{
public:
	CompareBusSeoul_Arrive() {};
	virtual ~CompareBusSeoul_Arrive() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};

// class BusSeoul_ArrivingByLine : public BusSeoul_ArrivingInfo 로 바꾸자!
class BusSeoul_ArrivingByLine : public Osp::Base::Object
{
public:
	Osp::Base::String 		__arsId;
	Osp::Base::String 		__avgCf1;
	Osp::Base::String 		__avgCf2;
	Osp::Base::String 		__brdrde_Num1;
	Osp::Base::String 		__brdrde_Num2;
	Osp::Base::String 		__brerde_Div1;
	Osp::Base::String 		__brerde_Div2;
	Osp::Base::String 		__busRouteId;
	Osp::Base::String 		__busType1;
	Osp::Base::String 		__busType2;
	Osp::Base::String 		__dir;
	Osp::Base::String 		__expCf1;
	Osp::Base::String 		__expCf2;
	Osp::Base::String 		__exps1;
	Osp::Base::String 		__exps2;
	Osp::Base::String 		__firstTm;
	Osp::Base::String 		__full1;
	Osp::Base::String 		__full2;
	Osp::Base::String 		__goal1;
	Osp::Base::String 		__goal2;
	Osp::Base::String 		__isArrive1;
	Osp::Base::String 		__isArrive2;
	Osp::Base::String 		__isLast1;
	Osp::Base::String 		__isLast2;
	Osp::Base::String 		__kalCf1;
	Osp::Base::String 		__kalCf2;
	Osp::Base::String 		__kals1;
	Osp::Base::String 		__kals2;
	Osp::Base::String 		__lastTm;
	Osp::Base::String 		__mkTm;
	Osp::Base::String 		__namin2Sec1;
	Osp::Base::String 		__namin2Sec2;
	Osp::Base::String 		__neuCf1;
	Osp::Base::String 		__neuCf2;
	Osp::Base::String 		__neus1;
	Osp::Base::String 		__neus2;
	Osp::Base::String 		__nmain2Ord1;
	Osp::Base::String 		__nmain2Ord2;
	Osp::Base::String 		__nmain2Stnid1;
	Osp::Base::String 		__nmain2Stnid2;
	Osp::Base::String 		__nmain3Ord1;
	Osp::Base::String 		__nmain3Ord2;
	Osp::Base::String 		__nmain3Sec1;
	Osp::Base::String 		__nmain3Sec2;
	Osp::Base::String 		__nmain3Stnid1;
	Osp::Base::String 		__nmain3Stnid2;
	Osp::Base::String 		__nmainOrd1;
	Osp::Base::String 		__nmainOrd2;
	Osp::Base::String 		__nmainSec1;
	Osp::Base::String 		__nmainSec2;
	Osp::Base::String 		__nmainStnid1;
	Osp::Base::String 		__nmainStnid2;
	Osp::Base::String 		__nstnId1;
	Osp::Base::String 		__nstnId2;
	Osp::Base::String 		__nstnOrd1;
	Osp::Base::String 		__nstnOrd2;
	Osp::Base::String 		__nstnSec1;
	Osp::Base::String 		__nstnSec2;
	Osp::Base::String 		__nstnSpd1;
	Osp::Base::String 		__nstnSpd2;
	Osp::Base::String 		__plainNo1;
	Osp::Base::String 		__plainNo2;
	Osp::Base::String 		__repTm1;
	Osp::Base::String 		__repTm2;
	Osp::Base::String 		__rerdie_Div1;
	Osp::Base::String 		__rerdie_Div2;
	Osp::Base::String 		__reride_Num1;
	Osp::Base::String 		__reride_Num2;
	Osp::Base::String 		__routeType;
	Osp::Base::String 		__rtNm;
	Osp::Base::String 		__sectOrd1;
	Osp::Base::String 		__sectOrd2;
	Osp::Base::String 		__stId;
	Osp::Base::String 		__stNm;
	Osp::Base::String 		__staOrd;
	Osp::Base::String 		__stationNm1;
	Osp::Base::String 		__stationNm2;
	Osp::Base::String 		__term;
	Osp::Base::String 		__traSpd1;
	Osp::Base::String 		__traSpd2;
	Osp::Base::String 		__traTime1;
	Osp::Base::String 		__traTime2;
	Osp::Base::String 		__vehId1;
	Osp::Base::String 		__vehId2;

	Osp::Base::DateTime 	__datetime1;
	Osp::Base::DateTime 	__datetime2;

public:
	void 	Overwrite(BusSeoul_ArrivingByLine * psrc);
};



/**
 * 정류소별경유노선목록조회 결과
 */

/*
// 이거 나중에 BusSeoul_Line 혹은 BusSeoul_LineDetail 로 없애자~
//

class BusSeoul_LineInfo : public Osp::Base::Object
{
public:
	Osp::Base::String 		__lineID;
	Osp::Base::String 		__lineName;
	Osp::Base::String 		__lineType;
	Osp::Base::String 		__lineLenght;
	Osp::Base::String 		__linePlaceFrom;
	Osp::Base::String 		__linePlaceTo;
	Osp::Base::String 		__lineInterval;

	bool 					__isLastBus;
	Osp::Base::String 		__timeFirstBus;
	Osp::Base::String 		__timeLastBus;
	Osp::Base::String 		__timeFirstLowBus;
	Osp::Base::String 		__timeLastLowBus;
};
*/


/**
 *
 */
class BusSeoul_Line	: public Osp::Base::Object
{
public:
	Osp::Base::String 		__busRouteId;
	Osp::Base::String 		__busRouteNm;
	Osp::Base::String 		__edStationNm;
	Osp::Base::String 		__routeType;
	Osp::Base::String 		__stStationNm;
	Osp::Base::String 		__term;
	Osp::Base::String 		__length;

	// 자주쓰는 등록시 사용
	Osp::Base::String 		__memo;
	Osp::Base::String 		__cdate;
};

class BusSeoul_LineDetail : public BusSeoul_Line
{
public:
	Osp::Base::String 		__firstBusTm;
	Osp::Base::String 		__lastBusTm;
	Osp::Base::String 		__firstLowBusTm;
	Osp::Base::String 		__lastLowBusTm;

	Osp::Base::String 		__firstBusTmCurr;
	Osp::Base::String 		__lastBusTmCurr;

	Osp::Base::String 		__firstBusTmCurrTTS;
	Osp::Base::String 		__lastBusTmCurrTTS;

};

class CompareBusSeoul_Line :  public Osp::Base::Collection::IComparer
{
public:
	CompareBusSeoul_Line() {};
	virtual ~CompareBusSeoul_Line() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};

/**
 *
 */

class BusSeoul_BusPosition : public Osp::Base::Object
{
public:
	Osp::Base::String 		__busType;
	Osp::Base::String 		__dataTm;
	Osp::Base::String 		__fullSectDist;
	Osp::Base::String 		__gpsX;
	Osp::Base::String 		__gpsY;
	Osp::Base::String 		__isrunyn;  // 운행 여부
	Osp::Base::String 		__lastStTm;
	Osp::Base::String 		__lastStnId;
	Osp::Base::String 		__lastStnOrd;
	Osp::Base::String 		__lstbusyn; // 막차 여부
	Osp::Base::String 		__nextStTm; // 다음 정류소 도착 소요 예상 시간
	Osp::Base::String 		__plainNo;
	Osp::Base::String 		__rtDist;
	Osp::Base::String 		__sectDist;
	Osp::Base::String 		__sectOrd;  // 구간 순번 - 해당노선의 N 번째 정류소
	Osp::Base::String 		__sectionId;
	Osp::Base::String 		__stopFlag; // 정류소 도착여부 - 0:운행중, 1:도착
	Osp::Base::String 		__trnstnid;
	Osp::Base::String 		__vehId;
};


/**
 *
 */

class BusSeoul_StopByLine	: public Osp::Base::Object
{
public:
	BusSeoul_StopByLine();
	virtual ~BusSeoul_StopByLine();
public:
	Osp::Base::String 		__beginTm;
	Osp::Base::String 		__busRouteId;
	Osp::Base::String 		__busRouteNm;
	Osp::Base::String 		__direction;
	Osp::Base::String 		__gpsX;
	Osp::Base::String 		__gpsY;
	Osp::Base::String 		__lastTm;
	Osp::Base::String 		__routeType;
	Osp::Base::String 		__section;
	Osp::Base::String 		__seq;
	Osp::Base::String 		__station;
	Osp::Base::String 		__stationNm;
	Osp::Base::String 		__stationNo;
	Osp::Base::String 		__fullSectDist;
	Osp::Base::String 		__trnstnid;

	//
	//BusSeoul_BusPosition * 	__pPositionInfo;
	Osp::Base::Collection::ArrayList 	*	__pArrPositionInfo;
};

/**
 *
 */

class BusSeoul_StopInfo	: public Osp::Base::Object
{
public:
	Osp::Base::String 		__arsId;
	Osp::Base::String 		__stationId;
	Osp::Base::String 		__stationNm;
	Osp::Base::String 		__stationTp;

	Osp::Base::String 		__gpsX;
	Osp::Base::String 		__gpsY;

	float 					__distance;

	// 자주쓰는 등록시 사용
	Osp::Base::String 		__memo;
	Osp::Base::String 		__cdate;
};

class CompareBusSeoul_StopNear :  public Osp::Base::Collection::IComparer
{
public:
	CompareBusSeoul_StopNear() {};
	virtual ~CompareBusSeoul_StopNear() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};

class CompareBusSeoul_StopName :  public Osp::Base::Collection::IComparer
{
public:
	CompareBusSeoul_StopName() {};
	virtual ~CompareBusSeoul_StopName() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};

#endif /* DATABUSSEOUL_H_ */
