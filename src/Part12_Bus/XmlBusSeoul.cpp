/*
 * XmlBusSeoul.cpp
 *
 *  Created on: 2012. 7. 11.
 *      Author: benny
 */

#include "XmlBusSeoul.h"
#include "DataBusSeoul.h"
#include "UtilApp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParser::ParseXml(ByteBuffer* pBuffer, ArrayList * pTo, bool bUpdate)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"ServiceResult") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppAssert(0);  //
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
			bDone = do_messageBody(doc, curNode->children, pTo, bUpdate);
		}

		//
		curNode = curNode->next;

		if (bDone) {
			return false;
		}
	}

	return true;
}

bool XmlBusSeoulParser::do_commonHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	return false;
}

bool XmlBusSeoulParser::do_messageHeader(xmlDocPtr doc, xmlNodePtr theNode)
{
	xmlNodePtr headerNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (headerNode) {
		//AppLogDebug("message Header Name = [%s]", headerNode->name);

		xValue = xmlNodeListGetString(doc, headerNode->xmlChildrenNode, 1);

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
	//AppLogDebug("응답 메시지 = [%S]", __responseMessage.GetPointer());

	return (__responseCode.CompareTo("0") != 0);
}

bool XmlBusSeoulParser::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	AppLogDebug("XmlBusSeoulParser를 상속받아 do_messageBody 함수를 별도로 구현해야 합니다.");
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseLinesByStop::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	AppLogDebug("XmlBusSeoulParseLinesByStop::do_messageBody");

	while (bodyNode) {
		//AppLogDebug("message Body Name = [%s]", bodyNode->name);
		//BusSeoul_LineInfo 	* pLineInfo = new BusSeoul_LineInfo();
		BusSeoul_LineDetail * pLineInfo = new BusSeoul_LineDetail();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {
			//AppLogDebug("  -- message Body = [%s]", contentNode->name);

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);
			//Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, sValue);
			//AppLogDebug("    + value (UTF8)/(String) = [%s]/[%S]", xValue, sValue.GetPointer());

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__busRouteNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__routeType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__firstBusTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lastBusTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTmLow") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__firstLowBusTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTmLow") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lastLowBusTm);
			}


			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"length") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__length);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stBegin") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__stStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stEnd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__edStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__term);
			}

			//else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nextBusYN") == 0)	{
			//	pLineInfo->__isLastBus = (xmlStrcmp(xValue, (const xmlChar *)"Y") == 0);  //
			//}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		pTo->Add(*pLineInfo);

		bodyNode = bodyNode->next;
	}

	AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseTimeByLineByStop::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	String 	arsId, busRouteId, busRouteNm, firstBusTm, lastBusTm;

	while (bodyNode) {

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {
			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"arsId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, arsId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, busRouteNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, firstBusTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, lastBusTm);
			}
			//else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTmLow") == 0)	{
			//	Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__firstLowBusTm);
			//}
			//else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTmLow") == 0)	{
			//	Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pLineInfo->__lastLowBusTm);
			//}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		//pTo->Add(*pLineInfo);

		bodyNode = bodyNode->next;
	}

	//AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());
	//BusSeoul_LineDetail * pLineInfo = new BusSeoul_LineDetail();
	//String 	arsId, busRouteId, busRouteNm, firstBusTm, lastBusTm;
	AppLogDebug("정류소 ID = %S", arsId.GetPointer());
	AppLogDebug("버스 ID = %S", busRouteId.GetPointer());
	AppLogDebug("버스 이름 = %S", busRouteNm.GetPointer());
	AppLogDebug("첫차 = %S", firstBusTm.GetPointer());
	AppLogDebug("막차 = %S", lastBusTm.GetPointer());

	for (int i=0; i<pTo->GetCount(); i++) {
		BusSeoul_LineDetail * pLineInfo = static_cast<BusSeoul_LineDetail *> (pTo->GetAt(i));

		if (pLineInfo->__busRouteId.CompareTo(busRouteId) == 0) {
			pLineInfo->__firstBusTmCurr = firstBusTm;
			pLineInfo->__lastBusTmCurr = lastBusTm;

			String sDate1Y, sDate1M, sDate1D, sDate2Y, sDate2M, sDate2D;
			String sTime1H, sTime1M, sTime1S, sTime2H, sTime2M, sTime2S;

			/*
			if (pLineInfo->__firstBusTm.GetLength() > 12) {
				pLineInfo->__firstBusTm.SubString( 0, 4, sDate1Y);
				pLineInfo->__firstBusTm.SubString( 4, 2, sDate1M);
				pLineInfo->__firstBusTm.SubString( 6, 2, sDate1D);
				pLineInfo->__firstBusTm.SubString( 8, 2, sTime1H);
				pLineInfo->__firstBusTm.SubString(10, 2, sTime1M);
				pLineInfo->__firstBusTm.SubString(12, 2, sTime1S);
			}

			if (pLineInfo->__lastBusTm.GetLength() > 12) {
				pLineInfo->__lastBusTm.SubString( 0, 4, sDate2Y);
				pLineInfo->__lastBusTm.SubString( 4, 2, sDate2M);
				pLineInfo->__lastBusTm.SubString( 6, 2, sDate2D);
				pLineInfo->__lastBusTm.SubString( 8, 2, sTime2H);
				pLineInfo->__lastBusTm.SubString(10, 2, sTime2M);
				pLineInfo->__lastBusTm.SubString(12, 2, sTime2S);
			}

			pLineInfo->__firstBusTmCurrTTS.Format(32, L"첫차 %S:%S", sTime1H.GetPointer(), sTime1M.GetPointer());
			pLineInfo->__lastBusTmCurrTTS.Format(32, L"막차 %S:%S", sTime2H.GetPointer(), sTime2M.GetPointer());
			 */

			//sTime1 = "현재 정류소의 첫차/막차 정보를";
			//sTime2 = " 확인하려면 이곳을 누르세요~";

			if (!pLineInfo->__firstBusTmCurr.IsEmpty()) {
				pLineInfo->__firstBusTmCurr.SubString( 8, 2, sTime1H);
				pLineInfo->__firstBusTmCurr.SubString(10, 2, sTime1M);
				pLineInfo->__firstBusTmCurrTTS.Format(255, L"오늘 첫차는 %S 시 %S 분 입니다.", sTime1H.GetPointer(), sTime1M.GetPointer());
			}
			if (!pLineInfo->__lastBusTmCurr.IsEmpty()) {
				pLineInfo->__lastBusTmCurr.SubString( 0, 4, sDate2Y);
				pLineInfo->__lastBusTmCurr.SubString( 4, 2, sDate2M);
				pLineInfo->__lastBusTmCurr.SubString( 6, 2, sDate2D);
				pLineInfo->__lastBusTmCurr.SubString( 8, 2, sTime2H);
				pLineInfo->__lastBusTmCurr.SubString(10, 2, sTime2M);
				pLineInfo->__lastBusTmCurr.SubString(12, 2, sTime2S);
				pLineInfo->__lastBusTmCurrTTS.Format(255, L"그리고 막차는 %S 월 %S 일 %S 시 %S 분 입니다.", sDate2M.GetPointer(), sDate2D.GetPointer(), sTime2H.GetPointer(), sTime2M.GetPointer());
			}

			break;
		}

	}

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseArrivesByStop::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	DateTime dtNow = UtilApp::GetNow();
	AppLogDebug("@@@ time Now = %04d-%02d-%02d %02d:%02d:%02d", dtNow.GetYear(), dtNow.GetMonth(), dtNow.GetDay(), dtNow.GetHour(), dtNow.GetMinute(), dtNow.GetSecond());

	while (bodyNode) {
		BusSeoul_ArrivingInfo 	* pInfo = new BusSeoul_ArrivingInfo();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"arsId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__arsId);
			} // 1
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busType1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busType1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busType2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busType2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__firstTm);
			} // 5
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsX") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsX);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsY") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsY);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isArrive1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isArrive1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isArrive2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isArrive2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isLast1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isLast1);
			} // 10
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isLast2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isLast2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"plainNo1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__plainNo1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"plainNo2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__plainNo2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"routeType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__routeType);
			} // 15
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rtNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__rtNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectOrd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectOrd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectOrd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectOrd2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stNm);
			} // 20
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"staOrd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__staOrd);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationTp") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationTp);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__term);
			} // 25
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traSpd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traSpd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traSpd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traSpd2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traTime1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traTime1);
				int nsec;
				Integer::Parse(pInfo->__traTime1, nsec);
				pInfo->__datetime1 = dtNow;
				pInfo->__datetime1.AddSeconds(nsec);
				//AppLogDebug("현재시간(1) %S 의 %d 초후 시간은 %S", dtNow.ToString().GetPointer(), nsec, pInfo->__datetime1.ToString().GetPointer());
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traTime2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traTime2);
				int nsec;
				Integer::Parse(pInfo->__traTime2, nsec);
				pInfo->__datetime2 = dtNow;
				pInfo->__datetime2.AddSeconds(nsec);
				//AppLogDebug("현재시간(2) %S 의 %d 초후 시간은 %S", dtNow.ToString().GetPointer(), nsec, pInfo->__datetime2.ToString().GetPointer());
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"vehId1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__vehId1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"vehId2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__vehId2);
			}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		// check for update or new
		bool bItsNew = true;
		BusSeoul_ArrivingInfo * pOLD = null;

		for (int i=0; i<pTo->GetCount(); i++) {
			pOLD = static_cast<BusSeoul_ArrivingInfo*> (pTo->GetAt(i));
			if (pOLD->__busRouteId.CompareTo(pInfo->__busRouteId) == 0) {
				bItsNew = false;
				break;
			}
		}

		if (bItsNew == true) {
			pTo->Add(*pInfo);
		}
		else {
			pOLD->Overwrite(pInfo);
			delete pInfo;
		}

		bodyNode = bodyNode->next;
	}

	AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseArrivesByLine::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	DateTime dtNow = UtilApp::GetNow();
	AppLogDebug("@@@ time Now = %04d-%02d-%02d %02d:%02d:%02d", dtNow.GetYear(), dtNow.GetMonth(), dtNow.GetDay(), dtNow.GetHour(), dtNow.GetMinute(), dtNow.GetSecond());

	while (bodyNode) {
		BusSeoul_ArrivingByLine 	* pInfo = new BusSeoul_ArrivingByLine();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"arsId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__arsId);
			} // 1
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"avgCf1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__avgCf1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"avgCf2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__avgCf2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"brdrde_Num1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__brdrde_Num1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"brdrde_Num2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__brdrde_Num2);
			} // 5
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"brerde_Div1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__brerde_Div1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"brerde_Div2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__brerde_Div2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busType1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busType1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busType2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busType2);
			} // 10
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"dir") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__dir);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"expCf1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__expCf1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"expCf2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__expCf2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"exps1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__exps1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"exps2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__exps2);
			} // 15
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__firstTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"full1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__full1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"full2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__full2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"goal1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__goal1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"goal2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__goal2);
			} // 20
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isArrive1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isArrive1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isArrive2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isArrive2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isLast1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isLast1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isLast2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isLast2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"kalCf1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__kalCf1);
			} // 25
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"kalCf2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__kalCf2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"kals1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__kals1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"kals2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__kals2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"mkTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__mkTm);
			} // 30
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"namin2Sec1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__namin2Sec1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"namin2Sec2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__namin2Sec2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"neuCf1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__neuCf1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"neuCf2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__neuCf2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"neus1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__neus1);
			} // 35
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"neus2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__neus2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain2Ord1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain2Ord1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain2Ord2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain2Ord2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain2Stnid1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain2Stnid1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain2Stnid2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain2Stnid2);
			} // 40
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Ord1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Ord1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Ord2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Ord2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Sec1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Sec1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Sec2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Sec2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Stnid1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Stnid1);
			} // 45
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmain3Stnid2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmain3Stnid2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainOrd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainOrd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainOrd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainOrd2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainSec1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainSec1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainSec2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainSec2);
			} // 50
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainStnid1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainStnid1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nmainStnid2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nmainStnid2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnId1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnId1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnId2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnId2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnOrd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnOrd1);
			} // 55
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnOrd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnOrd2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnSec1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnSec1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnSec2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnSec2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnSpd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnSpd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nstnSpd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nstnSpd2);
			} // 60
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"plainNo1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__plainNo1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"plainNo2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__plainNo2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"repTm1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__repTm1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"repTm2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__repTm2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rerdie_Div1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__rerdie_Div1);
			} // 65
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rerdie_Div2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__rerdie_Div2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"reride_Num1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__reride_Num1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"reride_Num2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__reride_Num2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"routeType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__routeType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rtNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__rtNm);
			} // 70
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectOrd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectOrd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectOrd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectOrd2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"staOrd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__staOrd);
			} // 75
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm2);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__term);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traSpd1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traSpd1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traSpd2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traSpd2);
			}  // 80
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traTime1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traTime1);
				int nsec;
				Integer::Parse(pInfo->__traTime1, nsec);
				pInfo->__datetime1 = dtNow;
				pInfo->__datetime1.AddSeconds(nsec);
				//AppLogDebug("현재시간(1) %S 의 %d 초후 시간은 %S", dtNow.ToString().GetPointer(), nsec, pInfo->__datetime1.ToString().GetPointer());
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"traTime2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__traTime2);
				int nsec;
				Integer::Parse(pInfo->__traTime2, nsec);
				pInfo->__datetime2 = dtNow;
				pInfo->__datetime2.AddSeconds(nsec);
				//AppLogDebug("현재시간(2) %S 의 %d 초후 시간은 %S", dtNow.ToString().GetPointer(), nsec, pInfo->__datetime2.ToString().GetPointer());
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"vehId1") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__vehId1);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"vehId2") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__vehId2);
			}  // 84

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		// check for update or new
		bool bItsNew = true;
		BusSeoul_ArrivingByLine * pOLD = null;

		for (int i=0; i<pTo->GetCount(); i++) {
			pOLD = static_cast<BusSeoul_ArrivingByLine*> (pTo->GetAt(i));
			if (pOLD->__busRouteId.CompareTo(pInfo->__busRouteId) == 0 && pOLD->__arsId.CompareTo(pInfo->__arsId) == 0)
			{
				bItsNew = false;
				break;
			}
		}

		if (bItsNew == true) {
			pTo->Add(*pInfo);
		}
		else {
			pOLD->Overwrite(pInfo);
			delete pInfo;
		}


		bodyNode = bodyNode->next;
	}

	AppLogDebug("XmlBusSeoulParseArrivesByLine 응답  갯수   = [%d]", pTo->GetCount());

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 전체 노선 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseLines::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {

		BusSeoul_Line 	* pInfo = new BusSeoul_Line();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"edStationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__edStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"routeType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__routeType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stStationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__term);
			}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

#if 0
		int typeIndex = 0;
		Integer::Parse(pInfo->__routeType, typeIndex);

		if (__pArrGroup[typeIndex] == null) {
			__pArrGroup[typeIndex] = new ArrayList();
			__pArrGroup[typeIndex]->Construct();
			pTo->Add(*__pArrGroup[typeIndex]);
		}
		__pArrGroup[typeIndex]->Add(*pInfo);
#else
		pTo->Add(*pInfo);
#endif
		bodyNode = bodyNode->next;
	}

	AppLogDebug("응답  갯수   = [%d]", pTo->GetCount());

	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 특정 노선 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseLineInfo::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {
		BusSeoul_LineDetail * pInfo = new BusSeoul_LineDetail();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"edStationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__edStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"routeType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__routeType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stStationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stStationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"term") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__term);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"length") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__length);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"firstBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__firstBusTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastBusTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastBusTm);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 특정 노선의 위치  정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParsePositionInfo::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	pTo->RemoveAll(true);

	while (bodyNode) {
		BusSeoul_BusPosition * pInfo = new BusSeoul_BusPosition();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"busType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"dataTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__dataTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"fullSectDist") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__fullSectDist);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsX") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsX);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsY") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsY);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"isrunyn") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__isrunyn);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastStTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastStTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastStnId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastStnId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastStnOrd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastStnOrd);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lstbusyn") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lstbusyn);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"nextStTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__nextStTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"plainNo") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__plainNo);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"rtDist") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__rtDist);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectDist") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectDist);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectOrd") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectOrd);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"sectionId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__sectionId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stopFlag") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stopFlag);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"trnstnid") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__trnstnid);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"vehId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__vehId);
			}

			xmlFree(xValue);
			contentNode = contentNode->next;
		}

		pTo->Add(*pInfo);

		bodyNode = bodyNode->next;
	}

	AppLogDebug("특정노선 버스의 현재 위치 응답  갯수   = [%d]", pTo->GetCount());

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 특정 노선의 정류소 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseLineStops::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {
		BusSeoul_StopByLine 	* pInfo = new BusSeoul_StopByLine();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			//AppLogDebug("parsing xml [attr: value] = %s[%s]", contentNode->name, xValue);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"beginTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__beginTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"busRouteNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__busRouteNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"direction") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__direction);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsX") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsX);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsY") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsY);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"lastTm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__lastTm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"routeType") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__routeType);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"section") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__section);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"seq") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__seq);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"station") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__station);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNo") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNo);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"fullSectDist") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__fullSectDist);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"trnstnid") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__trnstnid);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 특정이름 정류소 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseStopName::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {
		BusSeoul_StopInfo 	* pInfo = new BusSeoul_StopInfo();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"arsId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__arsId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"tmX") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsX);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"tmY") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsY);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm);
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 주변 정류소 정보
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool XmlBusSeoulParseNearStops::do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;
	Osp::Base::String sValue;

	while (bodyNode) {
		BusSeoul_StopInfo 	* pInfo = new BusSeoul_StopInfo();

		xmlNodePtr contentNode = bodyNode->children;
		while (contentNode) {

			xValue = xmlNodeListGetString(doc, contentNode->xmlChildrenNode, 1);

			//AppLogDebug("parsing xml [attr: value] = %s[%s]", contentNode->name, xValue);

			if(xmlStrcmp(contentNode->name, (const xmlChar *)"arsId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__arsId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsX") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsX);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"gpsY") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__gpsY);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationId") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationId);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationNm") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationNm);
			}
			else if(xmlStrcmp(contentNode->name, (const xmlChar *)"stationTp") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, pInfo->__stationTp);
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
