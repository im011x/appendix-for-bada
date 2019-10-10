/*
 * FormAxInfo.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXINFO_H_
#define FORMAXINFO_H_

#include "FormAxWeb.h"

class FormAxInfo: public FormAxWeb {
public:
	FormAxInfo();
	virtual ~FormAxInfo();
	virtual bool AtInitialize();

protected:
	virtual 	void buildFooter();

// Implementation
protected:
	void 	loadAbout(int nTab);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	virtual bool  OnLoadingRequested (const Osp::Base::String &url, WebNavigationType type);
};

#endif /* FORMAXINFO_H_ */
