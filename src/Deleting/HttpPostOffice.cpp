/*
 * HttpPostOffice.cpp
 *
 *  Created on: 2012. 6. 17.
 *      Author: benny
 */

#include "HttpPostOffice.h"
#include "UtilCode.h"
#include <FXml.h>

using namespace Osp::Base::Utility;
using namespace Osp::Xml;

HttpPostOffice::HttpPostOffice(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
}

HttpPostOffice::~HttpPostOffice()
{
}

void HttpPostOffice::GetCodeByAddress(const String & keyword)
{
	ByteBuffer * pKeywordBuffer = UtilCode::Unicode_to_EuckrN(keyword);

	String theUrl("http://openapi.epost.go.kr/postal/retrieveNewAdressService/retrieveNewAdressService/getNewAddressList?searchSe=road&srchwrd=");
	for (int i=0; i<pKeywordBuffer->GetLimit(); i++) {
		byte theByte;
		pKeywordBuffer->GetByte(i, theByte);
		theUrl.Append((char) theByte);
	}

	theUrl.Append("&serviceKey=");
	theUrl.Append(__ServiceKey);

	__pHttpHandler->GetHttp(theUrl, "http://openapi.epost.go.kr");
}

void HttpPostOffice::ParseCodeByAddress(ByteBuffer* pBuffer)
{
	AppLogDebug("HttpPostOffice::ParseCodeByAddress data = [%s]", pBuffer->GetPointer());

    String nodeName;
    String nodeValue;

    xmlDocPtr doc;
	xmlNodePtr curNode;
	xmlChar* key;

	doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	curNode = xmlDocGetRootElement(doc);

	// only for NewAddressListResponse
	if (xmlStrcmp(curNode->name, (const xmlChar *)"NewAddressListResponse") != 0) {
		return;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		if (xmlStrcmp(curNode->name, (const xmlChar *)"cmmMsgHeader") == 0) {
			xmlNodePtr msgNode = curNode->children;

			while (msgNode) {
				AppLogDebug("message Name = %s", msgNode->name);

				if(xmlStrcmp(msgNode->name, (const xmlChar *)"errMsg") == 0)
				{
					key = xmlNodeListGetString(doc, msgNode->xmlChildrenNode, 1);
					AppLogDebug("error message (UTF8)   = [%s]", key);

					Osp::Base::Utility::StringUtil::Utf8ToString((char*)key, nodeValue);
					AppLogDebug("error message (String) = [%S]", nodeValue.GetPointer());
				}

				msgNode = msgNode->next;
			}
		}

		if (xmlStrcmp(curNode->name, (const xmlChar *)"newAddressList") == 0) {
			xmlNodePtr addrNode = curNode->children;

			while (addrNode) {
				AppLogDebug("address Name = %s", addrNode->name);

				if(xmlStrcmp(addrNode->name, (const xmlChar *) "lnmAdres") == 0)
				{
					key = xmlNodeListGetString(doc, addrNode->xmlChildrenNode, 1);
					AppLogDebug("lot address (UTF8)   = [%s]", key);

					Osp::Base::Utility::StringUtil::Utf8ToString((char*)key, nodeValue);
					AppLogDebug("lot address (String) = [%S]", nodeValue.GetPointer());
				}
				else if(xmlStrcmp(addrNode->name, (const xmlChar *) "rnAdres") == 0)
				{
					key = xmlNodeListGetString(doc, addrNode->xmlChildrenNode, 1);
					AppLogDebug("road address (UTF8)   = [%s]", key);

					Osp::Base::Utility::StringUtil::Utf8ToString((char*)key, nodeValue);
					AppLogDebug("road address (String) = [%S]", nodeValue.GetPointer());
				}
				else if(xmlStrcmp(addrNode->name, (const xmlChar *) "zipNo") == 0)
				{
					key = xmlNodeListGetString(doc, addrNode->xmlChildrenNode, 1);
					AppLogDebug("zip code(UTF8)   = [%s]", key);

					Osp::Base::Utility::StringUtil::Utf8ToString((char*)key, nodeValue);
					AppLogDebug("zip code (String) = [%S]", nodeValue.GetPointer());
				}

				addrNode = addrNode->next;
			}
		}

		curNode = curNode->next;
	}

	AppLogDebug("=================");
}

