/*
 * FormAxBusStopZip2Near.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusStopZip2Near.h"

FormAxBusStopZip2Near::FormAxBusStopZip2Near()
{
}

FormAxBusStopZip2Near::~FormAxBusStopZip2Near()
{
}

bool FormAxBusStopZip2Near::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("우편번호로 정류소찾기");

	return true;
}

result FormAxBusStopZip2Near::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	return r;
}

result FormAxBusStopZip2Near::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}
