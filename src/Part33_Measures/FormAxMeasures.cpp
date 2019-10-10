/*
 * FormAxMeasures.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxMeasures.h"

FormAxMeasures::FormAxMeasures()
{
}

FormAxMeasures::~FormAxMeasures()
{
}

bool FormAxMeasures::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("도량형 환산");

	return true;
}

result FormAxMeasures::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();

	return r;
}

result FormAxMeasures::OnTerminating(void)
{
	return BacBaseForm::OnTerminating();
}

void FormAxMeasures::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}
