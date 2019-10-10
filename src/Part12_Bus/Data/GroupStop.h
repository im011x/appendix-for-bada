/*
 * GroupStop.h
 *
 *  Created on: 2012. 8. 9.
 *      Author: benny
 */

#ifndef GROUPSTOP_H_
#define GROUPSTOP_H_

#include <FBase.h>
#include <FUi.h>

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Graphics;

class GroupStop : public Object
{
public:
	GroupStop();
	virtual ~GroupStop();

public:
	String 			sId;
	String 			sId2;
	String 			sName;
	ArrayList * 	pArr;  // collection of MybusItem
};

#endif /* GROUPSTOP_H_ */
