/*
 * BCanAccssibility.h
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#ifndef BCANACCSSIBILITY_H_
#define BCANACCSSIBILITY_H_

#include "HandleTTS.h"
#include "HandleSTT.h"
#include "UtilPopupProgress.h"
#include <FSystem.h>

class BCanAccssibility
	: public ITTSHandler
	, public ISTTHandler
	, public IPopupProgressUserAction
{
public:
	BCanAccssibility();
	virtual ~BCanAccssibility();

private:
	const static 	int 		ID_STOP_STT = 101;

	HandleTTS 		* 			__pTTS;
	HandleSTT 		* 			__pSTT;
	Osp::System::Vibrator * 	__pVibrator;

protected:
	void 	TTSClear();
	void 	STTClear();
	void	VibratorClear();

public:
	void 	TTS(const String & say, bool bVibrate = true);
	void 	STT();
	void 	Vibrate(int milisec = 500, int level = 100);

	// ITTSHandler
	virtual void 	OnTtsError(TextToSpeechError errorCode);
	virtual void 	OnTtsSynthesize();
	virtual void 	OnTtsPlay();
	virtual void 	OnTtsCompleted();
	virtual void 	OnTtsIdle();

	// ISTTHandler
	virtual void OnSttInitialized();
	virtual void OnSttError(SpeechToTextError error, const String & message);
	virtual void OnSttCompleted(String& string, SpeechToTextWarning warning);
	virtual void OnSttStatusChanged(SpeechToTextRecordingStatus status);

	//
	virtual void	OnPopupProgressUserAction(int actionId);

};

#endif /* BCANACCSSIBILITY_H_ */
