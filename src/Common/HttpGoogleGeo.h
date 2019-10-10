/*
 * HttpGoogleGeo.h
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#ifndef HTTPGEOCODE_H_
#define HTTPGEOCODE_H_

#include "OpenApiBase.h"
#include <FXml.h>
#include <FWebJson.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Web::Json;
using namespace Osp::Base::Collection;

class HttpGoogleGeo: public OpenApiBase
{
public:
	HttpGoogleGeo(IHttpFirstResponder * pListener);
	virtual ~HttpGoogleGeo();

public:
	bool 		GetPosition(const String & address);
	bool 		GetReverse(const String & latitude, const String & longitude);

public:
	String 		__status;
	String 		__formattedAddress;
	String 		__lat;
	String 		__lng;

	String 		__nameFormatted;
	String 		__nameCode;
	String 		__name1;
	String 		__name2;
	String 		__name3;

protected:
	bool 		parseStatus(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parsePositionResult(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseReverseResult(xmlDocPtr doc, xmlNodePtr theNode);

public:
	bool 		ParsePosition(ByteBuffer* pBuffer, ArrayList * pTo);
	bool 		ParseReverse(ByteBuffer* pBuffer, ArrayList * pTo);

};

#endif /* HTTPGEOCODE_H_ */
