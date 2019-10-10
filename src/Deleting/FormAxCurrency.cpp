/*
 * FormAxCurrency.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxCurrency.h"
#include "UtilCode.h"

using namespace Osp::Net::Http;
using namespace Osp::Base::Utility;
using namespace Osp::Xml;

FormAxCurrency::FormAxCurrency() : __pSession(null)
{
}

FormAxCurrency::~FormAxCurrency()
{
}

bool FormAxCurrency::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("환율");

	return true;
}

result FormAxCurrency::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();



	invokeQuery();

	return r;
}

result FormAxCurrency::invokeQuery()
{
	result r = E_SUCCESS;
	HttpTransaction* pTransaction = null;
	HttpRequest* pRequest = null;

	String hostAddr("http://info.finance.naver.com");
	HttpHeader * pHeader = null;

	// https://raw.github.com/currencybot/open-exchange-rates/master/latest.json
	// http://themoneyconverter.com/rss-feed/KRW/rss.xml
	// "http://www.naver.com/include/timesquare/widget/exchange.xml"
	// http://info.finance.naver.com/marketindex/exchangeMain.nhn

	String theUrl("http://info.finance.naver.com/marketindex/exchangeList.nhn");
	AppLogDebug("REQUEST = [%S]", theUrl.GetPointer());

	//
	if(__pSession == null)
	{
		__pSession = new HttpSession();
		if(__pSession == null)
			goto CATCH;

		r = __pSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, hostAddr, null);
		if (IsFailed(r))
			goto CATCH;
	}

	pTransaction = __pSession->OpenTransactionN();
	if (null == pTransaction)
	{
		r = GetLastResult();
		goto CATCH;
	}

	r = pTransaction->AddHttpTransactionListener(*this);
	if (IsFailed(r))
		goto CATCH;

	pRequest = const_cast<HttpRequest*>(pTransaction->GetRequest());
	if(pRequest == null)
	{
		r = GetLastResult();
		goto CATCH;
	}

	/////////////////////////////////////////////////
/*
	pHeader = pRequest->GetHeader();
	if (pHeader != null) {
		pHeader->AddField("Referer", "http://info.finance.naver.com");
	}
*/
	/////////////////////////////////////////////////

	//r = pRequest->SetUri(L"www.bada.com");
	r = pRequest->SetUri(theUrl);
	if(IsFailed(r))
		goto CATCH;

	r = pRequest->SetMethod(NET_HTTP_METHOD_GET);
	if(IsFailed(r))
		goto CATCH;

	r = pTransaction->Submit();
	if(IsFailed(r))
		goto CATCH;

	return r;

CATCH:
	if(pTransaction != null)
	{
		delete pTransaction;
		pTransaction = null;
	}
	AppLog("failed. (%s)", GetErrorMessage(r));

	return r;
}

result FormAxCurrency::OnTerminating(void)
{
	if(__pSession != null)
	{
		delete __pSession;
		__pSession = null;
	}

	return BacBaseForm::OnTerminating();
}

void FormAxCurrency::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}

///////////////////////////////////////////////////////////
// IHttpTransactionEventListener
///////////////////////////////////////////////////////////

void FormAxCurrency::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("####### OnTransactionReadyToRead! #######");

	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	if(pHttpResponse == null)
		return;

	/*
	if (pHttpResponse->GetHttpStatusCode() == HTTP_STATUS_OK)
	{
		HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
		if(pHttpHeader != null)
		{
			String* tempHeaderString = pHttpHeader->GetRawHeaderN();

			ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

			String text(L"Read Body Length: ");
			text.Append(availableBodyLen);

			__pEditArea->SetText(text);
			__pFrame->Draw();
			__pFrame->Show();

			delete tempHeaderString;
			delete pBuffer;
		}
	}
	 */

	if (pHttpResponse->GetHttpStatusCode() != HTTP_STATUS_OK) {
		AppLogDebug("BAD HTTP Request!");
		return;
	}

	HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
	String* tempHeaderString = pHttpHeader->GetRawHeaderN();
	AppLogDebug("HTTP Response Header = [%S]", tempHeaderString->GetPointer());
	delete tempHeaderString;

	ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();
	//AppLogDebug("HTTP Response raw = (ksc5601) (%d)/[%s]", pBuffer->GetLimit(), pBuffer->GetPointer());

	//parseHtml(pBuffer);

	ByteBuffer * pBufferUtf8 = UtilCode::Euckr_to_Utf8N(pBuffer);
	parseHtml(pBufferUtf8, "utf-8");
	delete pBufferUtf8;

	/*
	String * pString = UtilCode::Euckr_to_UnicodeN(pBuffer);
	//AppLogDebug("HTTP Response raw (unicode) = (%d)/[%S]", pString->GetLength(), pString->GetPointer());
	delete pString;
	 */

	delete pBuffer;
}

void FormAxCurrency::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("####### OnTransactionAborted! (%s)#######", GetErrorMessage(r));

	delete &httpTransaction;
}

void FormAxCurrency::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("####### OnTransactionReadyToWrite! #######");
}

void FormAxCurrency::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool rs)
{
	AppLog("####### OnTransactionHeaderCompleted! #######");
}



void FormAxCurrency::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	AppLog("####### OnTransactionCompleted! #######");

	delete &httpTransaction;
}

void FormAxCurrency::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert)
{
	AppLog("####### OnTransactionCertVerificationRequiredN! #######");
}

void FormAxCurrency::parseHtml(ByteBuffer* pBuffer, const char * pCodingScheme)
{
	htmlDocPtr pDoc = NULL;
	htmlNodePtr pRoot = NULL;


	//pDoc = htmlParseDoc((xmlChar*) pBuffer->GetPointer(), (const char *)"KSC5601");
	//pDoc = htmlParseDoc((xmlChar*) pBuffer->GetPointer(), (const char *)"UTF-8");
	pDoc = htmlParseDoc((xmlChar*) pBuffer->GetPointer(), pCodingScheme);
	if (!pDoc) {
		AppLogDebug("");
		return;
	}

	pRoot = xmlDocGetRootElement(pDoc);
	if (!pRoot) {
		AppLogDebug("");
		return;
	}


	extractHyperLinks(pRoot, 0);

	AppLogDebug("### OK !!!");
}

void FormAxCurrency::extractHyperLinks(Osp::Xml::htmlNodePtr pRoot, int level)
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

			if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"a") == 0) {
				AppLogDebug("#### 화폐 ### []");
			}
			if(xmlStrcasecmp(pCurrentElement->parent->name, (const xmlChar*)"td") == 0) {
				AppLogDebug("#### 환율 ### []");
			}

			// 일단 바꾸자!
			Osp::Base::Utility::StringUtil::Utf8ToString ((char*) pCurrentElement->content, nameString);
			nameString.Trim();

			if (nameString.GetLength() > 0) {
				ByteBuffer* pNameBuffer = Osp::Base::Utility::StringUtil::StringToUtf8N(nameString);
				if (pNameBuffer) {
					AppLogDebug("   [%d] text of [%s] string value = [%s]", level, pCurrentElement->parent->name, pNameBuffer->GetPointer());
					delete pNameBuffer;
				}
			}

			//AppLogDebug("   [%d] text of [%s] = [%s]", level, pCurrentElement->parent->name, pCurrentElement->content);
			//AppLogDebug("   [%d] text of [%s] = (%S)[%s]", level, pCurrentElement->parent->name, nameString.GetPointer(), pCurrentElement->content);
			//AppLogDebug("   [%d] text of [%s] = [%s]", level, pCurrentElement->parent->name, pCurrentElement->parent->content);

		}


	}
}
