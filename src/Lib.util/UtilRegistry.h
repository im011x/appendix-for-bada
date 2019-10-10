/*
 * UtilRegistry.h
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#ifndef UTILREGISTRY_H_
#define UTILREGISTRY_H_

#include <FApp.h>
#include <FBase.h>

using namespace Osp::App;
using namespace Osp::Base;

class UtilRegistry
{
public:
	UtilRegistry();
	virtual ~UtilRegistry();

protected:
	AppRegistry 	* 			__pAppRegistry;

protected:
	void 			init();

public:
	void 			RegistryGet(const String & sKey, String & sValueDefault);
	void 			RegistryGet(const String & sKey, int & nValueDefault);

	void			RegistrySet(const String & sKey, const String & sValue);
	void			RegistrySet(const String & sKey, int nValue);

	void			RegistrySave();
	void			RegistryRemove(const String & sKey);
};

#endif /* UTILREGISTRY_H_ */
