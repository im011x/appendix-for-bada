/*
 * AppID.h
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#ifndef APPID_H_
#define APPID_H_

#include "BacDefine.h"

static const int 		FOOTER_ID_ITEM1 		= BAC_USER_FOOTER_START + 1;
static const int 		FOOTER_ID_ITEM2 		= BAC_USER_FOOTER_START + 2;
static const int 		FOOTER_ID_ITEM3 		= BAC_USER_FOOTER_START + 3;
static const int 		FOOTER_ID_ITEM4 		= BAC_USER_FOOTER_START + 4;
static const int 		FOOTER_ID_ITEM5 		= BAC_USER_FOOTER_START + 5;

static const int 		FORM_ID_APP_HOME 		= 0x01000000;
static const int 		FORM_ID_GPS 			= 0x01010000;

static const int 		FORM_ID_MAP						= 0x01100000;
static const int 		FORM_ID_MAP_CENTER				= 0x01100001;

//static const int 		FORM_ID_BUS 					= 0x01200000;
static const int 		FORM_ID_BUS_ABOUT_STOP 				= 0x01210001;
static const int 		FORM_ID_BUS_ABOUT_LINE 				= 0x01210002;
static const int 		FORM_ID_BUS_ABOUT_FREQ 				= 0x01210003;
static const int 		FORM_ID_BUS_ABOUT_MYBUS				= 0x01210004;

static const int 		FORM_ID_BUS_STOPSEARCH 				= 0x01220001;
static const int 		FORM_ID_BUS_STOPNEAR 				= 0x01220002;
static const int 		FORM_ID_BUS_STOPNAMES 				= 0x01220003;
static const int 		FORM_ID_BUS_STOPLINES 				= 0x01220004;
static const int 		FORM_ID_BUS_STOPADDR2NEAR 			= 0x01220005;
static const int 		FORM_ID_BUS_STOPZIP2NEAR 			= 0x01220006;
static const int 		FORM_ID_BUS_STOPINFO 				= 0x01220007;
static const int 		FORM_ID_BUS_STOPMAP 				= 0x01220008;

static const int 		FORM_ID_BUS_LINEINFO 				= 0x01230001;
static const int 		FORM_ID_BUS_LINEMAP 				= 0x01230002;
static const int 		FORM_ID_BUS_LINESTOPS 				= 0x01230003;


static const int 		FORM_ID_ADDRESS 			= 0x01300000;
static const int 		FORM_ID_ADDRESS_ROAD		= 0x01300001;
static const int 		FORM_ID_ADDRESS_LIST 		= 0x01300002;
static const int 		FORM_ID_ADDRESS_BOOKMARK	= 0x01300003;
static const int 		FORM_ID_ADDRESS_WEB			= 0x01300004;


static const int 		FORM_ID_PHONES		 	= 0x01400000;
static const int 		FORM_ID_PHONES2		 	= 0x01400001;
static const int 		FORM_ID_PHONE_DETAIL 	= 0x01400002;

static const int 		FORM_ID_SEOUL		 	= 0x01500000;
static const int 		FORM_ID_MEASURES 		= 0x01600000;
static const int 		FORM_ID_CURRENCY 		= 0x01700000;
static const int 		FORM_ID_CURRENCY_DETAIL	= 0x01700001;
static const int 		FORM_ID_WORLD	 		= 0x01800000;
static const int 		FORM_ID_UTILS 	 		= 0x01900000;

static const int 		FORM_ID_SETUP_SHORTCUT	= 0x01a00001;
static const int 		FORM_ID_SETUP_BUS 	 	= 0x01a00002;
static const int 		FORM_ID_SETUP_WEATHER 	= 0x01a00003;
static const int 		FORM_ID_SHORTCUT 	 	= 0x01b00000;
static const int 		FORM_ID_INFO 	 		= 0x01c00000;
static const int 		FORM_ID_WEB 	 		= 0x01d00000;


#endif /* APPID_H_ */
