/*
 * FormAxBusStopAddr2Near.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPADDR2NEAR_H_
#define FORMAXBUSSTOPADDR2NEAR_H_

#include "FormAxBus.h"

class FormAxBusStopAddr2Near: public FormAxBus {
public:
	FormAxBusStopAddr2Near();
	virtual ~FormAxBusStopAddr2Near();

	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	//virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXBUSSTOPADDR2NEAR_H_ */
