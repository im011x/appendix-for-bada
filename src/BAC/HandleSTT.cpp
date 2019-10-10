/*
 * HandleSTT.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "HandleSTT.h"

using namespace Osp::Base;
using namespace Osp::Uix;
using namespace Osp::Locales;

HandleSTT::HandleSTT(ISTTHandler * pHandler) : __pSttHandler(pHandler), __pStt(null)
{
}

HandleSTT::~HandleSTT()
{
	SttTerminate();
}

bool HandleSTT::SttInitialize()
{
	__pStt = new SpeechToText();

	result r = __pStt->Construct(*this);
	if (IsFailed(r)) {
		AppLogDebug("fail to construct a STT");
		return false;
	}

	r = __pStt->Initialize();
	if (IsFailed(r)) {
		AppLogDebug("fail to initialize the STT");
		return false;
	}

	AppLogDebug("success to initialize the STT");

	return true;
}

bool HandleSTT::SttTerminate()
{
	if (__pStt) {
		delete __pStt;
		__pStt = null;
	}

	return true;
}

bool HandleSTT::SttSetLocale(Osp::Locales::LanguageCode codeL, Osp::Locales::CountryCode codeC)
{
	Locale theLocale(codeL, codeC);
	result r = __pStt->SetLocale(theLocale);

	if (IsFailed(r)) {
		AppLogDebug("fail to set locale of the STT");
		return false;
	}

	return true;
}

bool HandleSTT::SttSetDefaults()
{
	String currGrammer("free_form_dictation");
	result r = __pStt->SetGrammar(currGrammer);

	if (IsFailed(r)) {
		AppLogDebug("fail to set Grammer of the STT");
		return false;
	}

	r = __pStt->SetProfanityFilterEnabled (true);
	if(IsFailed(r)) {
		AppLogDebug("fail to SetProfanityFilterEnabled of the STT");
		return false;
	}

	r =	__pStt->SetPunctuationOverrideEnabled (true);
	if(IsFailed(r)) {
		AppLogDebug("fail to SetPunctuationOverrideEnabled of the STT");
		return false;
	}

	r = __pStt->SetSilenceDetectionEnabled (true);
	if(IsFailed(r)) {
		AppLogDebug("fail to SetSilenceDetectionEnabled of the STT");
		return false;
	}

	return true;
}

bool HandleSTT::SttStart()
{
	AppLogDebug("SttStart() !!!");

	result r = __pStt->Start();

	if(IsFailed(r)) {
		AppLogDebug("fail to Start of the STT");
		return false;
	}

	return true;
}

bool HandleSTT::SttStop()
{
	if (__pStt) {
		return (!IsFailed(__pStt->Stop()));
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void HandleSTT::OnSpeechToTextInitialized(void)
{
	// force
	if (SttSetLocale(LANGUAGE_KOR, COUNTRY_KR) == false) {
		return ;
	}

	if (SttSetDefaults() == false) {
		return ;
	}

	if (__pSttHandler) {
		__pSttHandler->OnSttInitialized();
	}

	SttStart();
}

void HandleSTT::OnSpeechToTextErrorOccurred(Osp::Uix::SpeechToTextError error)
{
	const String msg_errors[] = {
			"STT_ERROR_INITIALIZATION_FAILED",
			"STT_ERROR_START_FAILED",
			"STT_ERROR_STOP_FAILED",
			"STT_ERROR_RECORDING_FAILED",
			"STT_ERROR_NETWORK_FAILED",
			"STT_ERROR_CANCELED",
			"STT_ERROR_RECOGNITION_FAILED",
			"STT_ERROR_TOO_SOON",
			"STT_ERROR_TOO_SHORT",
			"STT_ERROR_TOO_LONG",
			"STT_ERROR_TOO_QUIET",
			"STT_ERROR_TOO_LOUD",
			"STT_ERROR_TOO_FAST"
		};
	AppLogDebug("error OnSpeechToTextErrorOccurred - %d (%S)", error, msg_errors[error].GetPointer());

	if (__pSttHandler) {
		__pSttHandler->OnSttError(error, msg_errors[error]);
	}
}

void HandleSTT::OnSpeechToTextCompleted(Osp::Base::String& string, Osp::Uix::SpeechToTextWarning warning)
{
	const String msg_warns[] = {
			"STT_WARNING_NONE",
			"STT_WARNING_TOO_SOON",
			"STT_WARNING_TOO_SHORT",
			"STT_WARNING_TOO_LONG",
			"STT_WARNING_TOO_QUIET",
			"STT_WARNING_TOO_LOUD",
			"STT_WARNING_TOO_FAST"
	};
	AppLogDebug("completed OnSpeechToTextCompleted - %d (%S) - [%S]", warning, msg_warns[warning].GetPointer(), string.GetPointer());

#ifdef _DEBUG_DONE
	String srecognized = string;
	if (srecognized.GetLength() > 0) {
		ByteBuffer* pNameBuffer = Osp::Base::Utility::StringUtil::StringToUtf8N(srecognized);
		if (pNameBuffer) {
			AppLogDebug("   recognized string = [%s]", pNameBuffer->GetPointer());
			delete pNameBuffer;
		}
	}
#endif

	if (__pSttHandler) {
		if (string.CompareTo("_LOC") == 0) {  // 실패
			__pSttHandler->OnSttError(STT_ERROR_RECOGNITION_FAILED, "음성인식 실패! 재시도 바랍니다.");
		}
		else {  // 정상
			__pSttHandler->OnSttCompleted(string, warning);
		}
	}
}

void HandleSTT::OnSpeechToTextRecordingStatusChanged(Osp::Uix::SpeechToTextRecordingStatus status)
{
	const String msg_status[] = {
			"STT_STATUS_RECORDING_STARTED",
			"STT_STATUS_RECORDING_STOPPED",
			"STT_STATUS_RECORDING_COMPLETED"
	};
	AppLogDebug("changed OnSpeechToTextRecordingStatusChanged - %d (%S)", status, msg_status[status].GetPointer());

	if (__pSttHandler) {
		__pSttHandler->OnSttStatusChanged(status);
	}
}


