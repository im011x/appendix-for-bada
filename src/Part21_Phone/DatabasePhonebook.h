/*
 * DatabasePhonebook.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef DATABASEPHONEBOOK_H_
#define DATABASEPHONEBOOK_H_

#include "HandleDatabse.h"

class PhoneItem : public Object
{
public:
	String 		__owner_name;
	String 		__division;
	String 		__num_area;
	String 		__num_station;
	String 		__num_last;
	String 		__home_page;
	String 		__address;

};

class DatabasePhonebook : public HandleDatabse
{
public:
	DatabasePhonebook();
	virtual ~DatabasePhonebook();

protected:
	ArrayList 	* 	__pArrCategory;
	ArrayList 	* 	__pArrCurrPhones;

public:
	ArrayList 	*	GetCategories();
	ArrayList 	*	GetPhones(const String & categoryName);

	bool 		QueryCategories();
	bool 		QueryPhones(const String & categoryName);

};

#endif /* DATABASEPHONEBOOK_H_ */
