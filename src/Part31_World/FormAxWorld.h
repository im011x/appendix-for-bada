/*
 * FormAxWorld.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXWORLD_H_
#define FORMAXWORLD_H_

#include "AppBaseForm.h"

class FormAxWorld: public AppBaseForm {
public:
	FormAxWorld();
	virtual ~FormAxWorld();
	virtual bool AtInitialize();

// Implementation
protected:

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXWORLD_H_ */
