/*
 * HttpDaum.cpp
 *
 *  Created on: 2012. 7. 26.
 *      Author: benny
 */

#include "HttpDaum.h"

HttpDaum::HttpDaum(IHttpFirstResponder * pListener) : OpenApiBase(pListener)
{
}

HttpDaum::~HttpDaum() {
}

bool HttpDaum::GetPosition(const String & address)
{
	int foundIndex = -1;
	address.IndexOf('~', 0, foundIndex);

	String address2 = address;
	if (foundIndex > 0) {
		address.SubString(0, foundIndex, address2);
	}

	String encodedAddress;
	Osp::Base::Utility::UrlEncoder::Encode(address2, "UTF-8", encodedAddress);

	// 참조 - http://dna.daum.net/apis/local/geo/addr2coord

	String theUrl("http://apis.daum.net/local/geo/addr2coord?");
	theUrl.Append("q=");
	theUrl.Append(encodedAddress);
	theUrl.Append("&output=xml");
	theUrl.Append("&apikey=");
	theUrl.Append(__ServiceKeyDaum);

	return __pHttpHandler->GetHttp(theUrl, "http://apis.daum.net");
}

bool HttpDaum::GetReverse(const String & latitude, const String & longitude)
{
	// 참조 - http://dna.daum.net/apis/local/geo/coord2addr

	String theUrl("http://apis.daum.net/local/geo/coord2addr?");
	theUrl.Append("inputCoordSystem=WGS84");
	theUrl.Append("&latitude=");
	theUrl.Append(latitude);
	theUrl.Append("&longitude=");
	theUrl.Append(longitude);
	theUrl.Append("&output=json");
	theUrl.Append("&apikey=");
	theUrl.Append(__ServiceKeyDaum);

	return __pHttpHandler->GetHttp(theUrl, "http://apis.daum.net");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpDaum::ParsePosition(ByteBuffer* pBuffer, ArrayList * pTo)
{
	xmlDocPtr doc = xmlParseMemory((char*) pBuffer->GetPointer(), pBuffer->GetLimit());
	xmlNodePtr curNode = xmlDocGetRootElement(doc);

	// only for ServiceResult
	if (curNode == null || xmlStrcmp(curNode->name, (const xmlChar *)"channel") != 0) {
		AppLogDebug("이런일이 발생하면 안되는데 ...");
		AppAssert(0);  //
		return false;
	}

	curNode = curNode->children;

	while (curNode) {
		AppLogDebug("Node Name = %s", curNode->name);

		bool bDone = false;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"totalCount") == 0) {
			xmlChar* xValue = xmlNodeListGetString(doc, curNode->xmlChildrenNode, 1);
			if (xValue) {
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __totalCount);
				xmlFree(xValue);
			}
		}
		else if (xmlStrcmp(curNode->name, (const xmlChar *)"item") == 0) {
			bDone = parsePositionItem(doc, curNode->children, pTo);
		}

		//
		if (bDone) {
			return false;
		}

		curNode = curNode->next;
	}

	AppLogDebug("Daum GeoCoding result total Count = %S", __totalCount.GetPointer());
	AppLogDebug("Daum GeoCoding result address = %S", __formattedAddress.GetPointer());
	AppLogDebug("Daum GeoCoding result lat = %S", __lat.GetPointer());
	AppLogDebug("Daum GeoCoding result lng = %S", __lng.GetPointer());

	return true;
}

bool HttpDaum::parsePositionItem(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo)
{
	xmlNodePtr bodyNode = theNode;
	xmlChar* xValue;

	while (bodyNode) {
		AppLogDebug("  -- current Item node = [%s]", bodyNode->name);

		xValue = xmlNodeListGetString(doc, bodyNode->xmlChildrenNode, 1);

		if (xValue) {
			if(xmlStrcmp(bodyNode->name, (const xmlChar *)"title") == 0)	{
				Osp::Base::Utility::StringUtil::Utf8ToString((char*)xValue, __formattedAddress);
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

	//return false;
	return true;  // 제일 첫번째 것만 ...
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool HttpDaum::ParseReverse(ByteBuffer* pBuffer, ArrayList * pTo)
{
	AppLogDebug("%s", pBuffer->GetPointer());

	IJsonValue * pJson = JsonParser::ParseN(*pBuffer);

	if (pJson == null) {
		AppLogDebug("");
		return false;
	}

#if 0
	parseReverseJson(pJson);
#else
	JsonObject *pJsonObject = static_cast<JsonObject*>(pJson);

	IJsonValue *pValue = null;
	//JsonString *pJsonStringValue = null;

	String key_name  = "name";
	String key_name1 = "name1";
	String key_name2 = "name2";
	String key_name3 = "name3";
	String key_nameF = "fullName";

	if (pJsonObject->GetValue(&key_name, pValue) == E_SUCCESS) {
		JsonString * pJsonStringValueName = static_cast<JsonString*>(pValue);
		if (pJsonStringValueName)
			__nameCode = *pJsonStringValueName;
	}

	if (pJsonObject->GetValue(&key_name1, pValue) == E_SUCCESS) {
		JsonString * pJsonStringValueName1 = static_cast<JsonString*>(pValue);
		if (pJsonStringValueName1)
			__name1 = *pJsonStringValueName1;
	}

	if (pJsonObject->GetValue(&key_name2, pValue) == E_SUCCESS) {
		JsonString * pJsonStringValueName2 = static_cast<JsonString*>(pValue);
		if (pJsonStringValueName2)
			__name2 = *pJsonStringValueName2;
	}

	if (pJsonObject->GetValue(&key_name3, pValue) == E_SUCCESS) {
		JsonString * pJsonStringValueName3 = static_cast<JsonString*>(pValue);
		if (pJsonStringValueName3)
			__name3 = *pJsonStringValueName3;
	}

	if (pJsonObject->GetValue(&key_nameF, pValue) == E_SUCCESS) {
		JsonString * pJsonStringValueNameF = static_cast<JsonString*>(pValue);
		if (pJsonStringValueNameF)
			__nameFormatted = *pJsonStringValueNameF;
	}

	//AppLogDebug("daum reverse result - name  = %S", pJsonStringValueName->GetPointer());
	//AppLogDebug("daum reverse result - name1 = %S", pJsonStringValueName1->GetPointer());
	//AppLogDebug("daum reverse result - name2 = %S", pJsonStringValueName2->GetPointer());
	//AppLogDebug("daum reverse result - name3 = %S", pJsonStringValueName3->GetPointer());
	//AppLogDebug("daum reverse result - nameF = %S", pJsonStringValueNameF->GetPointer());
#endif

	switch (pJson->GetType()) { 	// cleanup
	case JSON_TYPE_OBJECT:
	{
		JsonObject * pJo = static_cast<JsonObject*> (pJson);
		pJo->RemoveAll(true);
	}
		break;
	case JSON_TYPE_ARRAY:
	{
		JsonArray * pJa = static_cast<JsonArray*> (pJson);
		pJa->RemoveAll(true);
	}
		break;
	default:
		break;
	}

	delete pJson;

	return true;
}

int isArray = 0;

bool HttpDaum::parseReverseJson(IJsonValue * pValue)
{
	if (pValue == null) {
		return false;
	}

	switch (pValue->GetType()) {
	case JSON_TYPE_OBJECT:
		AppLogDebug("JSON_TYPE_OBJECT");
		{
			JsonObject* pObject = static_cast<JsonObject*>(pValue);
			IMapEnumeratorT<const String*, IJsonValue*>*  pMapEnum = pObject->GetMapEnumeratorN();
			while( pMapEnum->MoveNext() == E_SUCCESS )
			{
				const String* key = null;
				IJsonValue* value = null;
				pMapEnum->GetKey( key );

				//String *pListKey = new String(*key);
				//__pJsonKeyList->Add(*pListKey);

				isArray = 0;
				pMapEnum->GetValue( value );

				//String *pStr = null;
				//if( value->GetType() == JSON_TYPE_OBJECT )
				//{
				//	  pStr = new String("Value is an Object");
				//	__pValueList->Add(*pStr);
				//}
				//else if( value->GetType() == JSON_TYPE_ARRAY )
				//{
				//	pStr = new String("Value is an Array");
				//	__pValueList->Add(*pStr);
				//}

				parseReverseJson( value );
			}
			delete pMapEnum;
		}
		break;
    case JSON_TYPE_ARRAY:
		AppLogDebug("JSON_TYPE_ARRAY");
		{
			JsonArray* pJsonArray = static_cast<JsonArray*>(pValue);
			pJsonArray->GetCount();
			IEnumeratorT<IJsonValue*>* pEnum = pJsonArray->GetEnumeratorN();
			while( pEnum->MoveNext() == E_SUCCESS )
			{
				IJsonValue* pValue = null;
				pEnum->GetCurrent( pValue );
				if ((pValue->GetType() == JSON_TYPE_STRING) || (pValue->GetType() == JSON_TYPE_NUMBER) || (pValue->GetType() == JSON_TYPE_BOOL) || (pValue->GetType() == JSON_TYPE_NULL))
					isArray = 1;
				parseReverseJson( pValue );
			}
			delete pEnum;
		}
		break;
    case JSON_TYPE_STRING:
		AppLogDebug("JSON_TYPE_STRING");
		{
			JsonString* pVal = static_cast<JsonString*>(pValue);
			if (isArray == 0)
			{
			  //String *pStr = new String(*pVal);
			  //__pValueList->Add(*pStr);
			}
		}
		break;
    case JSON_TYPE_NUMBER:
		AppLogDebug("JSON_TYPE_NUMBER");
		{
			JsonNumber* pVal = static_cast<JsonNumber*>(pValue);
			String* pStr = new String((pVal->ToString()));
			if (isArray == 0)  {
				//__pValueList->Add(*pStr);
			}
		}
		break;
    case JSON_TYPE_BOOL:
		AppLogDebug("JSON_TYPE_BOOL");
		{
			String* pStr = null;
			JsonBool* pVal = static_cast<JsonBool*>(pValue);
			if (*pVal == true)
				pStr = new String("true");
			else
				pStr = new String("false");

			if (isArray == 0) {
				//__pValueList->Add(*pStr);
			}
		}
		break;
    case JSON_TYPE_NULL:
		AppLogDebug("JSON_TYPE_NULL");
		{
			String* pStr = null;
			pStr = new String("null");
			if (isArray == 0) {
				//__pValueList->Add(*pStr);
			}
		}
		break;
    default:
		break;

	}

	return true;
}

