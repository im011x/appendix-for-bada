/*
 * HttpTranBase.cpp
 *
 *  Created on: 2012. 1. 18.
 *      Author: SONG
 */

#include "HttpTranBase.h"

Osp::Base::String 		HttpTranBase::__ServiceKey = "gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D";

HttpTranBase::HttpTranBase() : __pHttpHandler(null)
{
/*
	if (__pHttpHandler == null) {
		//__pHttpHandler = new HandleHttp(this);
		__pHttpHandler = new HandleHttp();
		__pHttpHandler->SetResponder(this);
	}
*/
}

HttpTranBase::~HttpTranBase()
{
	if (__pHttpHandler) {
		__pHttpHandler->Cancel();

		delete __pHttpHandler;
		__pHttpHandler = null;
	}
}

//void HttpTranBase::Cancel()
//{
//	if (__pHttpHandler) {
//		__pHttpHandler->Cancel();
//	}
//}

void HttpTranBase::SetResponseHandler(IHttpFirstResponder * p)
{
	if (__pHttpHandler != null) {
		__pHttpHandler->SetResponder(p);
	}
}

void HttpTranBase::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	AppLogDebug("");
}
void HttpTranBase::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	AppLogDebug("");
}
