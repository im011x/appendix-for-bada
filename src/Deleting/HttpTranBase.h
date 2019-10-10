/*
 * HttpTranBase.h
 *
 *  Created on: 2012. 1. 18.
 *      Author: SONG
 */

#ifndef HTTPTRANBASE_H_
#define HTTPTRANBASE_H_

#include "HandleHttp.h"

class _EXPORT_  HttpTranBase : public IHttpFirstResponder
{
public:
	HttpTranBase();
	virtual ~HttpTranBase();

protected:
	HandleHttp 		* 		__pHttpHandler;

	static Osp::Base::String 	__ServiceKey;

public:
	void 	SetResponseHandler(IHttpFirstResponder * p);

public:
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);
};

#endif /* HTTPTRANBASE_H_ */
