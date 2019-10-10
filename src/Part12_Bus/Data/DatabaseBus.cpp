/*
 * DatabaseBus.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "DatabaseBus.h"
#include "UtilApp.h"

DatabaseBus::DatabaseBus() : HandleDatabse("/Home/dbBus.sqlite")
{
	if (!Database::Exists(__fileNameDb)) {
		if (create() == false) {
			destroy();
		}
	}
}

DatabaseBus::~DatabaseBus()
{
}

bool DatabaseBus::create()
{
	// 일단 DB 파일을 생성한다.
	if (Open() == false) {
		AppLogDebug("DB 파일 생성 실패!");
		return false;
	}

	// 테이블을 생성한다.
	String sql_line = "CREATE TABLE IF NOT EXISTS TFreqLine ("
			//"  UID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT default 0"
			" lineID TEXT NOT NULL"
			", lineName TEXT NOT NULL"
			", lineType TEXT NOT NULL"
			", lineInterval TEXT"
			", lineFrom TEXT"
			", lineTo TEXT"
			", memo TEXT"
			", cdate TEXT"
			", primary key(lineID)"
			")";

	String sql_stop = "CREATE TABLE IF NOT EXISTS TFreqStop ("
			//"  UID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT default 0"
			" arsID TEXT NOT NULL"
			", stopID TEXT NOT NULL"
			", stopName TEXT"
			", stopType TEXT"
			", memo TEXT"
			", cdate TEXT"
			", primary key(arsID)"
			")";

	String sql_busStop = "CREATE TABLE IF NOT EXISTS TMyBus ("
			//"  UID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT default 0"
			"  stopID TEXT NOT NULL"
			", stopOrder TEXT NOT NULL"
			", stopName TEXT NOT NULL"
			", arsID TEXT NOT NULL"
			", busID TEXT NOT NULL"
			", busName TEXT NOT NULL"

			", memo TEXT"
			", cdate TEXT"

			", primary key(stopID, busID)"
			")";

	__pDb->BeginTransaction();

	if (Execute(sql_line) == false) {
		AppLogDebug("DB 테이블 생성 실패!");
		__pDb->RollbackTransaction();
		return false;
	}

	if (Execute(sql_stop) == false) {
		AppLogDebug("DB 테이블 생성 실패!");
		__pDb->RollbackTransaction();
		return false;
	}

	if (Execute(sql_busStop) == false) {
		AppLogDebug("DB 테이블 생성 실패!");
		__pDb->RollbackTransaction();
		return false;
	}

	__pDb->CommitTransaction();

	AppLogDebug("@@@ DB 생성 성공! @@@");

	Close();

	return true;
}

void DatabaseBus::destroy()
{
	if (Database::Exists(__fileNameDb))
		Database::Delete(__fileNameDb);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool DatabaseBus::InsertLine(const String & id, const String &nm, const String & type, const String & interval, const String & from, const String &to)
{
	String sql = "INSERT INTO TFreqLine (lineID, lineName, lineType, lineInterval, lineFrom, lineTo, cdate) VALUES (?, ?, ?, ?, ?, ?, ?)";

	AppLogDebug("Query = [%S]", sql.GetPointer());

	ArrayList * parrOpts = new ArrayList();
	parrOpts->Construct();
	parrOpts->Add(*(new String(id)));
	parrOpts->Add(*(new String(nm)));
	parrOpts->Add(*(new String(type)));
	parrOpts->Add(*(new String(interval)));
	parrOpts->Add(*(new String(from)));
	parrOpts->Add(*(new String(to)));
	parrOpts->Add(*(new String(UtilApp::GetNowString())));

	bool bOK = ExecuteWithOpts(sql, *parrOpts);

	parrOpts->RemoveAll(true);
	delete parrOpts;

	return bOK;
}

bool DatabaseBus::DeleteLine(const String & sId)
{
	String sql;
	sql.Format(256, L"DELETE from TFreqLine where lineID = '%S'", sId.GetPointer());

	bool bOK = Execute(sql);

	return bOK;
}

bool DatabaseBus::GetFreqLineN(ArrayList * parr)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppAssert(parr);
	parr->RemoveAll(true);

	const String query = "SELECT lineID, lineName, lineType, lineInterval, lineFrom, lineTo, memo, cdate FROM TFreqLine";
	DbEnumerator* pEnum = __pDb->QueryN(query);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			BusSeoul_Line * pItem = new BusSeoul_Line();
			int index = 0;

			pEnum->GetStringAt(index++, pItem->__busRouteId);
			pEnum->GetStringAt(index++, pItem->__busRouteNm);
			pEnum->GetStringAt(index++, pItem->__routeType);
			pEnum->GetStringAt(index++, pItem->__term);
			pEnum->GetStringAt(index++, pItem->__stStationNm);
			pEnum->GetStringAt(index++, pItem->__edStationNm);
			pEnum->GetStringAt(index++, pItem->__memo);
			pEnum->GetStringAt(index++, pItem->__cdate);

			parr->Add(*pItem);
		}

		delete pEnum;
	}

	//AppLogDebug("select count = %d", parr->GetCount());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool DatabaseBus::InsertStop(const String & id, const String &id2, const String & name, const String & type)
{
	String sql = "INSERT INTO TFreqStop (arsID, stopID, stopName, stopType, cdate) VALUES (?, ?, ?, ?, ?)";

	ArrayList * parrOpts = new ArrayList();
	parrOpts->Construct();
	parrOpts->Add(*(new String(id)));
	parrOpts->Add(*(new String(id2)));
	parrOpts->Add(*(new String(name)));
	parrOpts->Add(*(new String(type)));
	parrOpts->Add(*(new String(UtilApp::GetNowString())));

	bool bOK = ExecuteWithOpts(sql, *parrOpts);

	parrOpts->RemoveAll(true);
	delete parrOpts;

	return bOK;
}

bool DatabaseBus::DeleteStop(const String & sId)
{
	String sql;
	sql.Format(256, L"DELETE from TFreqStop where arsID = '%S'", sId.GetPointer());

	bool bOK = Execute(sql);

	return bOK;
}

bool DatabaseBus::GetFreqStopN(ArrayList * parr)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppAssert(parr);
	parr->RemoveAll(true);

	const String query = "SELECT arsID, stopID, stopName, stopType, memo, cdate FROM TFreqStop";
	DbEnumerator* pEnum = __pDb->QueryN(query);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			BusSeoul_StopInfo * pItem = new BusSeoul_StopInfo();
			int index = 0;

			pEnum->GetStringAt(index++, pItem->__arsId);
			pEnum->GetStringAt(index++, pItem->__stationId);
			pEnum->GetStringAt(index++, pItem->__stationNm);
			pEnum->GetStringAt(index++, pItem->__stationTp);
			pEnum->GetStringAt(index++, pItem->__memo);
			pEnum->GetStringAt(index++, pItem->__cdate);

			parr->Add(*pItem);
		}

		delete pEnum;
	}

	//AppLogDebug("select count = %d", parr->GetCount());

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool DatabaseBus::InsertMyBus(const String & stopId, const String & stopOrd, const String &stopName, const String &arsID, const String & busId, const String &busName)
{
	String sql = "INSERT INTO TMyBus (stopID, stopOrder, stopName, arsID, busID, busName, cdate) VALUES (?, ?, ?, ?, ?, ?, ?)";

	AppLogDebug("내버스 저장 문장 = %S", sql.GetPointer());

	AppLogDebug("내버스 저장 값 stop ID = %S", stopId.GetPointer());
	AppLogDebug("내버스 저장 값 stop Order = %S", stopOrd.GetPointer());
	AppLogDebug("내버스 저장 값 stop Name = %S", stopName.GetPointer());
	AppLogDebug("내버스 저장 값 ars ID = %S", arsID.GetPointer());
	AppLogDebug("내버스 저장 값 bus ID = %S", busId.GetPointer());
	AppLogDebug("내버스 저장 값 bus Name = %S", busName.GetPointer());

	ArrayList * parrOpts = new ArrayList();
	parrOpts->Construct();
	parrOpts->Add(*(new String(stopId)));
	parrOpts->Add(*(new String(stopOrd)));
	parrOpts->Add(*(new String(stopName)));
	parrOpts->Add(*(new String(arsID)));
	parrOpts->Add(*(new String(busId)));
	parrOpts->Add(*(new String(busName)));
	parrOpts->Add(*(new String(UtilApp::GetNowString())));

	bool bOK = ExecuteWithOpts(sql, *parrOpts);

	parrOpts->RemoveAll(true);
	delete parrOpts;

	return bOK;
}

bool DatabaseBus::DeleteMyBus(const String & stopId, const String & busId)
{
	String sql;
	sql.Format(256, L"DELETE from TMyBus where stopID = '%S' and busID = '%S'", stopId.GetPointer(), busId.GetPointer());

	bool bOK = Execute(sql);

	return bOK;
}

bool DatabaseBus::GetUinqueStopsMyBusN(ArrayList * parr)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppAssert(parr);
	parr->RemoveAll(true);

	// 정류소 코드를 가져온다.
	const String query = "SELECT distinct stopID FROM TMyBus";
	DbEnumerator* pEnum = __pDb->QueryN(query);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			GroupStop * pStop = new GroupStop();

			pEnum->GetStringAt(0, pStop->sId);

			parr->Add(*pStop);
		}

		delete pEnum;
	}

	//AppLogDebug("select count = %d", parr->GetCount());

	return GetMyBuses(parr);
}

//bool DatabaseBus::getUinqueStopsMyBus(ArrayList * parr)
//{
//	if (!isOpen()) {
//		if (!Open()) {
//			return false;
//		}
//	}
//
//	AppAssert(parr);
//
//	for (int i=0; i<parr->GetCount(); i++) {
//		GroupStop * pStop = static_cast<GroupStop*> (parr->GetAt(i));
//
//		String query = "SELECT stopName FROM TMyBus where stopID = ";
//		query.Append("'");
//		query.Append(pStop->sId);
//		query.Append("'");
//
//		//
//		DbEnumerator* pEnum = __pDb->QueryN(query);
//		if( pEnum != null )  {
//			while( pEnum->MoveNext()== E_SUCCESS )	{
//				pEnum->GetStringAt(0, pStop->sName);
//				break;
//			}
//
//			delete pEnum;
//		}
//		//
//	}
//
//	return true;
//}

bool DatabaseBus::GetMyBuses(ArrayList * pArrGrpup)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppAssert(pArrGrpup);

	/*
	"  stopID TEXT NOT NULL"
	", stopOrder TEXT NOT NULL"
	", stopName TEXT NOT NULL"
	", arsID TEXT NOT NULL"
	", busID TEXT NOT NULL"
	", busName TEXT NOT NULL"
	 */

	for (int i=0; i<pArrGrpup->GetCount(); i++) {
		GroupStop * pStop = static_cast<GroupStop*> (pArrGrpup->GetAt(i));

		String query = "SELECT stopID, stopOrder, stopName, arsID, busID, busName, memo, cdate FROM TMyBus where stopID = ";
		query.Append("'");
		query.Append(pStop->sId);
		query.Append("'");

		//
		DbEnumerator* pEnum = __pDb->QueryN(query);
		if( pEnum != null )  {
			while( pEnum->MoveNext()== E_SUCCESS )	{
				MybusItem * pItem = new MybusItem();
				int index = 0;

				//
				pEnum->GetStringAt(index++, pItem->__stationId);
				pEnum->GetStringAt(index++, pItem->__stationOrd);
				pEnum->GetStringAt(index++, pItem->__stationNm);
				pEnum->GetStringAt(index++, pItem->__arsId);
				pEnum->GetStringAt(index++, pItem->__busId);
				pEnum->GetStringAt(index++, pItem->__busName);
				pEnum->GetStringAt(index++, pItem->__memo);
				pEnum->GetStringAt(index++, pItem->__cdate);

				pStop->sId2 = pItem->__arsId;  // 정석은 아니지만, 꼼수로 바꾸었다!
				pStop->sName = pItem->__stationNm;

				pStop->pArr->Add(*pItem);
			}

			delete pEnum;
		}
		//
	}

	return true;
}

