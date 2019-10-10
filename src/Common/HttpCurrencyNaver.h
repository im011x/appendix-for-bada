/*
 * HttpCurrencyNaver.h
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#ifndef HTTPCURRENCYNAVER_H_
#define HTTPCURRENCYNAVER_H_

#include "OpenApiBase.h"
#include <FXml.h>
#include "DataCurrency.h"

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Base::Collection;


class HttpCurrencyNaver
	: public OpenApiBase
{
public:
	HttpCurrencyNaver(IHttpFirstResponder * pListener);
	virtual ~HttpCurrencyNaver();

public:
	bool 		GetCurrecny();
	bool 		ParseCurrecny(ByteBuffer* pBuffer, ArrayList * pArr);
	bool 		GetCurrecnyTime();
	bool 		ParseCurrecnyTime(ByteBuffer* pBuffer);

protected:
	ArrayList 			* 		__pCurrArr;
	DataCurrency 		* 		__pCurrCurrency;
	int 						__nCountCurrency;

protected:
	bool 		parseHtml(ByteBuffer* pBuffer, const char * pCodingScheme);
	bool 		parseHtml2(ByteBuffer* pBuffer, const char * pCodingScheme);
	void 		extractHyperLinks(Osp::Xml::htmlNodePtr pRoot, int level);
	void 		scanHtml(Osp::Xml::htmlNodePtr pRoot, int level);

};

#endif /* HTTPCURRENCYNAVER_H_ */
