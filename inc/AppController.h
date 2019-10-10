/*
 * AppController.h
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#ifndef APPCONTROLLER_H_
#define APPCONTROLLER_H_

#include "BacNavigationController.h"
#include <FLocations.h>
#include <FApp.h>

class AppController
	: public BacNavigationController
	, public Osp::Locations::ILocationListener
{
public:
	AppController();
	virtual ~AppController();

public:
	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);

	// app events
public:
	void 	AtAppFG();
	void 	AtAppBG();
	void 	AtAppLowMemory();
	void 	AtAppScreenOn();
	void 	AtAppScreenOff();

	void 	StartLocationService();

public:
	//ILocationListener method
	virtual void OnLocationUpdated(Osp::Locations::Location& location);
	virtual void OnProviderStateChanged(Osp::Locations::LocProviderState newState);

private:
	//int __stateLocationUpdate;
	Osp::Locations::LocationProvider* __pLocationProvider;

protected:
	bool 	start_locationDetection();
	void 	stop_locationDetection();

};

/**
 *


LBS based mashup app (bus information, finding address based on current location, and many others)




 바다에서 위치정보 GPS 활용하기  (GEO Location Service)

 바다에서 위치 정보를 획득하기 위해서는 Osp::Locations::LocationProvider 를 사용해야 한다.
 이를 사용하는 전후 과정이 약간은 복잡하다.


 1. 권한 설정
 - SettingInfo::GetValue("GPSEnabled", ...) 이용하여 현 상태 가져온다
 - false 이면 AppControl 실행
 		AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_SETTINGS, "");
 		-> 이를 실행하는 편의함수 LaunchLocationSetting() 이용한다.
 - AppControl 에서 설정을 마치면, 내 App 의 OnAppControlCompleted() 로 응답이 온다. 따라서
 	 Osp::App::IAppControlEventListener 의 다음 함수를 구현한다.
		LocationManager::OnAppControlCompleted(const Osp::Base::String& appControlId, const Osp::Base::String& operationId, const Osp::Base::Collection::IList* pResultList)
	이때 다시 SettingInfo::GetValue("GPSEnabled", ...) 이용하여 현 상태 가져온다
	false 이면 이를 사용자에게 알려 준다.
	true 이면 아래 2번 실행

 2. Osp::Locations::LocationProvider 설정
 	 start_locationDetection() 에 해당하는 기능 수행

 3. Osp::Locations::ILocationListener 설정
 	 Osp::Locations::ILocationListener 의 두 개의 함수 내용 구현
	- void OnLocationUpdated(Osp::Locations::Location& location);
	- void OnProviderStateChanged(Osp::Locations::LocProviderState newState);

 4. Application FG/BG 처리
 	 GPS 센서를 이용하는 것은 밧데리 소모량이 많다.
 	 따라서 BG 로 전환되는 순간 위치 탐지를 멈추도록 한다.
 	 App의 BG/FG 순간에
 	 stop_locationDetection()
 	 start_locationDetection()

 *
 *
 */


#endif /* APPCONTROLLER_H_ */
