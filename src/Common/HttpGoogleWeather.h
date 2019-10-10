/*
 * HttpGoogleWeather.h
 *
 *  Created on: 2012. 8. 2.
 *      Author: benny
 */

#ifndef HTTPGOOGLEWEATHER_H_
#define HTTPGOOGLEWEATHER_H_

#include "OpenApiBase.h"
#include <FXml.h>
#include <FWebJson.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Web::Json;
using namespace Osp::Base::Collection;


class GoogleWeatherForcast : public Osp::Base::Object
{
//public:
//	GoogleWeatherForcast() {};
//	virtual ~GoogleWeatherForcast() {};

public:
	Osp::Base::String 		__dayOfWeek;
	Osp::Base::String 		__low;
	Osp::Base::String 		__high;
	Osp::Base::String 		__icon;
	Osp::Base::String 		__condition;
};

class HttpGoogleWeather : public OpenApiBase
{
public:
	HttpGoogleWeather(IHttpFirstResponder * pListener);
	virtual ~HttpGoogleWeather();

public:
	String 		__forcastCity;
	String 		__forcastDate;
	String 		__currentCondition;
	String 		__currentTempF;
	String 		__currentTempC;
	String 		__currentHumidity;
	String 		__currentIcon;
	String 		__currentWind;

	ArrayList * 	__pArrForcasts;

protected:
	bool 		parseForcastInformation(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseCurrentCondition(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseForcastCondition(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo);

public:
	bool 		GetWeather(const String & address);
	bool 		ParseWeather(ByteBuffer* pBuffer);

};

#endif /* HTTPGOOGLEWEATHER_H_ */
