/*
 * FormAxGPS.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXGPS_H_
#define FORMAXGPS_H_

#include "AppBaseForm.h"

class FormAxGPS: public AppBaseForm {
public:
	FormAxGPS();
	virtual ~FormAxGPS();
	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXGPS_H_ */
