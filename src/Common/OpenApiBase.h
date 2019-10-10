/*
 * OpenApiBase.h
 *
 *  Created on: 2012. 7. 8.
 *      Author: benny
 */

#ifndef OPENAPIBASE_H_
#define OPENAPIBASE_H_

#include "HandleHttp.h"

class OpenApiBase
{
public:
	OpenApiBase(IHttpFirstResponder * pListener);
	virtual ~OpenApiBase();

protected:
	IHttpFirstResponder * 			__pListener;
	HandleHttp * 					__pHttpHandler;

public:
	HandleHttp * 		GetHandler() 	{  return __pHttpHandler; };

protected:
	static Osp::Base::String 			__ServiceKey;
	static Osp::Base::String 			__ServiceKeyDaum;


};

#endif /* OPENAPIBASE_H_ */
