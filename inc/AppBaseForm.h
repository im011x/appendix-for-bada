/*
 * AppBaseForm.h
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#ifndef APPBASEFORM_H_
#define APPBASEFORM_H_

#include "BacBaseForm.h"
#include "ControlMaker.h"
#include "AppID.h"
#include <FAds.h>

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;


class AppBaseForm
	: public BacBaseForm
	, public ControlMaker
	, public Osp::Ads::Controls::IAdListener
{
public:
	AppBaseForm();
	virtual ~AppBaseForm();

private:
	String 		__titleName;
	String 		__tabNames[4];

	static 	const 	int 			TIMER_INTERVAL_ADS = 12;

protected:
	static 		bool 		__currPosStable;
	static 		String 		__currPosLatitude;
	static 		String 		__currPosLongitude;

	int 		__nSelectedIndexOfAnyList;

	String 		__imageHeaderButtonNormal;
	String 		__imageHeaderButtonPressed;
	int 		__actionIdHeaderButton;

	Osp::Ads::Controls::Ad* 		__pAd;
	HandleTimer * 	 				__pAdsTimer;

protected:
	void		SetTitle(const String & name);
	void 		SetTabName(int index, const String & name);

	void 		SetHeaderButton(const String & imageNormal, const String & imagePressed, int actionId);
	void 		SetRefreshButton(int actionId);
	void 		requestAds();

protected:
	virtual 	void buildHeader();
	virtual 	void buildFooter();

	void 		commShow(bool builtIn = false);
	void 		commHide(bool builtIn = false);

	HandleTimer * 	createAds(Rectangle rect, Container * bottom, bool bForeverTimer = false);

public:
	static 		void 	SetLatitLongi(const String & latit, const String longi);

	// Osp::Ads::Controls::IAdListener
	virtual void OnAdReceived(RequestId reqId, const Osp::Ads::Controls::Ad& source, result r,
			const Osp::Base::String& errorCode, const Osp::Base::String& errorMsg);
	virtual void AtTimer(HandleTimer * pTimerHandler);

};

#endif /* APPBASEFORM_H_ */
