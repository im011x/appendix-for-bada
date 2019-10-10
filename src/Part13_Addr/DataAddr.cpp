/*
 * DatAddr.cpp
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#include "DataAddr.h"

DataAddr::DataAddr()
{
}

DataAddr::~DataAddr()
{
}

result CompareZipAddress::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const DataAddr * p1 = static_cast <const DataAddr*>(&obj1);
	const DataAddr * p2 = static_cast <const DataAddr*>(&obj2);

	//cmp = Osp::Base::String::Compare(p1->__zipCode, p2->__zipCode);
	cmp = Osp::Base::String::Compare(p1->__address, p2->__address);

	return E_SUCCESS;
}
