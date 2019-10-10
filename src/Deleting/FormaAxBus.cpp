/*
 * FormaAxBus.cpp
 *
 *  Created on: 2012. 6. 16.
 *      Author: benny
 */

#include "FormaAxBus.h"

FormaAxBus::FormaAxBus()
{
	__httpSeoulBus = null;
}

FormaAxBus::~FormaAxBus()
{
}

bool FormaAxBus::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("서울 시내버스");

	return true;
}

result FormaAxBus::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	BacBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();

	//
	//__httpSeoulBus = new HttpBusSeoul;

	///////////////////////////////////////////////////////////////////////////
	//__httpSeoulBus->GetStopsByName("광화문");  // Not OK
	//__httpSeoulBus->GetArrivingByStop("15203");  // OK
	//__httpSeoulBus->GetLanesByStop("15203");  // OK
	//__httpSeoulBus->GetTimesBy("15203", "4663800");

	//__httpSeoulBus->GetLowStopsByName("광화문");  // Not OK
	//__httpSeoulBus->GetLowArrivingByStop("02134");  // OK
	//__httpSeoulBus->GetStopsByPosition("126.981427", "37.570361", "500");

	///////////////////////////////////////////////////////////////////////////
	//__httpSeoulBus->GetArrivingByLane("3060000");  // OK
	//__httpSeoulBus->GetArrivingBy("17001", "3060000");  // 신도림역, 600번  // Not OK
	//__httpSeoulBus->GetLowArrivingByLane("3060000"); // 600번, Not OK
	//__httpSeoulBus->GetLowArrivingBy("02134", "3060000"); // 서대문, 600번 // Not OK

	///////////////////////////////////////////////////////////////////////////
	__httpSeoulBus->GetPostionsByName("광화문");

	return r;
}

result FormaAxBus::OnTerminating(void)
{
	delete __httpSeoulBus;
	return BacBaseForm::OnTerminating();
}

void FormaAxBus::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	default:
		BacBaseForm::OnActionPerformed(source, actionId);
		break;
	}
}
