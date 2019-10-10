/*
 * FormAxWorld.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormAxWorld.h"

FormAxWorld::FormAxWorld()
{
}

FormAxWorld::~FormAxWorld()
{
}

bool FormAxWorld::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("세계 시간");

	return true;
}

result FormAxWorld::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();

	return r;
}

result FormAxWorld::OnTerminating(void)
{
	return BacBaseForm::OnTerminating();
}

void FormAxWorld::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}
