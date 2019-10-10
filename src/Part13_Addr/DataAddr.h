/*
 * DatAddr.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#ifndef DATADDR_H_
#define DATADDR_H_

#include <FBase.h>

class DataAddr : public Osp::Base::Object
{
public:
	DataAddr();
	virtual ~DataAddr();

public:
	Osp::Base::String 		__zipCode;
	Osp::Base::String 		__address;
	Osp::Base::String 		__roadAddress;

};

class CompareZipAddress :  public Osp::Base::Collection::IComparer
{
public:
	CompareZipAddress() {};
	virtual ~CompareZipAddress() {};
public:
	virtual result Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const;
};



#endif /* DATADDR_H_ */
