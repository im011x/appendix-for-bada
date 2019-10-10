/*
 * HttpAddress.h
 *
 *  Created on: 2012. 7. 23.
 *      Author: benny
 */

#ifndef HTTPADDRESS_H_
#define HTTPADDRESS_H_

#include "OpenApiBase.h"
#include "HandleHttp.h"
#include <FXml.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Base::Collection;

class HttpAddress
	: public OpenApiBase
{
public:
	HttpAddress(IHttpFirstResponder * pListener);
	virtual ~HttpAddress();

protected:
	bool  		getAddr(const String & category, const String & keyword);
	bool  		getNewAddr(const String & category, const String & keyword);

public:
	bool 		GetAddrByDongName(const String & keyword);
	bool 		GetAddrByBuildingName(const String & keyword);
	bool 		GetAddrByZipCode(const String & keyword);
	bool 		GetAddrByCityName(const String & keyword);

	bool 		GetNewAddrByDongName(const String & keyword);
	bool 		GetNewAddrByRoadName(const String & keyword);
	bool 		GetNewAddrByZipCode(const String & keyword);

protected:
	Osp::Base::String 			__responseYN;
	Osp::Base::String 			__responseCode;
	Osp::Base::String 			__responseMessage;

public:
	Osp::Base::String & 	GetCode()  		{  return __responseCode; };
	Osp::Base::String & 	GetMessage()	{  return __responseMessage; };


protected:
	bool 		parseHeader(xmlDocPtr doc, xmlNodePtr theNode);

	bool 		parseAddrHeader(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseAddrBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo);
	bool 		parseNewAddrHeader(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseNewAddrBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo);

public:
	bool 		ParseAddr(ByteBuffer* pBuffer, ArrayList * pTo);
	bool 		ParseNewAddr(ByteBuffer* pBuffer, ArrayList * pTo);

};

#endif /* HTTPADDRESS_H_ */
