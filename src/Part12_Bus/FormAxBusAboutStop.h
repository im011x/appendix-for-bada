/*
 * FormAxBusAboutStop.h
 *
 *  Created on: 2012. 7. 27.
 *      Author: benny
 */

#ifndef FORMAXBUSABOUTSTOP_H_
#define FORMAXBUSABOUTSTOP_H_

#include "FormAxBus.h"

class FormAxBusAboutStop
	: public FormAxBus
{
public:
	FormAxBusAboutStop();
	virtual ~FormAxBusAboutStop();
	virtual bool AtInitialize();
	virtual bool AtWakeup();

// Implementation
protected:
	const 	static 	int 		ID_SEARCH_CLEAR = 110;
	const 	static 	int 		ID_SEARCH_CANCEL = 111;
	const 	static 	int 		ID_SEARCH_DO = 112;
	const 	static 	int 		ID_SEARCH_DO2 = 113;

	const 	static 	int 		ID_MESSAGEBOX_CONFIRM = 201;

	const 	static 	int 		ID_GOTO_SEARCH_ADDRESS = 301;
	const 	static 	int 		ID_GOTO_SEARCH_NEARSTOP = 302;
	const 	static 	int 		ID_SEARCH_BY_VOICE = 303;

	const 	static 	int 		ID_SEARCH_OPTION_1 = 501;
	const 	static 	int 		ID_SEARCH_OPTION_2 = 502;

	EditField 	* 						__pSearchEdit;
	MyButton 	* 						__pOptionsBtn[2];

private:
	int 					__searchOption;

protected:
	void 			call_search_form();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// ISTTHandler 가운데 중요한 것 하나만 !
	virtual void OnSttCompleted(String& string, SpeechToTextWarning warning);
	virtual void OnSttError(SpeechToTextError error, const String & message);
};

#endif /* FORMAXBUSABOUTSTOP_H_ */
