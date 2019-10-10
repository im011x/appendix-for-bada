/*
 * FormAxMeasures.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXMEASURES_H_
#define FORMAXMEASURES_H_

#include "AppBaseForm.h"

class FormAxMeasures: public AppBaseForm {
public:
	FormAxMeasures();
	virtual ~FormAxMeasures();
	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXMEASURES_H_ */
