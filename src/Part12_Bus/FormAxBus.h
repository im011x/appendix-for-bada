/*
 * FormAxBus.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUS_H_
#define FORMAXBUS_H_

#include "AppBaseForm.h"

class FormAxBus: public AppBaseForm
{
public:
	FormAxBus();
	virtual ~FormAxBus();

protected:
	virtual 	void buildHeader();
	virtual 	void buildFooter();

protected:
	Bitmap * 	GetBitmapBus(const String & routeType);

private:
	int 		__tabID;
	static int 	__lastTabIndex;

private:
	void	setFooterFocus();
	void 	setBread();
	void 	setTabID();

public:
	virtual result OnInitializing(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXBUS_H_ */
