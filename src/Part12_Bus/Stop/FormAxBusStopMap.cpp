/*
 * FormAxBusStopMap.cpp
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#include "FormAxBusStopMap.h"

FormAxBusStopMap::FormAxBusStopMap()
{
}

FormAxBusStopMap::~FormAxBusStopMap()
{
}
bool FormAxBusStopMap::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("정류소 위치");

	return true;
}

result FormAxBusStopMap::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	FormAxBus::OnInitializing();

	return r;
}

result FormAxBusStopMap::OnTerminating(void)
{
	return FormAxBus::OnTerminating();
}
