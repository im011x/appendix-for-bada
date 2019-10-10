/*
 * DatabaseBus.h
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#ifndef DATABASEBUS_H_
#define DATABASEBUS_H_

#include "HandleDatabse.h"
#include "DataBusSeoul.h"
#include "GroupStop.h"

class MybusGroup : public Osp::Base::Object
{
public:
	MybusGroup()
	{
		__pArr = new ArrayList();
		__pArr->Construct();
	}
	virtual ~MybusGroup()
	{
		__pArr->RemoveAll(true);
		delete __pArr;
	}

public:
	Osp::Base::String 							__arsId;
	Osp::Base::String 							__stationId;
	Osp::Base::String 							__stationNm;
	Osp::Base::Collection::ArrayList 	* 		__pArr;  // collection of MybusGroupArr
};

class MybusGroupIndex : public Osp::Base::Object
{
public:
	Osp::Base::Integer 			__indexArrivingArr;
};

class MybusItem	: public Osp::Base::Object
{
public:
	Osp::Base::String 		__stationId;
	Osp::Base::String 		__stationOrd;
	Osp::Base::String 		__stationNm;

	Osp::Base::String 		__arsId;

	Osp::Base::String 		__busId;
	Osp::Base::String 		__busName;

	Osp::Base::String 		__memo;
	Osp::Base::String 		__cdate;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class DatabaseBus: public HandleDatabse {
public:
	DatabaseBus();
	virtual ~DatabaseBus();

private:
	bool 				create();
	void 				destroy();

public:
	bool 				InsertLine(const String & id, const String &nm, const String & type, const String & interval, const String & from, const String &to);
	bool 				DeleteLine(const String & id);
	bool 		  		GetFreqLineN(ArrayList * parr);

	bool 				InsertStop(const String & id, const String &id2, const String & name, const String & type);
	bool 				DeleteStop(const String & id);
	bool 		  		GetFreqStopN(ArrayList * parr);

	bool 				InsertMyBus(const String & stopId, const String & stopOrd, const String &stopName, const String &arsID, const String & busId, const String &busName);
	bool 				DeleteMyBus(const String & stopId, const String & busId);

	bool 				GetUinqueStopsMyBusN(ArrayList * parr);

protected:
	//bool 				getUinqueStopsMyBus(ArrayList * parr);
	bool 				GetMyBuses(ArrayList * parr);

};

#endif /* DATABASEBUS_H_ */
