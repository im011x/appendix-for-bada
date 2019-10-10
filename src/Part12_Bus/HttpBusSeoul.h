/*
 * HttpBusSeoul.h
 *
 *  Created on: 2012. 6. 30.
 *      Author: benny
 */

#ifndef HTTPBUSSEOUL_H_
#define HTTPBUSSEOUL_H_

#include <FUi.h>
#include "OpenApiBase.h"

class HttpBusSeoul : public OpenApiBase
{
public:
	HttpBusSeoul(IHttpFirstResponder * pListener);
	virtual ~HttpBusSeoul();

public:
	/*
	 * 정류소
	 */
	// station, bus stop information
	bool 		GetStopsByName(const String & keywords);
	bool 		GetArrivingByStop(const String & stopID);
	bool 		GetLanesByStop(const String & stopID);
	bool 		GetTimesBy(const String & stopID, const String & laneID);
	// low bus only
	bool 		GetLowStopsByName(const String & keywords);
	bool 		GetLowArrivingByStop(const String & stopID);
	// near
	bool 		GetStopsByPosition(const String & latit, const String & longi, const String & radius);

	/*
	 * 버스 도착
	 */
	//
	bool 		GetArrivingByLane(const String & laneID);
	bool 		GetArrivingBy(const String & stopID, const String & stopOrd, const String & laneID);
	bool 		GetLowArrivingByLane(const String & laneID);
	bool 		GetLowArrivingBy(const String & stopID, const String & laneID);

	/*
	 * 버스 위치
	 */
	//
	bool 		GetPositionsByLine(const String & lineID); //

	/*
	 * 버스 노선
	 */
	bool 		GetLines(); // 모든 버스 노선
	bool 		GetInfoByLine(const String & lineID);  // 노선 기본정보
	bool 		GetStopsByLine(const String & lineID); // 노선별 정류소 정보
	bool 		GetGpsByLine(const String & lineID); //


	/*
	 * 환승정보
	 */
	bool 		GetPostionsByName(const String & keywords);
};


#endif /* HTTPBUSSEOUL_H_ */
