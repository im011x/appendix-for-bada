/*
 * HandleTTS.h
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#ifndef HANDLETTS_H_
#define HANDLETTS_H_

#include <FBase.h>
#include <FUixTextToSpeech.h>

using namespace Osp::Uix;
using namespace Osp::Base;

class ITTSHandler
{
public:
	ITTSHandler() {};
	virtual ~ITTSHandler() {};

public:
	virtual void 	OnTtsError(TextToSpeechError errorCode) = 0;
	virtual void 	OnTtsSynthesize() = 0;
	virtual void 	OnTtsPlay() = 0;
	virtual void 	OnTtsCompleted() = 0;
	virtual void 	OnTtsIdle() = 0;

};

class HandleTTS
	: public Osp::Uix::ITextToSpeechEventListener
{
public:
	HandleTTS(ITTSHandler * pHandler);
	virtual ~HandleTTS();

// Implementation
private:
	ITTSHandler 				* 		__pTtsHandler;
	Osp::Uix::TextToSpeech		* 		__pTts;
	String 								__sentence;

public:
	virtual void OnTextToSpeechErrorOccurred (TextToSpeechError error);
	virtual void OnTextToSpeechStatusChanged (TextToSpeechStatus status);

protected:
	void 		showError(TextToSpeechError error);
	void 		showStatus(TextToSpeechStatus status);
	void 		showLocales();

public:
	bool 		TtsInitialize();
	bool 		TtsInitialize(const String &words);
	bool 		TtsTerminate();

	void 		TtsSetSentence(const String &words);
	bool 		TtsSetLocale(Osp::Locales::LanguageCode codeL, Osp::Locales::CountryCode codeC);

	bool 		TtsStart();
	bool 		TtsStop();
	bool 		TtsResume();
	bool 		TtsPause();

};

#endif /* HANDLETTS_H_ */
