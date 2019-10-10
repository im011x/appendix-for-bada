/*
 * FormAxCalc.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXCALC_H_
#define FORMAXCALC_H_

#include "AppBaseForm.h"

class FormAxCalc: public AppBaseForm {
public:
	FormAxCalc();
	virtual ~FormAxCalc();
	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXCALC_H_ */
