/*
 * AppController.cpp
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#include "AppController.h"
#include "AppFormFactory.h"
#include "AppBaseForm.h"
#include "Intent.h"
#include <FSystem.h>
#include <FApp.h>
#include "UtilApp.h"

using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::System;
using namespace Osp::Locations;
using namespace Osp::Locations::Services;
using namespace Osp::Locales;

AppController::AppController()
{
	 __pLocationProvider = null;
}

AppController::~AppController()
{
}

bool AppController::Initialize()
{
	BacNavigationController::Initialize();

	return true;
}

result AppController::OnInitializing(void)
{
	result r = E_SUCCESS;

	BacBaseForm::SetDefaultBackgroundColor(Color::COLOR_GREY);
	ControlMaker::INIT_Pool();
	Intent::Init_Intent();

	SetFactory(new AppFormFactory(), FORM_ID_APP_HOME);
	Activate(true);

	//StartLocationService();

	return r;
}

result AppController::OnTerminating(void)
{
	result r = E_SUCCESS;

	// TODO: Add your termination code here
	ControlMaker::FINI_Pool();
	Intent::Fini_Intent();

	// location ....
	if(__pLocationProvider){
		stop_locationDetection();

		delete __pLocationProvider;
		__pLocationProvider = null;
	}

	return r;
}

void AppController::StartLocationService()
{
	if (__pLocationProvider == null) {
		__pLocationProvider = new LocationProvider();
		result r = __pLocationProvider->Construct(LOC_METHOD_HYBRID);
		if( IsFailed(r) )	{
			delete __pLocationProvider;
			__pLocationProvider = null;
			AppLogException("[%s] Construct is failed.", GetErrorMessage(r));

			return;
		}
	}

	start_locationDetection();
}

bool AppController::start_locationDetection()
{
	result r = E_SUCCESS;
#if 0
	int intervalInSec = 2;
	r = __pLocationProvider->RequestLocationUpdates(*this, intervalInSec, true);  // 특정 시간 주기로 GPS 위치 받는다.
#else
	int checkingIntervalInSec = 1;
	float distanceInMeter = 100.0;
	r = __pLocationProvider->RequestLocationUpdates(*this, checkingIntervalInSec, distanceInMeter, true);  // 특정시간 그리고 특정 범위를 벗어난 경우에만 받는다.
#endif

	if(IsFailed(r)) {
		AppLogDebug("fail to RequestLocationUpdates - %s", GetErrorMessage(r));
		return false;
	}

	LocProviderState state =  __pLocationProvider->GetState();

	OnProviderStateChanged(state);

	if (state == LOC_PROVIDER_AVAILABLE)
	{
		AppLogDebug("Location : Available");
	}
	else if(state == LOC_PROVIDER_TEMPORARILY_UNAVAILABLE)
	{
		AppLogDebug("Location : Temporarily Unavailable");
	}
	else
	{
		AppLogDebug("Location : Out of Service");
	}

	return true;
}

void AppController::stop_locationDetection()
{
	if (__pLocationProvider)
		__pLocationProvider->CancelLocationUpdates();

}

void AppController::OnLocationUpdated(Osp::Locations::Location& location)
{
	const QualifiedCoordinates* pCoor = location.GetQualifiedCoordinates();

	bool bOK = false;
	String showData = "";

	if (location.IsValid() && pCoor) {
		bOK = true;

		showData.Append("Coordinates(" + Double::ToString(pCoor->GetLatitude())+
				", " + Double::ToString(pCoor->GetLongitude()) +
				", " + Float::ToString(pCoor->GetAltitude()) + ")\n");

		showData.Append("Speed	: " + Float::ToString(location.GetSpeed()) + "\n");
		showData.Append("Course	: " + Float::ToString(location.GetCourse())+ "\n");
		showData.Append("vAcc		: " + Float::ToString(pCoor->GetVerticalAccuracy())+ "\n");
		showData.Append(" hAcc: " + Float::ToString(pCoor->GetHorizontalAccuracy()) + L"\n");

		//
		AppBaseForm::SetLatitLongi(Double::ToString(pCoor->GetLatitude()), Double::ToString(pCoor->GetLongitude()));
	}
	else {
		bOK = false;

		showData.Append("Invalid location\n");
		showData.Append("ErrorCode: " + Integer::ToString(location.GetErrorCode()));

		if (location.GetErrorCode() == 102) {
			showData.Append(", Location services are disabled.\n");
		} else {
			showData.Append("\n");
		}

	}

	AppLogDebug("Location updated : %S", showData.GetPointer());

	if (bOK && __pFormHome) {
		__pFormHome->AtLocation();
	}
}

void AppController::OnProviderStateChanged(Osp::Locations::LocProviderState newState)
{
	switch (newState) {
	case LOC_PROVIDER_AVAILABLE:
		AppLogDebug("Location : Available");
		break;
	case LOC_PROVIDER_TEMPORARILY_UNAVAILABLE:
		AppLogDebug("Location : Temporarily Unavailable");
		break;
	default:
		AppLogDebug("Location : Out of Service");
		break;
	}
}

void AppController::AtAppFG()
{
	AppLogDebug("");
	if (__pFormActive) {
		__pFormActive->AtWakeup();
	}
}

void AppController::AtAppBG()
{
	AppLogDebug("");
	if (__pFormActive) {
		__pFormActive->AtSleep();
	}
}

void AppController::AtAppLowMemory()
{
	AppLogDebug("");

}

void AppController::AtAppScreenOn()
{
	AppLogDebug("");

}

void AppController::AtAppScreenOff()
{
	AppLogDebug("");

}
