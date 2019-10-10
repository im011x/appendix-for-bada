/*
 * DatabasePhonebook.cpp
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#include "DatabasePhonebook.h"

DatabasePhonebook::DatabasePhonebook() : HandleDatabse("/Home/publicphones.sqlite", false)
{
	__pArrCategory = new ArrayList();
	__pArrCategory->Construct();

	__pArrCurrPhones = new ArrayList();
	__pArrCurrPhones->Construct();
}

DatabasePhonebook::~DatabasePhonebook()
{
	if (__pArrCategory) {
		__pArrCategory->RemoveAll(true);
		delete __pArrCategory;
	}

	if (__pArrCurrPhones) {
		__pArrCurrPhones->RemoveAll(true);
		delete __pArrCurrPhones;
	}
}

ArrayList * DatabasePhonebook::GetCategories()
{
	if (QueryCategories()) {
		return __pArrCategory;
	}

	return null;
}

ArrayList * DatabasePhonebook::GetPhones(const String & categoryName)
{
	if (QueryPhones(categoryName)) {
		return __pArrCurrPhones;
	}

	return null;
}

bool DatabasePhonebook::QueryCategories()
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	__pArrCategory->RemoveAll(true);

	const String query = "SELECT distinct category FROM PhoneBook;";
	DbEnumerator* pEnum = __pDb->QueryN(query);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			String * pName = new String();
			pEnum->GetStringAt(0, *pName);
			__pArrCategory->Add(*pName);
		}

		delete pEnum;
	}

	return true;
}

bool DatabasePhonebook::QueryPhones(const String & categoryName)
{
	if (!isOpen()) {
		if (!Open()) {
			return false;
		}
	}

	__pArrCurrPhones->RemoveAll(true);

	const String query = "SELECT owner_name, division, num_area, num_station, num_last, home_page, address "
			"FROM PhoneBook where category = ?;";
	DbStatement* pStmt = __pDb->CreateStatementN(query);
	pStmt->BindString(0, categoryName);

	DbEnumerator* pEnum = __pDb->ExecuteStatementN(*pStmt);

	if( pEnum != null )  {
		while( pEnum->MoveNext()== E_SUCCESS )	{
			PhoneItem * pItem = new PhoneItem();
			int index = 0;

			pEnum->GetStringAt(index++, pItem->__owner_name);
			pEnum->GetStringAt(index++, pItem->__division);
			pEnum->GetStringAt(index++, pItem->__num_area); //
			pEnum->GetStringAt(index++, pItem->__num_station); //
			pEnum->GetStringAt(index++, pItem->__num_last);
			pEnum->GetStringAt(index++, pItem->__home_page);
			pEnum->GetStringAt(index++, pItem->__address);

			__pArrCurrPhones->Add(*pItem);
		}

		delete pEnum;
	}

	if (pStmt != null)
		delete pStmt;

	return true;
}

