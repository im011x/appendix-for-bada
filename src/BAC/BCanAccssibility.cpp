/*
 * BCanAccssibility.cpp
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#include "BCanAccssibility.h"
using namespace Osp::System;


BCanAccssibility::BCanAccssibility() : __pTTS(null), __pSTT(null), __pVibrator(null)
{
}

BCanAccssibility::~BCanAccssibility()
{
	TTSClear();
	STTClear();
	VibratorClear();
}

void BCanAccssibility::TTSClear()
{
	if (__pTTS) {
		delete __pTTS;
		__pTTS = null;
	}
}

void BCanAccssibility::STTClear()
{
	if (__pSTT) {
		delete __pSTT;
		__pSTT = null;
	}
}

void BCanAccssibility::VibratorClear()
{
	if (__pVibrator) {
		delete __pVibrator;
		__pVibrator = null;
	}
}

void BCanAccssibility::TTS(const String & say, bool bVibrate)
{
	if (bVibrate) {
		Vibrate();
	}

	TTSClear();

	__pTTS = new HandleTTS(this);
	__pTTS->TtsInitialize();
	__pTTS->TtsSetSentence(say);
}

void BCanAccssibility::STT()
{
	STTClear();

	__pSTT = new HandleSTT(this);
	__pSTT->SttInitialize();

	// show
	PopupProgressHelper::Show("음성인식 모듈을 초기화 하고 있습니다...", this, ID_STOP_STT, "음성 처리", false);
}

void BCanAccssibility::Vibrate(int milisec, int level)
{
	if (__pVibrator == null) {
		__pVibrator = new Osp::System::Vibrator();
		__pVibrator->Construct();
	}

	__pVibrator->Start(milisec, level);

	VibratorClear();

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void BCanAccssibility::OnPopupProgressUserAction(int actionId)
{
	AppLogDebug("");

	switch (actionId) {
	case ID_STOP_STT:
		AppLogDebug("stopping STT");
		if (__pSTT->SttStop() == false) {
			PopupProgressHelper::Hide();
		}
		break;
	}
}


void BCanAccssibility::OnTtsError(TextToSpeechError errorCode)
{
	AppLogDebug("");
}

void BCanAccssibility::OnTtsSynthesize()
{
	AppLogDebug("");
}

void BCanAccssibility::OnTtsPlay()
{
	AppLogDebug("");
}

void BCanAccssibility::OnTtsCompleted()
{
	AppLogDebug("");
}

void BCanAccssibility::OnTtsIdle()
{
	AppLogDebug("");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void BCanAccssibility::OnSttInitialized()
{
	AppLogDebug("");
}

void BCanAccssibility::OnSttError(SpeechToTextError error, const String & message)
{
	AppLogDebug("");
	PopupProgressHelper::Show("음성인식 오류입니다. \n " + message);
}

void BCanAccssibility::OnSttCompleted(String& recognized, SpeechToTextWarning warning)
{
	AppLogDebug("음성인식 결과는 ... [%S]", recognized.GetPointer());
	PopupProgressHelper::Hide();
}

void BCanAccssibility::OnSttStatusChanged(SpeechToTextRecordingStatus status)
{
	AppLogDebug("");

	String showMessage;
	switch (status) {
	case STT_STATUS_RECORDING_STARTED:
		showMessage = "음성 입력 대기중 입니다.";
		break;
	case STT_STATUS_RECORDING_STOPPED:
		showMessage = "음성인식 처리중 입니다.";
		break;
	case STT_STATUS_RECORDING_COMPLETED:
		showMessage = "음성인식을 완료합니다.";
		break;
	default:
		showMessage = "음성인식 상태 미지정 ...";
		break;
	}

	PopupProgressHelper::Show(showMessage);
}

