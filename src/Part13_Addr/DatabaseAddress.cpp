/*
 * DatabaseAddress.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "DatabaseAddress.h"
#include "UtilApp.h"

DatabaseAddress::DatabaseAddress() : HandleDatabse("/Home/dbAddress.sqlite")
{
	if (!Database::Exists(__fileNameDb)) {
		if (create() == false) {
			destroy();
		}
	}
}

DatabaseAddress::~DatabaseAddress()
{
}

bool DatabaseAddress::create()
{
	// 일단 DB 파일을 생성한다.
	if (Open() == false) {
		AppLogDebug("DB 파일 생성 실패!");
		return false;
	}

	// 테이블을 생성한다.
	String sql = "CREATE TABLE IF NOT EXISTS TAddressFreq ("
			"  UID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT default 0"
			", zip TEXT NOT NULL"
			", address TEXT"
			", addressNew TEXT"
			", memo TEXT"
			", cdate TEXT"
			")";

	if (Execute(sql) == false) {
		AppLogDebug("DB 테이블 생성 실패!");
		return false;
	}

	AppLogDebug("@@@ DB 생성 성공! @@@");

	Close();

	return true;
}

void DatabaseAddress::destroy()
{
	if (Database::Exists(__fileNameDb))
		Database::Delete(__fileNameDb);
}

bool DatabaseAddress::Insert(const String & zipCode, const String & address1, const String & addressNew)
{
	String sql = "INSERT INTO TAddressFreq (zip, address, addressNew, cdate) VALUES (?, ?, ?, ?)";

	ArrayList * parrOpts = new ArrayList();
	parrOpts->Construct();
	parrOpts->Add(*(new String(zipCode)));
	parrOpts->Add(*(new String(address1)));
	parrOpts->Add(*(new String(addressNew)));
	parrOpts->Add(*(new String(UtilApp::GetNowString())));

	bool bOK = ExecuteWithOpts(sql, *parrOpts);

	parrOpts->RemoveAll(true);
	delete parrOpts;

	return bOK;
}

bool DatabaseAddress::Update(const String & memo, int uid)
{
	String sql;
	sql.Format(128, L"UPDATE TAddressFreq SET memo='%S' where UID = %d", memo.GetPointer(), uid);

	bool bOK = Execute(sql);

	return bOK;
}

bool DatabaseAddress::Delete(int uid)
{
	String sql;
	sql.Format(128, L"DELETE from TAddressFreq where UID = %d", uid);

	bool bOK = Execute(sql);

	return bOK;
}

bool DatabaseAddress::GetFreqN(ArrayList * parr)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	AppLogDebug("");

	AppAssert(parr);
	parr->RemoveAll(true);

	const String query = "SELECT UID, zip, address, addressNew, memo, cdate FROM TAddressFreq";
	DbEnumerator* pEnum = __pDb->QueryN(query);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			AddressItem * pItem = new AddressItem();
			int index = 0;

			pEnum->GetIntAt(index++, pItem->__uid);
			pEnum->GetStringAt(index++, pItem->__zip);
			pEnum->GetStringAt(index++, pItem->__address);
			pEnum->GetStringAt(index++, pItem->__addressNew);
			pEnum->GetStringAt(index++, pItem->__memo);
			pEnum->GetStringAt(index++, pItem->__cdate);

			parr->Add(*pItem);
		}

		delete pEnum;
	}

	AppLogDebug("select count = %d", parr->GetCount());

	return true;
}

