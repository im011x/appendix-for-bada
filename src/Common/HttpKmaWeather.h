/*
 * HttpKmaWeather.h
 *
 *  Created on: 2012. 8. 28.
 *      Author: benny
 */

#ifndef HTTPKMAWEATHER_H_
#define HTTPKMAWEATHER_H_

#include "OpenApiBase.h"
#include <FXml.h>
#include <FWebJson.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Web::Json;
using namespace Osp::Base::Collection;

class HttpKmaWeather : public OpenApiBase
{
public:
	HttpKmaWeather(IHttpFirstResponder * pListener);
	virtual ~HttpKmaWeather();

public:
	String 		__forcastCity;
	String 		__forcastDate;
	String 		__currentCondition;
	String 		__currentTempF;
	String 		__currentTempC;
	String 		__currentHumidity;
	String 		__currentIcon;
	String 		__currentWind;

	static 		ArrayList 	* 		__pARRweathers;
	static 		String 				__sCITYweather;

protected:
	//ArrayList * 	__pArrForcasts;
	String 			__sWantAddress;
	String 			__codeIcon;

protected:
	bool 		parseLocal(xmlDocPtr doc, xmlNodePtr theNode);
	bool 		parseAttr0(xmlNodePtr bodyNode, xmlAttr * theAttr);
	bool 		parseAttr(xmlNodePtr bodyNode, xmlAttr * theAttr);
	void		assocIcon();

public:
	static void 		Init();
	static void 		Term();
	bool 		GetWeather(const String & address);
	bool 		ParseWeather(ByteBuffer* pBuffer);
};

#endif /* HTTPKMAWEATHER_H_ */
