/*
 * FormAxSetup.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXSETUP_H_
#define FORMAXSETUP_H_

#include "AppBaseForm.h"

class FormAxSetup
	: public AppBaseForm
{
public:
	FormAxSetup();
	virtual ~FormAxSetup();

protected:
	virtual void buildFooter();

// Implementation
protected:
	void setFooterFocus();

public:
	virtual result OnInitializing(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXSETUP_H_ */
