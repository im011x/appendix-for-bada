/*
 * HttpKmaWeather.cpp
 *
 *  Created on: 2012. 8. 28.
 *      Author: benny
 */

#include "HttpKmaWeather.h"

ArrayList 	* 	HttpKmaWeather::__pARRweathers = null;
String 			HttpKmaWeather::__sCITYweather = "서울";

HttpKmaWeather::HttpKmaWeather(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
//	__pArrForcasts = new ArrayList();
//	__pArrForcasts->Construct();
}

HttpKmaWeather::~HttpKmaWeather()
{
//	__pArrForcasts->RemoveAll(true);
//	delete __pArrForcasts;
}

void HttpKmaWeather::Init()
{
	if (HttpKmaWeather::__pARRweathers == null) {
		HttpKmaWeather::__pARRweathers = new ArrayList();
		HttpKmaWeather::__pARRweathers->Construct();
	}
}

void HttpKmaWeather::Term()
{
	if (HttpKmaWeather::__pARRweathers != null) {
		HttpKmaWeather::__pARRweathers->RemoveAll(true);
		delete HttpKmaWeather::__pARRweathers;
		HttpKmaWeather::__pARRweathers = null;
	}
}

bool HttpKmaWeather::GetWeather(const String & address)
{
	__sWantAddress = address;

	String theUrl("http://www.kma.go.kr/XML/weather/sfc_web_map.xml");

	return __pHttpHandler->GetHttp(theUrl, "http://www.kma.go.kr");
}

bool HttpKmaWeather::ParseWeather(ByteBuffer* pBuffer)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for xml_api_reply
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"current") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppLogDebug("KMA 날씨응답:%s", pBuffer->GetPointer());
		return false;
	}
	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		if (curNode->properties) {
			AppLogDebug("property Name = %s", curNode->properties->name);
			parseAttr0(curNode, curNode->properties);
		}

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"weather") == 0) {
			bDone = parseLocal(doc, curNode->children);
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}

	__forcastCity = __sWantAddress;

	assocIcon();

/*
	AppLogDebug("Google Weather forcast City = %S", __forcastCity.GetPointer());
	AppLogDebug("Google Weather forcast Date = %S", __forcastDate.GetPointer());
	AppLogDebug("Google Weather forcast Condition = %S", __currentCondition.GetPointer());
	AppLogDebug("Google Weather forcast Temperature = %S", __currentTempC.GetPointer());
	AppLogDebug("Google Weather forcast Humidity = %S", __currentHumidity.GetPointer());
	AppLogDebug("Google Weather forcast Icon = %S", __currentIcon.GetPointer());
	AppLogDebug("Google Weather forcast Wind = %S", __currentWind.GetPointer());
*/

	return true;
}

bool HttpKmaWeather::parseLocal(xmlDocPtr doc, xmlNodePtr bodyNode)
{
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("===================================================== [%s]", bodyNode->name);

		if(xmlStrcmp(bodyNode->name, (const xmlChar *)"local") == 0)	{
			//
			xValue = xmlNodeListGetString(doc, bodyNode->children, 1);
			if (xValue) {
				AppLogDebug("[%s] - 도시이름 = [%s]", bodyNode->name, xValue);
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __forcastCity);
				//xValue = 도시이름

				xmlFree(xValue);
			}

			///////////////////////////////////////////////////////////////////////////
			bool bAddableCity = true;
			for (int i=0; i<HttpKmaWeather::__pARRweathers->GetCount(); i++) {
				String *pKept = static_cast<String*> (HttpKmaWeather::__pARRweathers->GetAt(i));
				if (*pKept == __forcastCity) {
					bAddableCity = false;
					break;
				}
			}

			if (bAddableCity) {
				HttpKmaWeather::__pARRweathers->Add(*(new String(__forcastCity)));
			}
			///////////////////////////////////////////////////////////////////////////

			//if (__forcastCity.StartsWith("서울", 0))
			if (__forcastCity.StartsWith(__sCITYweather, 0)) { // 원하는 도시 여부 검사~
				if (bodyNode->properties) {
					parseAttr(bodyNode, bodyNode->properties);
				}
				//break;
			}
		}

		bodyNode = bodyNode->next;
	}

	return false;
}

bool HttpKmaWeather::parseAttr0(xmlNodePtr bodyNode, xmlAttr * theAttr)
{
	xmlChar* xValue;
	xmlAttr * curAttr = theAttr;

	String sY, sM, sD, sH;

	while (curAttr) {
		//AppLogDebug("  -- current Attr name = [%s]", curAttr->name);

		xValue = xmlGetProp(bodyNode, curAttr->name);

		if (xValue) {
			AppLogDebug("  -- [%s] = [%s]", curAttr->name, xValue);

			if(xmlStrcmp(curAttr->name, (const xmlChar *)"year") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sY);
			}
			if(xmlStrcmp(curAttr->name, (const xmlChar *)"month") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sM);
			}
			if(xmlStrcmp(curAttr->name, (const xmlChar *)"day") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sD);
			}
			if(xmlStrcmp(curAttr->name, (const xmlChar *)"hour") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sH);
			}

			xmlFree(xValue);
		}

		curAttr = curAttr->next;
	}

	__forcastDate = sY;
	__forcastDate.Append("-");
	__forcastDate.Append(sM);
	__forcastDate.Append("-");
	__forcastDate.Append(sD);
	__forcastDate.Append(" ");
	__forcastDate.Append(sH);
	__forcastDate.Append(" 시 현재");

	return false;
}

bool HttpKmaWeather::parseAttr(xmlNodePtr bodyNode, xmlAttr * theAttr)
{
	xmlChar* xValue;
	xmlAttr * curAttr = theAttr;

	while (curAttr) {
		//AppLogDebug("  -- current Attr name = [%s]", curAttr->name);

		xValue = xmlGetProp(bodyNode, curAttr->name);
		if (xValue) {
			AppLogDebug("  -- [%s] = [%s]", curAttr->name, xValue);

			if(xmlStrcmp(curAttr->name, (const xmlChar *)"desc") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentCondition);
			}
			if(xmlStrcmp(curAttr->name, (const xmlChar *)"ta") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentTempC);
			}

			if(xmlStrcmp(curAttr->name, (const xmlChar *)"icon") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __codeIcon);
			}

			xmlFree(xValue);
		}

		curAttr = curAttr->next;
	}

	return false;
}

void HttpKmaWeather::assocIcon()
{
	__currentIcon = "/";
	__currentIcon.Append(__codeIcon);
	__currentIcon.Append(".png");

	/*
	int iconCode = 0;
	Integer::Parse(__codeIcon, iconCode);

	switch (iconCode) {
	case 1: // 맑음
		__currentIcon = "/sunny.png";
		break;
	case 2: // 구름 조금
		__currentIcon = "/partly_cloudy.png";
		break;
	case 3: // 구름 많음
		__currentIcon = "/mostly_cloudy.png";
		break;
	case 4: // 흐림
		__currentIcon = "/cloudy.png";
		break;
	case 5: // 비
		__currentIcon = "/rain.png";
		break;
	case 6: // 눈/비
		__currentIcon = "/sleet.png";
		break;
	case 7: // 눈
		__currentIcon = "/snow.png";
		break;
	case 8: // 비
		__currentIcon = "/rain.png";
		break;
	case 17: // 박무
		__currentIcon = "/mist.png";
		break;
	case 18: // 연무
		__currentIcon = "/mist.png";
		break;
	case 0:
	default:
		__currentIcon = "/dust.png";
		break;
	}
	 */

}

