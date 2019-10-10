/*
 * FormAxCalc.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxCalc.h"

FormAxCalc::FormAxCalc()
{
}

FormAxCalc::~FormAxCalc()
{
}

bool FormAxCalc::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("계산기");

	return true;
}

result FormAxCalc::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();

	return r;
}

result FormAxCalc::OnTerminating(void)
{
	return BacBaseForm::OnTerminating();
}

void FormAxCalc::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}
