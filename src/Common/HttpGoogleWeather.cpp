/*
 * HttpGoogleWeather.cpp
 *
 *  Created on: 2012. 8. 2.
 *      Author: benny
 */

#include "HttpGoogleWeather.h"

HttpGoogleWeather::HttpGoogleWeather(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
	__pArrForcasts = new ArrayList();
	__pArrForcasts->Construct();
}

HttpGoogleWeather::~HttpGoogleWeather()
{
	__pArrForcasts->RemoveAll(true);
	delete __pArrForcasts;
}

bool HttpGoogleWeather::GetWeather(const String & address)
{
	String encodedAddress;
	Osp::Base::Utility::UrlEncoder::Encode(address, "UTF-8", encodedAddress);

	String theUrl("http://www.google.co.kr/ig/api?hl=ko&weather=");
	theUrl.Append(encodedAddress);

	return __pHttpHandler->GetHttp(theUrl, "http://www.google.co.kr");
}

bool HttpGoogleWeather::ParseWeather(ByteBuffer* pBuffer)
{
	AppLogDebug("");

	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for xml_api_reply
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"xml_api_reply") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppLogDebug("구글날씨응답:%s", pBuffer->GetPointer());
		return false;
	}
	curNode = curNode->children;

	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"weather") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppLogDebug("구글날씨응답:%s", pBuffer->GetPointer());
		return false;
	}
	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"forecast_information") == 0) {
			bDone = parseForcastInformation(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"current_conditions") == 0) {
			bDone = parseCurrentCondition(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"forecast_conditions") == 0) {
			bDone = parseForcastCondition(doc, curNode->children, __pArrForcasts);
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}

	AppLogDebug("Google Weather forcast City = %S", __forcastCity.GetPointer());
	AppLogDebug("Google Weather forcast Date = %S", __forcastDate.GetPointer());
	AppLogDebug("Google Weather forcast Condition = %S", __currentCondition.GetPointer());
	AppLogDebug("Google Weather forcast Temperature = %S", __currentTempC.GetPointer());
	AppLogDebug("Google Weather forcast Humidity = %S", __currentHumidity.GetPointer());
	AppLogDebug("Google Weather forcast Icon = %S", __currentIcon.GetPointer());
	AppLogDebug("Google Weather forcast Wind = %S", __currentWind.GetPointer());

	return true;
}

bool HttpGoogleWeather::parseForcastInformation(xmlDocPtr doc, xmlNodePtr bodyNode)
{
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("  -- current Body node = [%s]", bodyNode->name);

		//xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);
		xValue = xmlGetProp(bodyNode, (const xmlChar *) "data");

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"city") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __forcastCity);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"forecast_date") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __forcastDate);
			}

			xmlFree(xValue);
		}

		bodyNode = bodyNode->next;
	}

	return false;
}

bool HttpGoogleWeather::parseCurrentCondition(xmlDocPtr doc, xmlNodePtr bodyNode)
{
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("  -- current Body node = [%s]", bodyNode->name);

		//xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);
		xValue = xmlGetProp(bodyNode, (const xmlChar *) "data");

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"condition") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentCondition);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"temp_f") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentTempF);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"temp_c") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentTempC);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"humidity") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentHumidity);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"icon") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentIcon);
				int indexSlash = -1;
				__currentIcon.LastIndexOf('/', __currentIcon.GetLength()-1, indexSlash);

				if (indexSlash > 0) {
					String sout;
					__currentIcon.SubString(indexSlash, sout);
					__currentIcon = sout;
					__currentIcon.Replace(".gif", ".png");
				}
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"wind_condition") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __currentWind);
			}

			xmlFree(xValue);
		}

		bodyNode = bodyNode->next;
	}

	return false;
}

bool HttpGoogleWeather::parseForcastCondition(xmlDocPtr doc, xmlNodePtr bodyNode, ArrayList * pTo)
{
	xmlChar* xValue;

	GoogleWeatherForcast * pWeather = new GoogleWeatherForcast();

	while (bodyNode) {
		AppLogDebug("  -- current Body node = [%s]", bodyNode->name);

		//xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);
		xValue = xmlGetProp(bodyNode, (const xmlChar *) "data");

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"day_of_week") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pWeather->__dayOfWeek);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"low") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pWeather->__low);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"high") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pWeather->__high);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"icon") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pWeather->__icon);
				int indexSlash = -1;
				pWeather->__icon.LastIndexOf('/', pWeather->__icon.GetLength()-1, indexSlash);

				if (indexSlash > 0) {
					String sout;
					pWeather->__icon.SubString(indexSlash, sout);
					pWeather->__icon = sout;
					pWeather->__icon.Replace(".gif", ".png");
				}
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"condition") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pWeather->__condition);
			}

			xmlFree(xValue);
		}

		bodyNode = bodyNode->next;
	}

	if (pTo)
		pTo->Add(*pWeather);
	else
		delete pWeather;

	return false;
}

