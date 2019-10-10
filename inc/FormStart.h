#ifndef _FORMSTART_H_
#define _FORMSTART_H_

#include "AppBaseForm.h"
#include "HttpDaum.h"
#include "HttpGoogleWeather.h"
#include "HttpKmaWeather.h"
#include "UtilRegistry.h"


typedef enum {
	READY,
	QUERYING,
	DONE,
	ERROR
} STATE_GETTING_WEATHER;


class FormStart
	: public AppBaseForm
	, public UtilRegistry
	, public IHttpFirstResponder
	, public Osp::App::IAppControlEventListener
{
public:
	FormStart(void);
	virtual ~FormStart(void);
	virtual bool AtInitialize();

// Implementation
protected:
	static 	const 	int 			ID_MESSAGEBOX_CONFIRM = 101;
	static 	const 	int 			ID_MESSAGEBOX_APPCONTROL_SETTINGS = 102;
	static 	const 	int 			ID_MESSAGEBOX_APPCONTROL_SETTINGS_BAD = 103;
	static 	const 	int 			ID_UPDATE_WEATHER = 111;
	static 	const 	int 			ID_SETUP_WEATHER = 112;
	static 	const 	int 			ID_TEST_PROGRESS_POPUP = 121;


	HttpDaum 			* 			__pHttpDaumGeoCode;
	HttpGoogleWeather 	* 			__pHttpGoogleWeather;
	HttpKmaWeather 		* 			__pHttpKmaWeather;
	HandleTimer 		* 			__pTimerWaitGps;
	HandleTimer 		* 			__pTimerAds;
	HandleTimer 		* 			__pTimerShowAds;

	Panel 				* 			__pPanelWeather;
	Panel 				* 			__pPanelTopMenu;
	Panel 				* 			__pPanelAds;
	int 							__nCountShowAds;

	Panel 				* 			__pPanelWeatherProgress;
	Panel 				* 			__pPanelWeatherGoogle;
	Panel 				* 			__pPanelWeatherKma;

	Progress  			* 			__pProgressWaitGPS;
	int 							__nCountWaitGPS;

	MyLabel 			* 			__pLabelProgress;

	MyLabel 			* 			__pLabelWeatherIcon;
	MyLabel 			* 			__pLabelWeatherAddr;
	MyLabel 			* 			__pLabelWeatherDate;
	MyLabel 			* 			__pLabelWeatherDegree;
	MyLabel 			* 			__pLabelWeatherCondition;
	MyLabel 			* 			__pLabelWeatherWind;
	MyLabel 			* 			__pLabelWeatherHumidity;

	MyLabel 			* 			__pLabelWeatherForcastWeekDay[4];
	MyLabel 			* 			__pLabelWeatherForcastIcon[4];
	MyLabel 			* 			__pLabelWeatherForcastDegree[4];

	STATE_GETTING_WEATHER			__stateGetWeather;

protected:
	void 		init();
	void 		shortCut();
	void 		gotoShortcut();
	void 		checkupGpsAuthority();
	void 		startLocationService();

	bool 		invokeReverseGeoCoding();
	bool 		invokeWeather();

	void 		layoutWeather(Panel * pContainer);
	void 		layoutTopMenu(Panel * pContainer);
	void 		showWeatherGoogle();
	void 		showWeatherKma();

	void 		updateProgress();

	bool 		onHttpDaumGeoCode(ByteBuffer* pBuffer);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	virtual bool AtWakeup();
	virtual void AtTimer(HandleTimer * pTimerHandler);
	virtual void AtLocation();

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

	// Osp::App::IAppControlEventListener
	virtual void OnAppControlCompleted(const Osp::Base::String& providerId, const Osp::Base::String& operationId, const Osp::Base::Collection::IList* pResultList);
};

#endif	//_FORMSTART_H_
