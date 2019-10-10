/*
 * HandleHttp.cpp
 *
 *  Created on: 2012. 1. 18.
 *      Author: SONG
 */

#include <FText.h>
#include "HandleHttp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

HandleHttp::HandleHttp() : __pSession(null), __pTransaction(null), __pHttpResponder(null)
{
}

HandleHttp::~HandleHttp()
{
	Cancel();
}

void HandleHttp::Cancel()
{
	if (__pSession)	{
		__pSession->CloseAllTransactions();
		delete __pSession;
		__pSession = null;
	}

/*
	if (__pTransaction) {
		delete __pTransaction;
		__pTransaction = null;
	}
*/
}

void HandleHttp::dumpHeader(HttpHeader * pH)
{
	if (pH == null)
		return;

	Osp::Base::Collection::IList* il = pH->GetFieldNamesN();

	if (il != null) {
		int many = il->GetCount();
		for (int i=0; i<many; i++) {
			String * pKey = (String*) il->GetAt(i);
			AppLogDebug("Header key name = [%S]", pKey->GetPointer());
		}
	}

	AppLogDebug("==================");
}

bool HandleHttp::GetHttp(const Osp::Base::String& uri, const Osp::Base::String & host)
{
	result r = E_SUCCESS;
	//HttpTransaction* pTransaction = null;
	HttpRequest* pRequest = null;

	// 일단 이전의 요청을 취소하고 해당 세션을 삭제한다.
	Cancel();

	AppLogDebug("REQUEST URL = [%S]", uri.GetPointer());

	//
	__pSession = new HttpSession();
	r = __pSession->Construct(NET_HTTP_SESSION_MODE_NORMAL, null, host, null);
	if (IsFailed(r))
		return false;

	__pTransaction = __pSession->OpenTransactionN();
	if (null == __pTransaction)	{
		r = GetLastResult();
		return false;
	}

	r = __pTransaction->AddHttpTransactionListener(*this);
	if (IsFailed(r))
		return false;

	pRequest = const_cast<HttpRequest*>(__pTransaction->GetRequest());
	if(pRequest == null)
	{
		r = GetLastResult();
		return false;
	}

	//HttpHeader * pHeader = pRequest->GetHeader();
	//if (pHeader != null) {
	//	dumpHeader(pHeader);
	//	//pHeader->AddField("Accept-Language", "ko-KR");
	//	pHeader->AddField("Accept", "text/html,*/*");
	//	pHeader->AddField("User-Agent", "Mozilla/5.0");
	//	pHeader->AddField("Accept-Language", "ko-KR");
	//	dumpHeader(pHeader);
	//}

	__requestedUrl = uri; //

	r = pRequest->SetUri(uri);
	if(IsFailed(r))
		return false;

	r = pRequest->SetMethod(NET_HTTP_METHOD_GET);
	if(IsFailed(r))
		return false;

	r = __pTransaction->Submit();
	if(IsFailed(r))
		return false;

	//AppLogDebug("@@@ invoked transaction = 0x%x", pTransaction);

	return true;
}

///////////////////////////////////////////////////////////
// IHttpTransactionEventListener
///////////////////////////////////////////////////////////

void HandleHttp::OnTransactionReadyToRead(HttpSession& httpSession, HttpTransaction& httpTransaction, int availableBodyLen)
{
	AppLog("####### OnTransactionReadyToRead! #######");
	//AppLogDebug("@@@ callback transaction = 0x%x", &httpTransaction);

	/*
	HttpResponse* pHttpResponse = httpTransaction.GetResponse();

	if (pHttpResponse->GetStatusCode() != NET_HTTP_STATUS_OK)	{
		String sAlert; // = "서버의 기본 설정 정보를 가져오지 못하였습니다.";

		sAlert.Format(512, L"%S - 오류코드 = %d\n\n잠시 후 다시 실행하여 사용바랍니다.",
				pHttpResponse->GetStatusText().GetPointer(), pHttpResponse->GetStatusCode());
		if (__pHttpResponder) {
			__pHttpResponder->OnBadResponse(this, sAlert);  // AppUtil::UtilMessageBox();
		}

		return;
	}

	//HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
	//String* tempHeaderString = pHttpHeader->GetRawHeaderN();
	ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

	if (__pHttpResponder) {
		AppLogDebug("HTTP RECV Data = %s", pBuffer->GetPointer());
		__pHttpResponder->OnGoodResponse(this, pBuffer);
	}
	//else {
	//}

	delete pBuffer;
	 */

}

void HandleHttp::OnTransactionAborted(HttpSession& httpSession, HttpTransaction& httpTransaction, result r)
{
	AppLog("####### OnTransactionAborted! (%s)#######", GetErrorMessage(r));

	String sAlert = GetErrorMessage(r);
	sAlert.Insert("네트워크 에러입니다 - 에러내용은 \n", 0);

	Cancel();

	if (__pHttpResponder) {
		__pHttpResponder->OnBadResponse(this, sAlert);
	}

}

void HandleHttp::OnTransactionReadyToWrite(HttpSession& httpSession, HttpTransaction& httpTransaction, int recommendedChunkSize)
{
	AppLog("####### OnTransactionReadyToWrite! #######");
}

void HandleHttp::OnTransactionHeaderCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction, int headerLen, bool rs)
{
	AppLog("####### OnTransactionHeaderCompleted! #######");
}

void HandleHttp::OnTransactionCompleted(HttpSession& httpSession, HttpTransaction& httpTransaction)
{
	HttpResponse* pHttpResponse = httpTransaction.GetResponse();
	AppLog("####### OnTransactionCompleted! #######");

	if (pHttpResponse == null) {
		AppLogDebug("no response instance!!!");
		return;
	}

	if (pHttpResponse->GetStatusCode() != NET_HTTP_STATUS_OK)	{
		String sAlert = pHttpResponse->GetStatusText();

		String errCode;
		errCode.Format(128, L"\n(오류코드 = %d)", pHttpResponse->GetStatusCode());
		sAlert.Append(errCode);

		AppLogDebug("error !!! - %S", sAlert.GetPointer());

		if (__pHttpResponder) {
			__pHttpResponder->OnBadResponse(this, sAlert);
		}

		Cancel();
		return;
	}

	//HttpHeader* pHttpHeader = pHttpResponse->GetHeader();
	//String* tempHeaderString = pHttpHeader->GetRawHeaderN();

	ByteBuffer* pBuffer = pHttpResponse->ReadBodyN();

	if (pBuffer == null) {
		AppLogDebug("####### HTTP RECV buffer is NULL #######");
		return;
	}

	if (__pHttpResponder) {
		//AppLogDebug("HTTP RECV Data = %s", pBuffer->GetPointer()); // 이 자체에서 문제 발생한다... 너무 길면 ...
		__pHttpResponder->OnGoodResponse(this, pBuffer);
	}

	delete pBuffer;
	//delete &httpTransaction; // 확인해 볼것!

	Cancel();
}

void HandleHttp::OnTransactionCertVerificationRequiredN(HttpSession& httpSession, HttpTransaction& httpTransaction, Osp::Base::String* pCert)
{
	AppLog("####### OnTransactionCertVerificationRequiredN! #######");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

