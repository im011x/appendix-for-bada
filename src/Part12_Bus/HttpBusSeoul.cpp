/*
 * HttpBusSeoul.cpp
 *
 *  Created on: 2012. 6. 30.
 *      Author: benny
 */

#include "HttpBusSeoul.h"

HttpBusSeoul::HttpBusSeoul(IHttpFirstResponder * plistener) : OpenApiBase(plistener)
{
}

HttpBusSeoul::~HttpBusSeoul()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 정류소
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpBusSeoul::GetStopsByName(const String & keywords)
{
	String searchWords;
	//String searchWords = keywords;  // EUC-KR 변환 필요?
	//ByteBuffer * pSearchWords = UtilCode::Unicode_to_EuckrN(keywords);

	Osp::Base::Utility::UrlEncoder::Encode(keywords, "UTF-8", searchWords);

	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getStationByName");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&stSrch=");
	theUrl.Append(searchWords);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}
bool HttpBusSeoul::GetArrivingByStop(const String & stopID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getStationByUid");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&arsId=");
	theUrl.Append(stopID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetLanesByStop(const String & stopID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getRouteByStation");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&arsId=");
	theUrl.Append(stopID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetTimesBy(const String & stopID, const String & laneID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getBustimeByStation");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&arsId=");
	theUrl.Append(stopID);
	theUrl.Append("&busRouteId=");
	theUrl.Append(laneID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetLowStopsByName(const String & keywords)
{
	String searchWords;
	Osp::Base::Utility::UrlEncoder::Encode(keywords, "UTF-8", searchWords);
	//Osp::Base::Utility::UrlEncoder::Encode(keywords, "KSC5601", searchWords);

	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getLowStationByName");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&stSrch=");
	theUrl.Append(searchWords);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetLowArrivingByStop(const String & stopID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getLowStationByUid");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&arsId=");
	theUrl.Append(stopID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetStopsByPosition(const String & latit, const String & longi, const String & radius)
{
	String theUrl("http://ws.bus.go.kr/api/rest/stationinfo/getStationByPos");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&tmX=");
	theUrl.Append(longi);
	theUrl.Append("&tmY=");
	theUrl.Append(latit);
	theUrl.Append("&radius=");
	theUrl.Append(radius);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스 도착
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpBusSeoul::GetArrivingByLane(const String & laneID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/arrive/getArrInfoByRouteAll");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(laneID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetArrivingBy(const String & stopID, const String & stopOrd, const String & lineID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/arrive/getArrInfoByRoute");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&stId=");
	theUrl.Append(stopID);
	theUrl.Append("&ord=");
	theUrl.Append(stopOrd);
	theUrl.Append("&busRouteId=");
	theUrl.Append(lineID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetLowArrivingByLane(const String & laneID)
{
	// http://ws.bus.go.kr/api/rest/arrive/getLowArrInfoByStId
	String theUrl("http://ws.bus.go.kr/api/rest/arrive/getLowArrInfoByStId");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(laneID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetLowArrivingBy(const String & stopID, const String & laneID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/arrive/getLowArrInfoByRoute");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&arsId=");
	theUrl.Append(stopID);
	theUrl.Append("&busRouteId=");
	theUrl.Append(laneID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스 위치
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpBusSeoul::GetPositionsByLine(const String & lineID)
{

	String theUrl("http://ws.bus.go.kr/api/rest/buspos/getBusPosByRtid");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(lineID);

	// ex) http://ws.bus.go.kr/api/rest/buspos/getBusPosByRtid?serviceKey=...&busRouteId=3060000
	/*
		<itemList>
			<busType>0</busType>
			<dataTm>20120811001459</dataTm>
			<fullSectDist>2409.675</fullSectDist>
			<gpsX>126.94261780432299</gpsX>
			<gpsY>37.53714120935828</gpsY>
			<isrunyn>1</isrunyn>
			<lastStTm>4155</lastStTm>
			<lastStnId>5941</lastStnId>
			<lastStnOrd>24</lastStnOrd>
			<lstbusyn>1</lstbusyn>
			<nextStTm>604</nextStTm>
			<plainNo>서울71사3278</plainNo>
			<rtDist>38</rtDist>
			<sectDist>1778</sectDist>
			<sectOrd>25</sectOrd>
			<sectionId>52254</sectionId>
			<stopFlag>1</stopFlag>
			<trnstnid>1887</trnstnid>
			<vehId>4533</vehId>
		</itemList>
		<itemList>
			<busType>0</busType>
			<dataTm>20120811001459</dataTm>
			<fullSectDist>1071.422</fullSectDist>
			<gpsX>126.9621036705021</gpsX>
			<gpsY>37.55922590387209</gpsY>
			<isrunyn>1</isrunyn>
			<lastStTm>3726</lastStTm>
			<lastStnId>34960</lastStnId>
			<lastStnOrd>29</lastStnOrd>
			<lstbusyn>2</lstbusyn>
			<nextStTm>159</nextStTm>
			<plainNo>서울71사3277</plainNo>
			<rtDist>38</rtDist>
			<sectDist>734</sectDist>
			<sectOrd>30</sectOrd>
			<sectionId>70036</sectionId>
			<stopFlag>1</stopFlag>
			<trnstnid>1887</trnstnid>
			<vehId>6936</vehId>
		</itemList>
	 *
	 */

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 버스 노선
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 전체 노선 정보
 *
 */

bool HttpBusSeoul::GetLines()
{
	String theUrl("http://ws.bus.go.kr/api/rest/busRouteInfo/getBusRouteList");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

/**
 * 특정 노선의 정보
 */

bool HttpBusSeoul::GetInfoByLine(const String & lineID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/busRouteInfo/getRouteInfo");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(lineID);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

/**
 * 특정 노선의 정류소 목록
 *
 */
bool HttpBusSeoul::GetStopsByLine(const String & lineID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/busRouteInfo/getStaionByRoute");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(lineID);

	// ex) http://ws.bus.go.kr/api/rest/busRouteInfo/getStaionByRoute?serviceKey=...&busRouteId=3060000

	/* 이런 결과 돌려줌
		<itemList>
		<beginTm>04:00</beginTm>
		<busRouteId>3060000</busRouteId>
		<busRouteNm>600</busRouteNm>
		<direction>광화문</direction>
		<gpsX>126.82137891500601</gpsX>
		<gpsY>37.491921849804584</gpsY>
		<lastTm>23:30</lastTm>
		<routeType>3</routeType>
		<section>0</section>
		<seq>1</seq>
		<station>32645</station>
		<stationNm>온수동종점</stationNm>
		<stationNo>17290</stationNo>
		<fullSectDist>0</fullSectDist>
		<trnstnid>1887</trnstnid>
		</itemList>
	 *
	 */
	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

bool HttpBusSeoul::GetGpsByLine(const String & lineID)
{
	String theUrl("http://ws.bus.go.kr/api/rest/busRouteInfo/getRoutePath");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&busRouteId=");
	theUrl.Append(lineID);

	/* 이런 결과 잔뜩 돌려줌
		<itemList>
		<gpsX>126.82205974564961</gpsX>
		<gpsY>37.49190755841939</gpsY>
		<no>10</no>
		</itemList>
	 *
	 */

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 환승 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpBusSeoul::GetPostionsByName(const String & keywords)
{
	String searchWords;
	//String searchWords = keywords;  // EUC-KR 변환 필요?
	//ByteBuffer * pSearchWords = UtilCode::Unicode_to_EuckrN(keywords);

	Osp::Base::Utility::UrlEncoder::Encode(keywords, "UTF-8", searchWords);
	//Osp::Base::Utility::UrlEncoder::Encode(keywords, "KSC5601", searchWords);

	// http://ws.bus.go.kr/api/rest/pathinfo/getLocationInfo
	String theUrl("http://ws.bus.go.kr/api/rest/pathinfo/getLocationInfo");
	theUrl.Append("?serviceKey=");
	theUrl.Append(__ServiceKey);
	theUrl.Append("&stSrch=");
	theUrl.Append(searchWords);

	return __pHttpHandler->GetHttp(theUrl, "http://ws.bus.go.kr");
}

