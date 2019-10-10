/*
 * GroupBusLine.h
 *
 *  Created on: 2012. 7. 17.
 *      Author: benny
 */

#ifndef GROUPBUSLINE_H_
#define GROUPBUSLINE_H_

#include <FBase.h>
#include <FUi.h>

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Graphics;

class GroupBusLine : public Object
{
public:
	GroupBusLine();
	virtual ~GroupBusLine();

public:
	void 	SetName(const String & key);

	static 	String GetBusTypeName(const String & key);
	static 	Color  GetBusColor(const String & key);

public:
	String 		sName;
	ArrayList * pArr;
};

#endif /* GROUPBUSLINE_H_ */
