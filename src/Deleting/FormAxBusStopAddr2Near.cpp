/*
 * FormAxBusStopAddr2Near.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusStopAddr2Near.h"

FormAxBusStopAddr2Near::FormAxBusStopAddr2Near()
{
}

FormAxBusStopAddr2Near::~FormAxBusStopAddr2Near()
{
}

bool FormAxBusStopAddr2Near::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("주소로 정류소 찾기");

	return true;
}

result FormAxBusStopAddr2Near::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	return r;
}

result FormAxBusStopAddr2Near::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}
