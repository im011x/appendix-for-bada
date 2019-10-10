/*
 * HttpGeoCode.cpp
 *
 *  Created on: 2012. 7. 24.
 *      Author: benny
 */

#include "HttpGoogleGeo.h"

HttpGoogleGeo::HttpGoogleGeo(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
}

HttpGoogleGeo::~HttpGoogleGeo()
{
}

bool HttpGoogleGeo::GetPosition(const String & address)
{
	String encodedAddress;
	Osp::Base::Utility::UrlEncoder::Encode(address, "UTF-8", encodedAddress);

	String theUrl("http://maps.googleapis.com/maps/api/geocode/xml?");
	theUrl.Append("address=");
	theUrl.Append(encodedAddress);
	theUrl.Append("&region=kr");
	theUrl.Append("&language=ko");
	theUrl.Append("&sensor=false");

	return __pHttpHandler->GetHttp(theUrl, "http://maps.googleapis.com");
}

bool HttpGoogleGeo::GetReverse(const String & latitude, const String & longitude)
{
	String position = latitude;
	position.Append(",");
	position.Append(longitude);

	String encodedAddress;
	Osp::Base::Utility::UrlEncoder::Encode(position, "UTF-8", encodedAddress);

	String theUrl("http://maps.googleapis.com/maps/api/geocode/xml?");
	//String theUrl("http://maps.googleapis.com/maps/api/geocode/json?");
	theUrl.Append("latlng=");
	theUrl.Append(encodedAddress);
	theUrl.Append("&region=kr");
	theUrl.Append("&language=ko");
	theUrl.Append("&sensor=false");

	return __pHttpHandler->GetHttp(theUrl, "http://maps.googleapis.com");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpGoogleGeo::parseStatus(xmlDocPtr doc, xmlNodePtr theNode)
{
	if (theNode) {
		xmlChar* xValue = xmlNodeListGetString(doc, theNode->xmlChildrenNode, 1);
		if (xValue) {
			if(xmlStrcmp(theNode->name, (const xmlChar *)"status") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __status);
			}
			xmlFree(xValue);
		}
	}

	return !(__status == "OK");
}

bool HttpGoogleGeo::parsePositionResult(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("  -- current Body node = [%s]", bodyNode->name);

		xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"formatted_address") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __formattedAddress);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"geometry") == 0)	{
				parsePositionResult(doc, bodyNode->children);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"location") == 0)	{
				parsePositionResult(doc, bodyNode->children);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"lat") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __lat);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"lng") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __lng);
			}

			xmlFree(xValue);
		}

		bodyNode = bodyNode->next;
	}

	return false;
}

bool HttpGoogleGeo::ParsePosition(ByteBuffer* pBuffer, ArrayList * pTo)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"GeocodeResponse") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppAssert(0);  //
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"status") == 0) {
			bDone = parseStatus(doc, curNode);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"result") == 0) {
			bDone = parsePositionResult(doc, curNode->children);
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}

	AppLogDebug("Google GeoCoding result status = %S", __status.GetPointer());
	AppLogDebug("Google GeoCoding result formatted address = %S", __formattedAddress.GetPointer());
	AppLogDebug("Google GeoCoding result lat = %S", __lat.GetPointer());
	AppLogDebug("Google GeoCoding result lng = %S", __lng.GetPointer());

	return true;
}

bool HttpGoogleGeo::ParseReverse(ByteBuffer* pBuffer, ArrayList * pTo)
{
	//AppLogDebug("%s", pBuffer->GetPointer());
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"GeocodeResponse") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppAssert(0);  //
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"status") == 0) {
			bDone = parseStatus(doc, curNode);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"result") == 0) {
			bDone = parseReverseResult(doc, curNode->children);
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}



	return true;
}

bool HttpGoogleGeo::parseReverseResult(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("  -- current Google result node = [%s]", bodyNode->name);

		xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"formatted_address") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __formattedAddress);
			}
			else if(xmlStrcmp(bodyNode->name, (const xmlChar *)"address_component") == 0)	{
				parseReverseResult(doc, bodyNode->children);
			}

			xmlFree(xValue);
		}

		bodyNode = bodyNode->next;
	}

	//return false;
	return true;  // 제일 첫번째 것만 ...
}
