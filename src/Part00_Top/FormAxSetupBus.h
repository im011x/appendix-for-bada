/*
 * FormAxSetupBus.h
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#ifndef FORMAXSETUPBUS_H_
#define FORMAXSETUPBUS_H_

#include "FormAxSetup.h"
#include "UtilRegistry.h"

class FormAxSetupBus
	: public FormAxSetup
	, public UtilRegistry
	, public Osp::Ui::IAdjustmentEventListener
	, public Osp::Ui::Controls::ISliderEventListener
{
public:
	FormAxSetupBus();
	virtual ~FormAxSetupBus();
	virtual bool AtInitialize();

protected:
	static 	const 	int 			ID_TAP_MYBUS_GOTO_NONE = 201;
	static 	const 	int 			ID_TAP_MYBUS_GOTO_STOP = 202;
	static 	const 	int 			ID_TAP_MYBUS_GOTO_LINE = 203;
	static 	const 	int 			ID_TAP_STOPLINE_GOTO_NONE = 211;
	static 	const 	int 			ID_TAP_STOPLINE_GOTO_LINE = 212;
	static 	const 	int 			ID_TAP_STOPLINE_GOTO_STOP = 213;

	Slider 		* 			__pSliderScreen;
	Slider 		* 			__pSliderServer;
	MyLabel		* 			__pLabelTimeScreen;
	MyLabel		* 			__pLabelTimeServer;

	MyButton 	* 			__pOptionsMybus[3];
	MyButton 	* 			__pOptionsStoplines[3];

	int 					__nTapMybusGoto;
	int 					__nTapStoplineGoto;

protected:
	void 		init();
	void 		save();
	void 		update();

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IAdjustmentEventListener
	virtual void OnAdjustmentValueChanged(const Osp::Ui::Control& source, int adjustment);
	// ISliderEventListener
	virtual void OnSliderBarMoved(Osp::Ui::Controls::Slider& source, int value);
};

#endif /* FORMAXSETUPBUS_H_ */
