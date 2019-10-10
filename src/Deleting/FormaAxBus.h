/*
 * FormaAxBus.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAAXBUS_H_
#define FORMAAXBUS_H_

#include "AppBaseForm.h"
#include "HttpBusSeoul.h"

class FormaAxBus: public AppBaseForm {
public:
	FormaAxBus();
	virtual ~FormaAxBus();
	virtual bool AtInitialize();

// Implementation
protected:
	HttpBusSeoul 		* 		__httpSeoulBus;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAAXBUS_H_ */
