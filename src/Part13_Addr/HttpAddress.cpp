/*
 * HttpAddress.cpp
 *
 *  Created on: 2012. 7. 23.
 *      Author: benny
 */

#include "HttpAddress.h"
#include "UtilCode.h"
#include <FXml.h>
#include "DataAddr.h"

using namespace Osp::Base::Utility;
using namespace Osp::Xml;

HttpAddress::HttpAddress(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
}

HttpAddress::~HttpAddress()
{
}

bool HttpAddress::getAddr(const String & category, const String & keyword)
{
	ByteBuffer * pKeywordBuffer = UtilCode::Unicode_to_EuckrN(keyword);

	String theUrl("http://openapi.epost.go.kr/postal/retrieveLotNumberAdressService/retrieveLotNumberAdressService/getDetailList?srchwrd=");
	for (int i=0; i<pKeywordBuffer->GetLimit(); i++) {
		byte theByte;
		pKeywordBuffer->GetByte(i, theByte);
		theUrl.Append((char) theByte);
	}
	theUrl.Append("&searchSe=");
	theUrl.Append(category);

	theUrl.Append("&serviceKey=");
	theUrl.Append(__ServiceKey);

	return __pHttpHandler->GetHttp(theUrl, "http://openapi.epost.go.kr");
}

bool HttpAddress::getNewAddr(const String & category, const String & keyword)
{
	ByteBuffer * pKeywordBuffer = UtilCode::Unicode_to_EuckrN(keyword);

	String theUrl("http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?srchwrd=");
	for (int i=0; i<pKeywordBuffer->GetLimit(); i++) {
		byte theByte;
		pKeywordBuffer->GetByte(i, theByte);
		theUrl.Append((char) theByte);
	}
	theUrl.Append("&searchSe=");
	theUrl.Append(category);

	theUrl.Append("&serviceKey=");
	theUrl.Append(__ServiceKey);

	return __pHttpHandler->GetHttp(theUrl, "http://openapi.epost.go.kr");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpAddress::GetAddrByDongName(const String & keyword)
{
	return getAddr("dong", keyword);
}

bool HttpAddress::GetAddrByBuildingName(const String & keyword)
{
	return getAddr("dong", keyword);
}

bool HttpAddress::GetAddrByZipCode(const String & keyword)
{
	return getAddr("post", keyword);
}

bool HttpAddress::GetAddrByCityName(const String & keyword)
{
	return getAddr("sido", keyword);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpAddress::GetNewAddrByDongName(const String & keyword)
{
	return getNewAddr("dong", keyword);
}

bool HttpAddress::GetNewAddrByRoadName(const String & keyword)
{
	return getNewAddr("road", keyword);
}

bool HttpAddress::GetNewAddrByZipCode(const String & keyword)
{
	return getNewAddr("post", keyword);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////


bool HttpAddress::ParseAddr(ByteBuffer* pBuffer, ArrayList * pTo)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"DetailListResponse") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppAssert(0);  //
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"cmmMsgHeader") == 0) {
			bDone = parseAddrHeader(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"detailList") == 0) {
			bDone = parseAddrBody(doc, curNode, pTo);
			break;
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}

	return true;
}

bool HttpAddress::ParseNewAddr(ByteBuffer* pBuffer, ArrayList * pTo)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"NewAddressListResponse") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ... %s", curNode->name);
		AppAssert(0);  //
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		//AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"cmmMsgHeader") == 0) {
			bDone = parseNewAddrHeader(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"newAddressList") == 0) {
			bDone = parseNewAddrBody(doc, curNode, pTo);
			break;
		}

		if (bDone) {
			return false;
		}

		//
		curNode = curNode->next;

	}

	return true;
}

bool HttpAddress::parseHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr headerNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (headerNode) {
		//AppLogDebug("message Header Name = [%s]", headerNode->name);

		xValue = xmlNodeListGetString(doc, headerNode->xmlChildrenNode, 1);

		if (xValue) {
			if(xmlStrcmp(headerNode->name, (const xmlChar *)"successYN") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __responseYN);
			}
			if(xmlStrcmp(headerNode->name, (const xmlChar *)"returnCode") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __responseCode);
			}
			if(xmlStrcmp(headerNode->name, (const xmlChar *)"errMsg") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __responseMessage);
			}

			xmlFree(xValue);
		}

		headerNode = headerNode->next;
	}

	AppLogDebug("응답  코드   = [%S]", __responseCode.GetPointer());

	//return (__responseCode.CompareTo("0") != 0);
	return !(__responseYN == "Y");
}

bool HttpAddress::parseAddrHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	return parseHeader(doc, theNode);
}

bool HttpAddress::parseAddrBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	AppAssert(pTo);

	while (bodyNode) {
		DataAddr * pInfo = new DataAddr();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {
			//AppLogDebug("  -- message Body = [%s]", contentNode->name);

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if (xValue) {
				if(xmlStrcmp(contentNode->name, (const xmlChar *)"zipNo") == 0)	{
					Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__zipCode);
				}
				else if(xmlStrcmp(contentNode->name, (const xmlChar *)"adres") == 0)	{
					Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__address);
				}

				xmlFree(xValue);
			}

			contentNode = contentNode->next;
		}

		pTo->Add(*pInfo);

		bodyNode = bodyNode->next;
	}

	AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());

	return true;
}

bool HttpAddress::parseNewAddrHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	return parseHeader(doc, theNode);
}

bool HttpAddress::parseNewAddrBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {
		DataAddr * pInfo = new DataAddr();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {
			//AppLogDebug("  -- message Body = [%s]", contentNode->name);

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"zipNo") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__zipCode);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lnmAdres") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__address);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rnAdres") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__roadAddress);
			}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		pTo->Add(*pInfo);

		bodyNode = bodyNode->next;
	}

	AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());

	return false;
}

