/*
 * FormAxBusStopZip2Near.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPZIP2NEAR_H_
#define FORMAXBUSSTOPZIP2NEAR_H_

#include "FormAxBus.h"

class FormAxBusStopZip2Near: public FormAxBus {
public:
	FormAxBusStopZip2Near();
	virtual ~FormAxBusStopZip2Near();

	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	//virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXBUSSTOPZIP2NEAR_H_ */
