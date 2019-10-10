/*
 * AppFormFactory.cpp
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#include "AppFormFactory.h"
#include "FormStart.h"
#include "FormAxGPS.h"
#include "FormAxMap.h"
#include "FormAxMapCenter.h"

#include "FormAxBusAboutStop.h"
#include "FormAxBusAboutLine.h"
#include "FormAxBusAboutFreq.h"
#include "FormAxBusAboutMybus.h"

#include "FormAxBusStopNear.h"
#include "FormAxBusStopLines.h"

//#include "FormAxBusStopSearch.h"
//#include "FormAxBusStopAddr2Near.h"
//#include "FormAxBusStopZip2Near.h"
#include "FormAxBusStopInfo.h"
#include "FormAxBusStopMap.h"
#include "FormAxBusStopNames.h"
#include "FormAxBusLineInfo.h"
#include "FormAxBusLineMap.h"
#include "FormAxBusLineStops.h"

#include "FormAxAddrSearch.h"
#include "FormAxAddresses.h"
#include "FormAxAddrBookmark.h"
#include "FormAxAddressWeb.h"

#include "FormAxPhones.h"
#include "FormAxPhones2.h"
#include "FormAxPhonesDetail.h"

//#include "FormAxCurrency.h"
#include "FormAxCurrencyNaver.h"
#include "FormAxCurrencyDetail.h"
#include "FormAxWorld.h"
#include "FormAxCalc.h"
#include "FormAxMeasures.h"
#include "FormAxSetup.h"
#include "FormAxSetupBus.h"
#include "FormAxSetupShortcut.h"
#include "FormAxSetupWeather.h"

#include "FormAxInfo.h"
//#include "FormAxSeoul.h"
#include "FormAxWeb.h"


AppFormFactory::AppFormFactory()
{
}

AppFormFactory::~AppFormFactory()
{
}

Osp::Ui::Controls::Form * AppFormFactory::CreateForm(RequestId formID)
{
	Osp::Ui::Controls::Form * pFormCreating = null;

	switch (formID) {
	case FORM_ID_APP_HOME:
		pFormCreating = new FormStart();
		break;
	case FORM_ID_GPS:
		pFormCreating = new FormAxGPS();
		break;
	case FORM_ID_MAP:
		pFormCreating = new FormAxMap();
		break;
	case FORM_ID_MAP_CENTER:
		pFormCreating = new FormAxMapCenter();
		break;

//	case FORM_ID_BUS:
//		pFormCreating = new FormaAxBus();
//		break;

	case FORM_ID_BUS_ABOUT_STOP:
		pFormCreating = new FormAxBusAboutStop();
		break;
	case FORM_ID_BUS_ABOUT_LINE:
		pFormCreating = new FormAxBusAboutLine();
		break;
	case FORM_ID_BUS_ABOUT_FREQ:
		pFormCreating = new FormAxBusAboutFreq();
		break;
	case FORM_ID_BUS_ABOUT_MYBUS:
		pFormCreating = new FormAxBusAboutMybus();
		break;

	case FORM_ID_BUS_STOPNEAR:
		pFormCreating = new FormAxBusStopNear();
		break;
	case FORM_ID_BUS_STOPLINES:
		pFormCreating = new FormAxBusStopLines();
		break;

		// 버스 정류소
	case FORM_ID_BUS_STOPINFO:
		pFormCreating = new FormAxBusStopInfo();
		break;
	case FORM_ID_BUS_STOPMAP:
		pFormCreating = new FormAxBusStopMap();
		break;
	case FORM_ID_BUS_STOPNAMES:
		pFormCreating = new FormAxBusStopNames();
		break;
/*
	case FORM_ID_BUS_STOPSEARCH:
		pFormCreating = new FormAxBusStopSearch();
		break;
	case FORM_ID_BUS_STOPADDR2NEAR:
		pFormCreating = new FormAxBusStopAddr2Near();
		break;
	case FORM_ID_BUS_STOPZIP2NEAR:
		pFormCreating = new FormAxBusStopZip2Near();
		break;
*/

		// 버스 노선
	case FORM_ID_BUS_LINEINFO:
		pFormCreating = new FormAxBusLineInfo();
		break;
	case FORM_ID_BUS_LINEMAP:
		pFormCreating = new FormAxBusLineMap();
		break;
	case FORM_ID_BUS_LINESTOPS:
		pFormCreating = new FormAxBusLineStops();
		break;

		// 주소
	case FORM_ID_ADDRESS:
		pFormCreating = new FormAxAddrSearch();
		break;
	case FORM_ID_ADDRESS_ROAD:
		pFormCreating = new FormAxAddrSearch(1);
		break;
	case FORM_ID_ADDRESS_LIST:
		pFormCreating = new FormAxAddresses();
		break;
	case FORM_ID_ADDRESS_BOOKMARK:
		pFormCreating = new FormAxAddrBookmark();
		break;
	case FORM_ID_ADDRESS_WEB:
		pFormCreating = new FormAxAddressWeb();
		break;

	case FORM_ID_PHONES:
		pFormCreating = new FormAxPhones();
		break;
	case FORM_ID_PHONES2:
		pFormCreating = new FormAxPhones2();
		break;
	case FORM_ID_PHONE_DETAIL:
		pFormCreating = new FormAxPhonesDetail();
		break;
	//case FORM_ID_SEOUL:
	//	pFormCreating = new FormAxSeoul();
	//	break;
	case FORM_ID_MEASURES:
		pFormCreating = new FormAxMeasures();
		break;
	case FORM_ID_CURRENCY:
		pFormCreating = new FormAxCurrencyNaver();
		break;
	case FORM_ID_CURRENCY_DETAIL:
		pFormCreating = new FormAxCurrencyDetail();
		break;

	case FORM_ID_WORLD:
		pFormCreating = new FormAxWorld();
		break;
	case FORM_ID_UTILS:
		pFormCreating = new FormAxCalc();
		break;
	case FORM_ID_SETUP_SHORTCUT:
		pFormCreating = new FormAxSetupShortcut();
		break;
	case FORM_ID_SETUP_BUS:
		pFormCreating = new FormAxSetupBus();
		break;
	case FORM_ID_SETUP_WEATHER:
		pFormCreating = new FormAxSetupWeather();
		break;

	case FORM_ID_SHORTCUT:
		break;
	case FORM_ID_INFO:
		pFormCreating = new FormAxInfo();
		break;
	case FORM_ID_WEB:
		pFormCreating = new FormAxWeb();
		break;

	// 화면 이동 명령에 해당하는 ID 사용하지 않도록 이곳에 명기한다.
	case REQUEST_GOTO_BACK:
	case REQUEST_GOTO_BREAD:
	default:
		break;
	}

	return pFormCreating;
}
