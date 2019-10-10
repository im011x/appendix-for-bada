/*
 * HandleSTT.h
 *
 *  Created on: 2012. 8. 8.
 *      Author: benny
 */

#ifndef HANDLESTT_H_
#define HANDLESTT_H_

#include <FBase.h>
#include <FUixSpeechToText.h>

using namespace Osp::Uix;
using namespace Osp::Base;

class ISTTHandler
{
public:
	ISTTHandler() {};
	virtual ~ISTTHandler() {};

public:
	virtual void OnSttInitialized() = 0;
	virtual void OnSttError(SpeechToTextError error, const String & message) = 0;
	virtual void OnSttCompleted(String& string, SpeechToTextWarning warning) = 0;
	virtual void OnSttStatusChanged(SpeechToTextRecordingStatus status) = 0;
};


class HandleSTT
	: public Osp::Uix::ISpeechToTextEventListener
{
public:
	HandleSTT(ISTTHandler * pHandler);
	virtual ~HandleSTT();

private:
	ISTTHandler 				* 		__pSttHandler;
	Osp::Uix::SpeechToText* 			__pStt;

protected:
	//void 		showError();
	bool 		SttSetLocale(Osp::Locales::LanguageCode codeL, Osp::Locales::CountryCode codeC);
	bool 		SttSetDefaults();

public:
	bool 		SttInitialize();
	bool 		SttTerminate();

	bool 		SttStart();
	bool 		SttStop();

public:
	// ISpeechToTextEventListener
	virtual void 	OnSpeechToTextInitialized ( void );
	virtual void 	OnSpeechToTextErrorOccurred(Osp::Uix::SpeechToTextError error);
	virtual void 	OnSpeechToTextCompleted(Osp::Base::String& string, Osp::Uix::SpeechToTextWarning warning);
	virtual void 	OnSpeechToTextRecordingStatusChanged(Osp::Uix::SpeechToTextRecordingStatus status);

};

#endif /* HANDLESTT_H_ */
