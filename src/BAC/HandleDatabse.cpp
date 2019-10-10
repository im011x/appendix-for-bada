/*
 * HandleDatabse.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "HandleDatabse.h"

HandleDatabse::HandleDatabse(const String & fileName, bool createIfNotExist) : __fileNameDb(fileName), __createIfNotExist(createIfNotExist), __pDb(null)
{
}

HandleDatabse::~HandleDatabse()
{
	if (__pDb) {
		Close();
	}
}

bool HandleDatabse::isOpen()
{
	return !(__pDb == null);
}

bool HandleDatabse::Open()
{
	if (__pDb) {
		AppLogDebug("이미 DB가 오픈 되어 있습니다.");
		return true;
	}

	__pDb = new Database();

	result r = __pDb->Construct(__fileNameDb, __createIfNotExist);

	if (IsFailed(r)) {
		AppLogDebug("DB open 실패 - 에러코드: %d", r);
		Close();
		return false;
	}

	//AppLogDebug("@@@ DB open !");
	return true;
}

void HandleDatabse::Close()
{
	if (__pDb) {
		//AppLogDebug("@@@ DB close !");
		delete __pDb;
		__pDb = null;
	}
}

bool HandleDatabse::Execute(const String & query)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppLogDebug("execute DB command : [%S]", query.GetPointer());

	result r = __pDb->ExecuteSql(query, true);

	if (IsFailed(r)) {
		AppLogDebug("fail on execute the query (%S) - error code = %d", r);
		return false;
	}

	return true;
}

bool HandleDatabse::ExecuteWithOpts(const String & query, ArrayList &arrArgs)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	__pDb->BeginTransaction();

	DbStatement* pStmt = __pDb->CreateStatementN(query);

	for (int i=0; i<arrArgs.GetCount(); i++) {
		pStmt->BindString(i, *(static_cast<String*> (arrArgs.GetAt(i))));
	}

	ClearLastResult();

	DbEnumerator* pEnum = __pDb->ExecuteStatementN(*pStmt);

	int neffected_row = 0;
	if (pEnum == null) {
		result r = GetLastResult();
		if (r == E_SUCCESS)
			neffected_row = 1;
		//else
		//	AppLogDebug("ERROR! - %s", GetErrorMessage(r));
	}

	if( pEnum != null )  {
		pEnum->Reset();

		while( pEnum->MoveNext()== E_SUCCESS )	{
			neffected_row++;
		}
	}

	if (pEnum != null)
		delete pEnum;

	if (pStmt != null)
		delete pStmt;

	__pDb->CommitTransaction();

	return (neffected_row > 0);
}

