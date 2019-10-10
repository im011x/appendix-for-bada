/*
 * FormAxBusAboutStop.h
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOP_H_
#define FORMAXBUSSTOP_H_

#include "FormAxBus.h"

class FormAxBusAboutStopOLD: public FormAxBus
{
public:
	FormAxBusAboutStopOLD();
	virtual ~FormAxBusAboutStopOLD();

	virtual bool AtInitialize();

// Implementation
protected:
	const 	static 	int 		ID_STOP_NEAR = 101;
	const 	static 	int 		ID_STOP_NAME = 102;
	const 	static 	int 		ID_STOP_CODE = 103;
	const 	static 	int 		ID_STOP_MAP  = 104;
	const 	static 	int 		ID_STOP_DONG = 105;
	const 	static 	int 		ID_STOP_ZIP  = 106;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXBUSSTOP_H_ */
