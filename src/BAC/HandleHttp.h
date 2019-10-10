/*
 * HandleHttp.h
 *
 *  Created on: 2012. 1. 18.
 *      Author: SONG
 */

#ifndef HANDLEHTTP_H_
#define HANDLEHTTP_H_

#include <FNet.h>

using namespace Osp::Base;
using namespace Osp::Net::Http;

class _EXPORT_ HandleHttp;

class _EXPORT_ IHttpFirstResponder
{
public:
	IHttpFirstResponder() {};
	virtual ~IHttpFirstResponder() {};

public:
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer) = 0;
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message) = 0;
};


class _EXPORT_ HandleHttp : public Osp::Net::Http::IHttpTransactionEventListener
{
public:
	HandleHttp();
	virtual ~HandleHttp();

private:
	String 							__requestedUrl;  	//

	HttpSession 	*				__pSession;
	HttpTransaction		* 			__pTransaction;
	IHttpFirstResponder 	* 		__pHttpResponder;


private:
	void 	dumpHeader(HttpHeader * pH);

public:
	void 				SetResponder(IHttpFirstResponder * p)  {  __pHttpResponder = p;  };
	bool 				GetHttp(const Osp::Base::String& uri, const Osp::Base::String & host);
	HttpTransaction	* 	GetTransaction() 	{  return __pTransaction; };
	void 				Cancel();

	String 				GetRequestedUrl() const {  return __requestedUrl;  };

public:
	virtual void OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen);
	virtual void OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r);
	virtual void OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize);
	virtual void OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool bAuthRequired);
	virtual void OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction);
	virtual void OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert);

};

#endif /* HANDLEHTTP_H_ */
