/*
 * Intent.cpp
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#include "Intent.h"

HashMap *		Intent::__PMapRQ = null;

Intent::Intent()
{
}

Intent::~Intent()
{
}

void Intent::Init_Intent()
{
	if (Intent::__PMapRQ == null) {
		Intent::__PMapRQ = new HashMap();
		Intent::__PMapRQ->Construct();
	}
	else
		__PMapRQ->RemoveAll(true);
}

void Intent::Fini_Intent()
{
	if (Intent::__PMapRQ) {
		__PMapRQ->RemoveAll(false);
	}

	delete Intent::__PMapRQ;
	Intent::__PMapRQ = null;
}

void Intent::InitIntent()
{
	if (__PMapRQ)
		__PMapRQ->RemoveAll(false);
}

void Intent::InitKey(const String & key)
{
	bool bIsIn = false;

	__PMapRQ->ContainsKey(key, bIsIn);

	if (bIsIn == true) {
		__PMapRQ->Remove(key, true);
	}
}

void Intent::SetKeyValue(const String & key, long value)
{
	String * pKey = new String(key);
	Long * pValue = new Long(value);

	if (__PMapRQ->GetValue(key) != null) {
		__PMapRQ->Remove(key, true);
	}

	AppLogDebug("Intent::SetKeyValue(%S, %d)", key.GetPointer(), value);

	__PMapRQ->Add(*pKey, *pValue);
}

void Intent::SetKeyValue(const String & key, const String & value)
{
	String * pKey = new String(key);
	String * pVal = new String(value);

	if (__PMapRQ->GetValue(key) != null) {
		__PMapRQ->Remove(key, true);
	}

	__PMapRQ->Add(*pKey, *pVal);
}

void Intent::SetKeyObject(const String & key, const Object * pAllocedObject)
{
	String * pKey = new String(key);

	if (__PMapRQ->GetValue(key) != null) {
		__PMapRQ->Remove(key, false);
	}

	__PMapRQ->Add(*pKey, *pAllocedObject);
}

long Intent::GetValueByKey(const String & key)
{
	Long * pValue = static_cast<Long*> (__PMapRQ->GetValue(key));

	if (pValue == null)
		return 0;

	return pValue->ToLong();
}

String * Intent::GetStringByKey(const String & key)
{
	bool bIsIn = false;

	__PMapRQ->ContainsKey(key, bIsIn);

	if (bIsIn == false) {
		return null;
	}

	return static_cast<String*> (__PMapRQ->GetValue(key));
}

Object * Intent::GetObjectByKey(const String & key)
{
	bool bIsIn = false;

	__PMapRQ->ContainsKey(key, bIsIn);

	if (bIsIn == false) {
		return null;
	}

	return static_cast<Object*> (__PMapRQ->GetValue(key));
}
