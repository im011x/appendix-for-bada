/*
 * AppBaseForm.cpp
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#include "AppBaseForm.h"
#include "UtilPopup.h"

String 		AppBaseForm::__currPosLatitude = "37.5665";   // 서울시청
String 		AppBaseForm::__currPosLongitude = "126.9780";
bool 		AppBaseForm::__currPosStable = false;

using namespace Osp::Ads::Controls;
using namespace Osp::Base;
using namespace Osp::Base::Collection;

AppBaseForm::AppBaseForm() : __pAd(null), __pAdsTimer(null)
{
	__titleName = "Appendix";

	__tabNames[0] = "탭 1";
	__tabNames[1] = "탭 2";
	__tabNames[2] = "탭 3";
	__tabNames[3] = "탭 4";

	__nSelectedIndexOfAnyList = -1;

	__actionIdHeaderButton = -1;
}

AppBaseForm::~AppBaseForm()
{
}

void AppBaseForm::SetTitle(const String & name)
{
	__titleName = name;

	/*
	String s;
	s.Append(__titleName);
	s.Append(" 화면으로 이동 합니다.");
	TTS(s);
	 */
}

void AppBaseForm::SetTabName(int index, const String & name)
{
	if (index < 0 || index > 3)
		return;

	__tabNames[index] = name;
}

void AppBaseForm::SetHeaderButton(const String & imageNormal, const String & imagePressed, int actionId)
{
	__imageHeaderButtonNormal = imageNormal;
	__imageHeaderButtonPressed = imagePressed;
	__actionIdHeaderButton = actionId;
}

void AppBaseForm::SetRefreshButton(int actionId)
{
	SetHeaderButton("appendix/refresh.png", "appendix/refresh_ov.png", actionId);
}

void AppBaseForm::buildHeader()
{
	Header * pHeader = GetHeader();
	if (!pHeader)
		return;

	pHeader->RemoveAllItems();
	pHeader->RemoveAllButtons();

	pHeader->SetStyle(HEADER_STYLE_TITLE);

	Bitmap * pbmp = loadBitmapPool("appendix/title_bg.png");
	if (pbmp)
		pHeader->SetBackgroundBitmap(pbmp);

	if (!IsHome()) {
		ButtonItem * btnBack = makeButtonItem("appendix/back.png", "appendix/back.png", REQUEST_GOTO_BACK);
		pHeader->SetButton(BUTTON_POSITION_LEFT, *btnBack);
	}

	// GPS에 맞는 이미지 가져다 넣자!
#if 0
	ButtonItem * btnGPS = makeButtonItem("btn_back.png", "btn_back_ov.png", FORM_ID_GPS);
	pHeader->SetButton(BUTTON_POSITION_RIGHT, *btnGPS);
#endif

	if (__actionIdHeaderButton >= 0) {
		ButtonItem * btnRight = makeButtonItem(__imageHeaderButtonNormal, __imageHeaderButtonPressed, __actionIdHeaderButton);
		pHeader->SetButton(BUTTON_POSITION_RIGHT, *btnRight);
	}

	pHeader->AddActionEventListener(*this);

	pHeader->SetTitleText(__titleName);

}

void AppBaseForm::buildFooter()
{
	Footer* pFooter = GetFooter();
	if (!pFooter) {
		return;
	}

	//pFooter->SetStyle(FOOTER_STYLE_SEGMENTED_TEXT);
	//pFooter->SetBackButton();

	pFooter->SetStyle(FOOTER_STYLE_TAB);

	FooterItem footerItem1;
	footerItem1.Construct(FOOTER_ID_ITEM1);
	footerItem1.SetText(__tabNames[0]);
	pFooter->AddItem(footerItem1);

	FooterItem footerItem2;
	footerItem2.Construct(FOOTER_ID_ITEM2);
	footerItem2.SetText(__tabNames[1]);
	pFooter->AddItem(footerItem2);

	FooterItem footerItem3;
	footerItem3.Construct(FOOTER_ID_ITEM3);
	footerItem3.SetText(__tabNames[2]);
	pFooter->AddItem(footerItem3);

	FooterItem footerItem4;
	footerItem4.Construct(FOOTER_ID_ITEM4);
	footerItem4.SetText(__tabNames[3]);
	pFooter->AddItem(footerItem4);

	pFooter->AddActionEventListener(*this);

}

void AppBaseForm::SetLatitLongi(const String & latit, const String longi)
{
	__currPosLatitude = latit;
	__currPosLongitude = longi;

	AppBaseForm::__currPosStable = true;

	//AppLogDebug("@@@ 현재 위도/경도 좌표 = %S/%S", __latitude.GetPointer(), __longitude.GetPointer());
}


void AppBaseForm::commShow(bool builtIn)
{
	if (builtIn) {
		AppLogDebug("start the form-attached-progress!");
		transitAnimationStart();
	}
	else {
		PopupCommHelper::Show();
	}
}

void AppBaseForm::commHide(bool builtIn)
{
	if (builtIn) {
		AppLogDebug("stop the form-attached-progress!");
		transitAnimationStop();
	}
	else {
		PopupCommHelper::Hide();
	}
}

HandleTimer * AppBaseForm::createAds(Rectangle rect, Container * bottom, bool bForeverTimer)
{
	// Create an Ad control
	__pAd = new Ad();
	//r = __pAd->Construct(Osp::Graphics::Rectangle(0, 0, 480, 80), L"2011000001_001", this);
	result r = __pAd->Construct(rect, L"xv0c00000000tt", this);

	if (IsFailed(r))
	{
		AppLog("[%s] Ad construction failed.", GetErrorMessage(r));
		return null;
	}
	else
	{
		/**
		 *  Set keywords
		 */
		ArrayList * pKeywordsList = new Osp::Base::Collection::ArrayList();
		pKeywordsList->Construct();
		pKeywordsList->Add(*(new String(L"bada")));
		pKeywordsList->Add(*(new String(L"Samsung")));
		//pKeywordsList->Add(*(new String(L"baby")));
		//pKeywordsList->Add(*(new String(L"beauty")));
		//pKeywordsList->Add(*(new String(L"food")));
		//pKeywordsList->Add(*(new String(L"car")));

		__pAd->SetKeywords(pKeywordsList);

		if (pKeywordsList) {
			pKeywordsList->RemoveAll(true);
			delete pKeywordsList;
			pKeywordsList = null;
		}

		/**
		 * Set extra information
		 */
		HashMap * pExtraInfoList = new Osp::Base::Collection::HashMap();
		pExtraInfoList->Construct();
		pExtraInfoList->Add(*(new String(L"gender")), *(new String(L"male")));
		pExtraInfoList->Add(*(new String(L"age")), *(new String(L"30")));
		//pExtraInfoList->Add(*(new String(L"interests")), *(new String(L"accessories,baby,beauty,car,computer,food,soccer,travel")));
		//pExtraInfoList->Add(*(new String(L"interests")), *(new String(L"accessories,beauty,computer,food,soccer")));
		pExtraInfoList->Add(*(new String(L"interests")), *(new String(L"car,soccer")));
		pExtraInfoList->Add(*(new String(L"location")), *(new String(L"37.5,127.05, 10.5")));

		__pAd->SetExtraInfo(pExtraInfoList);

		if (pExtraInfoList) {
			pExtraInfoList->RemoveAll(true);
			delete pExtraInfoList;
			pExtraInfoList = null;
		}
	}

	// Add controls
	bottom->AddControl(*__pAd);

	AppLog("Ad construction succeed.");

	requestAds();

	//__pAdsTimer = CreateTimer(TIMER_INTERVAL_ADS, 0, bForeverTimer);
	__pAdsTimer = CreateTimer(TIMER_INTERVAL_ADS, 0, false);

	return __pAdsTimer;
}

void AppBaseForm::requestAds()
{
	if (__pAd) {
		RequestId reqId = INVALID_REQUEST_ID;
		__pAd->RequestAd(reqId);
		//AppLogDebug("RequestAd with ID = %d", reqId);
	}
}

void AppBaseForm::OnAdReceived(RequestId reqId, const Osp::Ads::Controls::Ad& source, result r, const Osp::Base::String& errorCode, const Osp::Base::String& errorMsg)
{
	AppLogDebug("RequestAd response : (reqId=%d, r=%s, errorCode=%S, errorMsg=%S)", reqId,
			GetErrorMessage(r), errorCode.GetPointer(), errorMsg.GetPointer());

	if (__pAdsTimer) {
		__pAdsTimer->Start(TIMER_INTERVAL_ADS * 1000);
	}
}

void AppBaseForm::AtTimer(HandleTimer * pTimerHandler)
{
	if (__pAdsTimer == pTimerHandler) {
		requestAds();
	}
}
