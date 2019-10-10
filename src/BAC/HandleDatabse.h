/*
 * HandleDatabse.h
 *
 *  Created on: 2012. 8. 7.
 *      Author: benny
 */

#ifndef HANDLEDATABSE_H_
#define HANDLEDATABSE_H_

#include <FBase.h>
#include <FIo.h>
#include <FIoDatabase.h>
#include <FBaseCol.h>

using namespace Osp::Base;
using namespace Osp::Io;
using namespace Osp::Base::Collection;

class HandleDatabse
{
public:
	HandleDatabse(const String & fileName, bool createIfNotExist = true);
	virtual ~HandleDatabse();

protected:
	String 					__fileNameDb;
	bool 					__createIfNotExist;
	Database 	* 			__pDb;

protected:
	bool 			isOpen();

public:
	bool 			Open();
	void 			Close();

	bool 			Execute(const String & query);
	bool 			ExecuteWithOpts(const String & query, ArrayList &arrArgs);

};

#endif /* HANDLEDATABSE_H_ */
