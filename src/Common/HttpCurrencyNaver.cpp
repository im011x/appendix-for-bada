/*
 * HttpCurrencyNaver.cpp
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#include "HttpCurrencyNaver.h"

HttpCurrencyNaver::HttpCurrencyNaver(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
	__pCurrArr = null;
	__pCurrCurrency = null;
	__nCountCurrency = 0;
}

HttpCurrencyNaver::~HttpCurrencyNaver()
{
}

bool HttpCurrencyNaver::GetCurrecny()
{
	String theUrl("http://info.finance.naver.com/marketindex/exchangeList.nhn");
	return __pHttpHandler->GetHttp(theUrl, "http://info.finance.naver.com");
}

bool HttpCurrencyNaver::GetCurrecnyTime()
{
	String theUrl("http://info.finance.naver.com/marketindex/exchangeMain.nhn?exchangeType=exchange1");
	return __pHttpHandler->GetHttp(theUrl, "http://info.finance.naver.com");
}

bool HttpCurrencyNaver::ParseCurrecny(ByteBuffer* pBuffer, ArrayList * pArr)
{
	__pCurrArr = pArr;

	return parseHtml(pBuffer, "utf-8");
}

bool HttpCurrencyNaver::ParseCurrecnyTime(ByteBuffer* pBuffer)
{
	return parseHtml2(pBuffer, "utf-8");
}

bool HttpCurrencyNaver::parseHtml(ByteBuffer* pBuffer, const char * pCodingScheme)
{
	htmlDocPtr pDoc = NULL;
	htmlNodePtr pRoot = NULL;

	pDoc = htmlParseDoc((xmlChar*) pBuffer->GetPointer(), pCodingScheme);
	if (!pDoc) {
		AppLogDebug("fail to parse the HTML document!");
		return false;
	}

	pRoot = xmlDocGetRootElement(pDoc);
	if (!pRoot) {
		AppLogDebug("");
		return false;
	}

	extractHyperLinks(pRoot, 0);

	AppLogDebug("### OK !!!");
	return true;
}

bool HttpCurrencyNaver::parseHtml2(ByteBuffer* pBuffer, const char * pCodingScheme)
{
#if 0
	htmlDocPtr pDoc = NULL;
	htmlNodePtr pRoot = NULL;

	pDoc = htmlParseDoc((xmlChar*) pBuffer->GetPointer(), pCodingScheme);
	if (!pDoc) {
		AppLogDebug("fail to parse the HTML document!");
		return false;
	}

	pRoot = xmlDocGetRootElement(pDoc);
	if (!pRoot) {
		AppLogDebug("");
		return false;
	}

	//extractHyperLinks(pRoot, 0);
	scanHtml(pRoot, 0);
#else
	int index1 = -1, index2 = -1;

	const char * pSRC = (const char*) pBuffer->GetPointer();
	size_t nSRC = pBuffer->GetLimit();

	AppLogDebug("### parsing ... source length = %d", nSRC);

	const char * pALIST = "<a name=\"list\">";
	const char * pTINFO = "<ul class=\"tbl_info\">";
	//const char * pALIST = "language=javascript";
	//const char * pTINFO = "exchange";

	for (size_t i=0; i<nSRC - strlen(pALIST); i++) {
		if (memcmp(pSRC+i, pALIST, strlen(pALIST)) == 0) {
			index1 = (int) i;
			AppLogDebug("### found index 1 = %d", index1);
			break;
		}
	}
	AppLogDebug("### parsing ... source = %s", pSRC);

	if (index1 < 0) {
		AppLogDebug("### fail to parse 1");
		return false;
	}

	for (size_t i=index1; i<nSRC - strlen(pTINFO); i++) {
		if (memcmp(pSRC+i, pTINFO, strlen(pTINFO)) == 0) {
			index2 = (int) i;
			break;
		}
	}

	if (index2 < 0) {
		AppLogDebug("### fail to parse 2");
		return false;
	}

	AppLogDebug("### extracting from %d to %d ...", index1, index2);

	/*
	String docHtml, docInteresting;
	Osp::Base::Utility::StringUtil::Utf8ToString ((char*) pBuffer->GetPointer(), docHtml);
	//docHtml = (const char*) pBuffer->GetPointer();

	if (docHtml.IndexOf("<a name=\"list\">", 0, index1) != E_SUCCESS) {
		AppLogDebug("### fail to parse 1");
		return false;
	}
	if (docHtml.IndexOf("<ul class=\"tbl_info\">", 0, index2) != E_SUCCESS) {
		AppLogDebug("### fail to parse 2");
		return false;
	}

	if (index1 >= index2) {
		AppLogDebug("### fail to parse 3");
		return false;
	}

	docHtml.SubString(index1, index2-index1, docInteresting);

	AppLogDebug("%S", docInteresting.GetPointer());
	 */

#endif

	AppLogDebug("### OK !!!");
	return true;
}

void HttpCurrencyNaver::extractHyperLinks(Osp::Xml::htmlNodePtr pRoot, int level)
{
	xmlNodePtr pCurrentElement = NULL;
	xmlAttrPtr attr;

	String 	nameString;

	for(pCurrentElement = pRoot; pCurrentElement; pCurrentElement = pCurrentElement->next)	{
		if(pCurrentElement->type == XML_ELEMENT_NODE)	{

			if (pCurrentElement->content) {
				AppLogDebug("[%d] node name/value = [%s]/[%s]", level, pCurrentElement->name, pCurrentElement->content);
			}
			else {
				AppLogDebug("[%d] node name = [%s]", level, pCurrentElement->name);
			}

			for(attr = pCurrentElement->properties; attr != NULL; attr = attr->next)	{
				/*
				if(xmlStrcasecmp(attr->name, (const xmlChar*)"href") == 0)
				{
					Osp::Base::Utility::StringUtil::Utf8ToString ((char*)pCurrentElement->children->content, *nodeName);
					//__pList->AddItem(&nodeName, null, null, null);
					//__pItemList->Add(*nodeName);
					AppLogDebug("<a HREF ... node name = [%S]", nodeName->GetPointer());
				}
				 */
				AppLogDebug("  [%d] ---- attr name/value = [%s]/[%s]", level, attr->name, attr->children->content);
			}

			if(pCurrentElement->children != NULL)	{
				extractHyperLinks(pCurrentElement->children, level+1);
			}
		}
		else if (pCurrentElement->type == XML_TEXT_NODE) {

			// 일단 바꾸자!
			Osp::Base::Utility::StringUtil::Utf8ToString ((char*) pCurrentElement->content, nameString);
			nameString.Trim();

			//AppLogDebug("  ==== value utf-8  = %s", pCurrentElement->content);
			//AppLogDebug("  ==== value string = %S", nameString.GetPointer());

			if (nameString.GetLength() > 0) {
				ByteBuffer* pNameBuffer = Osp::Base::Utility::StringUtil::StringToUtf8N(nameString);
				if (pNameBuffer) {
					AppLogDebug("   [%d] text of [%s] string value = [%s]-[%S]", level, pCurrentElement->parent->name, pNameBuffer->GetPointer(), nameString.GetPointer());
					delete pNameBuffer;
				}
			}

			if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"a") == 0) {
				//AppLogDebug("#### 화폐 ### []");
				AppLogDebug("#### 화폐 ### [%S]", nameString.GetPointer());

				__pCurrCurrency = new DataCurrency();
				__pCurrCurrency->__currencyName = nameString;
				if (__pCurrArr)
					__pCurrArr->Add(*__pCurrCurrency);

			}
			if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"td") == 0) {
				//AppLogDebug("#### 환율 ### []");
				AppLogDebug("#### 환율 ### [%S]", nameString.GetPointer());

				if (__pCurrCurrency) {
					__pCurrCurrency->__currencyRate[__nCountCurrency%7] = nameString;
					__nCountCurrency++;
				}
			}

		}
		//else {
		//	AppLogDebug("*** 미처리된 요소 타입 = %d", pCurrentElement->type);
		//}

	} // for
}

void HttpCurrencyNaver::scanHtml(Osp::Xml::htmlNodePtr pRoot, int level)
{
	xmlNodePtr pCurrentElement = NULL;
	xmlAttrPtr attr;

	String 	nameString;

	for(pCurrentElement = pRoot; pCurrentElement; pCurrentElement = pCurrentElement->next)	{
		if(pCurrentElement->type == XML_ELEMENT_NODE)	{
			if (pCurrentElement->content) {
				AppLogDebug("[%d] node name/value = [%s]/[%s]", level, pCurrentElement->name, pCurrentElement->content);
			}
			else {
				AppLogDebug("[%d] node name = [%s]", level, pCurrentElement->name);
			}

			for(attr = pCurrentElement->properties; attr != NULL; attr = attr->next)	{
				AppLogDebug("  [%d] ---- attr name/value = [%s]/[%s]", level, attr->name, attr->children->content);
			}

			if(pCurrentElement->children != NULL)	{
				scanHtml(pCurrentElement->children, level+1);
			}
		}
		else if (pCurrentElement->type == XML_TEXT_NODE) {
			// 일단 바꾸자!
			Osp::Base::Utility::StringUtil::Utf8ToString ((char*) pCurrentElement->content, nameString);
			nameString.Trim();

			/*
			if (nameString.GetLength() > 0) {
				ByteBuffer* pNameBuffer = Osp::Base::Utility::StringUtil::StringToUtf8N(nameString);
				if (pNameBuffer) {
					AppLogDebug("   [%d] text of [%s] string value = [%s]-[%S]", level, pCurrentElement->parent->name, pNameBuffer->GetPointer(), nameString.GetPointer());
					delete pNameBuffer;
				}

			}

			if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"a") == 0) {
				AppLogDebug("#### <a ### []");
			}
			else if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"td") == 0) {
				AppLogDebug("#### <td ### []");
			}
			else {
			}
			 */

			AppLogDebug("#### parent tag is <%s> and value = %s ###", pCurrentElement->parent->name, pCurrentElement->content);

		}
		else if (pCurrentElement->type == XML_CDATA_SECTION_NODE) {
			AppLogDebug("XML_CDATA_SECTION_NODE name = %s", pCurrentElement->name);
		}
		else {
			AppLogDebug("*** 미처리된 요소 타입 = %d (name = %s)", pCurrentElement->type, pCurrentElement->name);

		}
	}
}
