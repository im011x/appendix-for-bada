/*
 * FormAxBusStopMap.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPMAP_H_
#define FORMAXBUSSTOPMAP_H_

#include "FormAxBus.h"

class FormAxBusStopMap: public FormAxBus {
public:
	FormAxBusStopMap();
	virtual ~FormAxBusStopMap();

	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	//virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXBUSSTOPMAP_H_ */
