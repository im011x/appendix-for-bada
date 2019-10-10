/*
 * GroupStop.cpp
 *
 *  Created on: 2012. 8. 9.
 *      Author: benny
 */

#include "GroupStop.h"

GroupStop::GroupStop()
{
	pArr = new ArrayList();
	pArr->Construct();
}

GroupStop::~GroupStop()
{
	pArr->RemoveAll(true);
	delete pArr;
}
