/*
 * FormAxGPS.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxGPS.h"

FormAxGPS::FormAxGPS()
{
}

FormAxGPS::~FormAxGPS()
{
}

bool FormAxGPS::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("GPS 상태");

	return true;
}

result FormAxGPS::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();

	return r;
}

result FormAxGPS::OnTerminating(void)
{
	return BacBaseForm::OnTerminating();
}

void FormAxGPS::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}
