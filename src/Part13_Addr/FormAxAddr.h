/*
 * FormAxAddr.h
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#ifndef FORMAXADDR_H_
#define FORMAXADDR_H_

#include "AppBaseForm.h"

class FormAxAddr
	: public AppBaseForm
{
public:
	FormAxAddr();
	virtual ~FormAxAddr();

protected:
	virtual 	void buildHeader();
	virtual 	void buildFooter();

private:
	void	setFooterFocus();

public:
	virtual result OnInitializing(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

#endif /* FORMAXADDR_H_ */
