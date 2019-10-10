/*
 * FormAxCurrency.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXCURRENCY_H_
#define FORMAXCURRENCY_H_

#include "AppBaseForm.h"
#include <FNet.h>
#include <FXml.h>

class FormAxCurrency
	: public AppBaseForm
	, public Osp::Net::Http::IHttpTransactionEventListener
{
public:
	FormAxCurrency();
	virtual ~FormAxCurrency();
	virtual bool AtInitialize();

// Implementation
protected:
	Osp::Net::Http::HttpSession*	__pSession;

protected:
	result	 	invokeQuery();
	void 		parseHtml(ByteBuffer* pBuffer, const char * pCodingScheme);
	void 		extractHyperLinks(Osp::Xml::htmlNodePtr pRoot, int level);


public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	///////////////////////////////////////////////////////////
	// IHttpTransactionEventListener
	///////////////////////////////////////////////////////////
	void OnTransactionReadyToRead(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int availableBodyLen);
	void OnTransactionAborted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, result r);
	void OnTransactionReadyToWrite(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int recommendedChunkSize);
	void OnTransactionHeaderCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, int headerLen, bool rs);
	void OnTransactionCompleted(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction);
	void OnTransactionCertVerificationRequiredN(Osp::Net::Http::HttpSession& httpSession, Osp::Net::Http::HttpTransaction& httpTransaction, Osp::Base::String* pCert);
};

#endif /* FORMAXCURRENCY_H_ */
