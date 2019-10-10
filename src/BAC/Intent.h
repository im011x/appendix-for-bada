/*
 * Intent.h
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#ifndef INTENT_H_
#define INTENT_H_

#include <FBase.h>

using namespace Osp::Base;
using namespace Osp::Base::Collection;

class Intent {
public:
	Intent();
	virtual ~Intent();

private:
	static 	HashMap *		__PMapRQ;

public:
	static void 		Init_Intent();
	static void 		Fini_Intent();

	void 		InitIntent();
	void 		InitKey(const String & key);

	void 		SetKeyValue(const String & key, long value);
	void 		SetKeyValue(const String & key, const String & value);
	void 		SetKeyObject(const String & key, const Object * pAllocedObject);

	long 		GetValueByKey(const String & key);
	String * 	GetStringByKey(const String & key);
	Object * 	GetObjectByKey(const String & key);
};

#endif /* INTENT_H_ */
