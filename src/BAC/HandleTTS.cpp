/*
 * HandleTTS.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "HandleTTS.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Locales;
using namespace Osp::Uix;

HandleTTS::HandleTTS(ITTSHandler * pHandler) : __pTtsHandler(pHandler), __pTts(null)
{
}

HandleTTS::~HandleTTS()
{
	TtsTerminate();
}

bool HandleTTS::TtsInitialize()
{
	__pTts = new TextToSpeech();

	result r = __pTts->Construct(*this);
	if (IsFailed(r)) {
		AppLogDebug("fail to construct a TTS");
		return false;
	}

	r = __pTts->Initialize();
	if (IsFailed(r)) {
		AppLogDebug("fail to initialize the TTS");
		return false;
	}

	return true;
}

bool HandleTTS::TtsInitialize(const String &words)
{
	if (TtsInitialize()) {
		TtsSetSentence(words);
		return true;
	}

	return false;
}

bool HandleTTS::TtsTerminate()
{
	if (__pTts) {
		delete __pTts;
		__pTts = null;
	}

	return true;
}

void HandleTTS::TtsSetSentence(const String &words)
{
	__sentence = words;
}

bool HandleTTS::TtsSetLocale(LanguageCode codeL, CountryCode codeC)
{
	Locale theLocale(codeL, codeC);
	result r = __pTts->SetLocale(theLocale);

	if (IsFailed(r)) {
		AppLogDebug("fail to set locale of the TTS");
		return false;
	}

	return true;
}

bool HandleTTS::TtsStart()
{
	if (__pTts) {
		__pTts->Speak(__sentence);
	}

	return true;
}

bool HandleTTS::TtsStop()
{
	if (__pTts) {
		__pTts->Stop();
	}

	return true;
}

bool HandleTTS::TtsResume()
{
	if (__pTts) {
		__pTts->Resume();
	}

	return true;
}

bool HandleTTS::TtsPause()
{
	if (__pTts) {
		__pTts->Pause();
	}

	return true;
}

void HandleTTS::showError(TextToSpeechError error)
{
	const String errors[] = {
		"TTS_ERROR_INITIALIZATION_FAILED",
		"TTS_ERROR_START_FAILED",
		"TTS_ERROR_STOP_FAILED",
		"TTS_ERROR_PLAYING_FAILED",
		"TTS_ERROR_NETWORK_FAILED",
		"TTS_ERROR_PAUSE_FAILED",
		"TTS_ERROR_RESUME_FAILED" };

	AppLogDebug("error OnTextToSpeechErrorOccurred - %d (%S)", error, errors[error].GetPointer());
}

void HandleTTS::showStatus(TextToSpeechStatus status)
{
	const String statuses[] = {
		"TTS_STATUS_IDLE",
		"TTS_STATUS_INITIALIZED",
		"TTS_STATUS_SYNTHESIZING",
		"TTS_STATUS_PLAYING",
		"TTS_STATUS_COMPLETED",
		"TTS_STATUS_PAUSED" };

	AppLogDebug("status OnTextToSpeechStatusChanged - %d (%S)", status, statuses[status].GetPointer());
}

void HandleTTS::showLocales()
{
	IList * pLocales = __pTts->GetSupportedLocalesN();

	for (int i=0; i<pLocales->GetCount(); i++) {
		Locale * plocale = static_cast<Locale*> (pLocales->GetAt(i));

		AppLogDebug("supporting locale of TTS [%d] = %d,%d (%S,%S)",
				i, plocale->GetLanguageCode(), plocale->GetCountryCode(), plocale->GetLanguageCodeString().GetPointer(), plocale->GetCountryCodeString().GetPointer());
	}

	delete pLocales;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void HandleTTS::OnTextToSpeechErrorOccurred (TextToSpeechError error)
{
	showError(error);

	if (__pTtsHandler) {
		__pTtsHandler->OnTtsError(error);
	}
}

void HandleTTS::OnTextToSpeechStatusChanged (TextToSpeechStatus status)
{
	showStatus(status);

	switch (status) {
	case TTS_STATUS_INITIALIZED:
		// force the locale setting
		TtsSetLocale(LANGUAGE_KOR, COUNTRY_KR);
		TtsStart();
		break;
	case TTS_STATUS_SYNTHESIZING:
		if (__pTtsHandler) {
			__pTtsHandler->OnTtsSynthesize();
		}
		break;
	case TTS_STATUS_PLAYING:
		if (__pTtsHandler) {
			__pTtsHandler->OnTtsPlay();
		}
		break;
	case TTS_STATUS_COMPLETED:
		if (__pTtsHandler) {
			__pTtsHandler->OnTtsCompleted();
		}
		break;
	case TTS_STATUS_PAUSED:
		break;
	case TTS_STATUS_IDLE:
		if (__pTtsHandler) {
			__pTtsHandler->OnTtsIdle();
		}
		break;
	default:
		break;
	}
}

