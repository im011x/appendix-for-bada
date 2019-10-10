/*
 * HttpDaum.h
 *
 *  Created on: 2012. 7. 26.
 *      Author: benny
 */

#ifndef HTTPDAUM_H_
#define HTTPDAUM_H_

#include "OpenApiBase.h"
#include <FXml.h>
#include <FWebJson.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Web::Json;
using namespace Osp::Base::Collection;

class HttpDaum
	: public OpenApiBase
{
public:
	HttpDaum(IHttpFirstResponder * pListener);
	virtual ~HttpDaum();

public:
	bool 		GetPosition(const String & address);
	bool 		GetReverse(const String & latitude, const String & longitude);

//protected:
public:
	String 		__totalCount;
	String 		__formattedAddress;
	String 		__lat;
	String 		__lng;

	String 		__nameFormatted;
	String 		__nameCode;
	String 		__name1;
	String 		__name2;
	String 		__name3;

protected:
	bool 		parsePositionItem(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo);
	bool 		parseReverseJson(IJsonValue * pJv);

public:
	bool 		ParsePosition(ByteBuffer* pBuffer, ArrayList * pTo);
	bool 		ParseReverse(ByteBuffer* pBuffer, ArrayList * pTo);
};

#endif /* HTTPDAUM_H_ */
