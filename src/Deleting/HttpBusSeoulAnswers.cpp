/*
 * HttpBusSeoulAnswers.cpp
 *
 *  Created on: 2012. 7. 8.
 *      Author: benny
 */

#include "HttpBusSeoulAnswers.h"

HttpBusSeoulOnLanesByStop::HttpBusSeoulOnLanesByStop()
{
}

HttpBusSeoulOnLanesByStop::~HttpBusSeoulOnLanesByStop()
{
}

bool HttpBusSeoulOnLanesByStop::ParseXml(ByteBuffer* pBuffer)
{
	AppLogDebug("HttpBusSeoul::OnGoodResponse data = [%s]", pBuffer->GetPointer());

	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (xmlStrcmp(curNode->name, (const xmlChar *)"ServiceResult") != 0) {
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"comMsgHeader") == 0) {
			bDone = do_commonHeader(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"msgHeader") == 0) {
			bDone = do_messageHeader(doc, curNode->children);
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"msgBody") == 0) {
			__arrLines.RemoveAll(true);  //  이전에 구해 놓은 노선 정보 목록 삭제
			bDone = do_messageBody(doc, curNode->children);
		}

		//
		curNode = curNode->next;

		if (bDone) {
			return false;
			//break;
		}

	}

	AppLogDebug("================= done parse the xml ");
	return true;
}

bool HttpBusSeoulOnLanesByStop::do_commonHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	return false;
}

bool HttpBusSeoulOnLanesByStop::do_messageHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr headerNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (headerNode) {
		//AppLogDebug("message Header Name = [%s]", headerNode->name);

		xValue = xmlNodeListGetString(doc, headerNode->xmlChildrenNode, 1);

		//Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sValue);
		//AppLogDebug("    + value (UTF8)/(String) = [%s]/[%S]", xValue, sValue.GetPointer());

		if(xmlStrcmp(headerNode->name, (const xmlChar *)"headerCd") == 0)	{
			Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __responseCode);
		}
		if(xmlStrcmp(headerNode->name, (const xmlChar *)"headerMsg") == 0)	{
			Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __responseMessage);
		}
		if(xmlStrcmp(headerNode->name, (const xmlChar *)"itemCount") == 0)	{
		}

		xmlFree(xValue);
		headerNode = headerNode->next;
	}

	AppLogDebug("응답  코드   = [%S]", __responseCode.GetPointer());
	AppLogDebug("응답 메시지 = [%S]", __responseMessage.GetPointer());


/*
	if (__responseCode.CompareTo("0") != 0) {   // there's a problem.
		UtilMessageBox::ConfirmWithCallback("", __responseMessage, __pActionHandler, __ActionID_NoData);
		return true;
	}
	return false;
*/

	return (__responseCode.CompareTo("0") != 0);
}

bool HttpBusSeoulOnLanesByStop::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

/*
	while (bodyNode) {
		//AppLogDebug("message Body Name = [%s]", bodyNode->name);
		BusSeoul_LineInfo 	* pLineInfo = new BusSeoul_LineInfo();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {
			//AppLogDebug("  -- message Body = [%s]", contentNode->name);

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);
			//Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sValue);
			//AppLogDebug("    + value (UTF8)/(String) = [%s]/[%S]", xValue, sValue.GetPointer());

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lineID);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lineName);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lineType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"length") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lineLenght);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stBegin") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__linePlaceFrom);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stEnd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__linePlaceTo);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lineInterval);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nextBusYN") == 0)	{
				pLineInfo->__isLastBus = (xmlStrcmp(xValue, (const xmlChar *)"Y") == 0);  //
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__timeFirstBus);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__timeLastBus);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTmLow") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__timeFirstLowBus);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTmLow") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__timeLastLowBus);
			}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		__arrLines.Add(*pLineInfo);

		bodyNode = bodyNode->next;
	}
*/

	AppLogDebug("응답  노선 정보 갯수   = [%d]", __arrLines.GetCount());

	return false;
}
