/*
 * DatabaseAddress.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef DATABASEADDRESS_H_
#define DATABASEADDRESS_H_

#include "HandleDatabse.h"

class AddressItem : public Object
{
public:
	int 		__uid;
	String 		__zip;
	String 		__address;
	String 		__addressNew;
	String 		__memo;
	String 		__cdate;
};


class DatabaseAddress : public HandleDatabse
{
public:
	DatabaseAddress();
	virtual ~DatabaseAddress();

private:
	bool 				create();
	void 				destroy();

public:
	bool 				Insert(const String & zipCode, const String & address1, const String & addressNew);
	bool 				Update(const String & memo, int uid);
	bool 				Delete(int uid);
	bool 		  		GetFreqN(ArrayList * parr);

};

#endif /* DATABASEADDRESS_H_ */
