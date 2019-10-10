/*
 * UtilRegistry.cpp
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#include "UtilRegistry.h"

UtilRegistry::UtilRegistry() : __pAppRegistry(null)
{
}

UtilRegistry::~UtilRegistry()
{
	RegistrySave();
}

void UtilRegistry::init()
{
	if (__pAppRegistry == null) {
		__pAppRegistry = Osp::App::Application::GetInstance()->GetAppRegistry();
	}
}


void UtilRegistry::RegistryGet(const String & sKey, String & sValueDefault)
{
	init();

	if (__pAppRegistry->Get(sKey, sValueDefault) == E_KEY_NOT_FOUND) {
		//AppLogDebug("@@@@ key %S of registry is adding ...", sKey.GetPointer());
		__pAppRegistry->Add(sKey, sValueDefault);
	}
}

void UtilRegistry::RegistryGet(const String & sKey, int & nValueDefault)
{
	init();

	if (__pAppRegistry->Get(sKey, nValueDefault) == E_KEY_NOT_FOUND) {
		__pAppRegistry->Add(sKey, nValueDefault);
	}
}

void UtilRegistry::RegistrySet(const String & sKey, const String & sValue)
{
	init();

	__pAppRegistry->Set(sKey, sValue);
}

void UtilRegistry::RegistrySet(const String & sKey, int nValue)
{
	init();

	__pAppRegistry->Set(sKey, nValue);
}

void UtilRegistry::RegistrySave()
{
	init();
	__pAppRegistry->Save();
}

void UtilRegistry::RegistryRemove(const String & sKey)
{
	init();
	__pAppRegistry->Remove(sKey);
}

