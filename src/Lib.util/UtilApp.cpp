/*
 * UtilApp.cpp
 *
 *  Created on: 2012. 6. 24.
 *      Author: benny
 */

#include "UtilApp.h"
#include <FSystem.h>

using namespace Osp::Graphics;
using namespace Osp::App;
using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::System;

UtilApp::UtilApp()
{
}

UtilApp::~UtilApp()
{
}

Osp::Graphics::Bitmap* UtilApp::GetBitmap2N(const Osp::Base::String& name)
{
	//AppLogDebug("UtilApp::GetBitmap2N - %S", name.GetPointer());
	AppResource* pAppResource = Application::GetInstance()->GetAppResource();
	Bitmap* pBitmap = pAppResource->GetBitmapN(name);
	return pBitmap;
}

Osp::Base::DateTime UtilApp::GetNow()
{
	Osp::Base::DateTime dtNow;
	Osp::System::SystemTime::GetCurrentTime(Osp::System::STANDARD_TIME, dtNow);
	return dtNow;
}

Osp::Base::String UtilApp::GetNowString()
{
	Osp::Base::DateTime dtNow;
	Osp::System::SystemTime::GetCurrentTime(Osp::System::STANDARD_TIME, dtNow);

	String sNow;
	sNow.Format(64, L"%04d-%02d-%02d %02d:%02d:%02d", dtNow.GetYear(), dtNow.GetMonth(), dtNow.GetDay(), dtNow.GetHour(), dtNow.GetMinute(), dtNow.GetSecond());

	return sNow;
}

Osp::Base::String UtilApp::GetNowDate()
{
	Osp::Base::DateTime dtNow;
	Osp::System::SystemTime::GetCurrentTime(Osp::System::STANDARD_TIME, dtNow);

	String sNow;
	sNow.Format(64, L"%04d-%02d-%02d", dtNow.GetYear(), dtNow.GetMonth(), dtNow.GetDay());

	return sNow;
}

void UtilApp::InvokeEMail(const Osp::Base::String & message)
{
	// Email AppControl
}

void UtilApp::AppVideo(const String & mp4Name)
{
	AppLogDebug("PlayVideo(%S)", mp4Name.GetPointer());

	ArrayList * parr = new ArrayList();
	parr->Construct();


	parr->Add(* new String("type:video"));
	parr->Add(* new String("path:" + mp4Name));

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_VIDEO, APPCONTROL_OPERATION_PLAY);
	if (pAc)    {
		pAc->Start(parr, null);
		delete pAc;
	}

	parr->RemoveAll(true);
	delete parr;
}

void UtilApp::AppWeb(const Osp::Base::String & url)
{
	ArrayList* parr = null;
	parr = new ArrayList();
	parr->Construct();

	String *pData = new String();
	pData->Append("url:");
	pData->Append(url);
	parr->Add( *pData );

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_BROWSER, OPERATION_VIEW);
	if(pAc)
	{
		pAc->Start(parr, null);
		delete pAc;
	}

	parr->RemoveAll(true);
	delete parr;
}

void UtilApp::AppCall(const Osp::Base::String & number)
{
	ArrayList* parr = null;
	parr = new ArrayList();
	parr->Construct();

	String strNumber = "tel:";
	strNumber.Append(number);
	strNumber.Replace("-", "", 0);

	parr->Add(*(new String(strNumber)));
	parr->Add(*(new String("type:voice")));

	// Call AppControl-PhoneCall
	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_CALL, OPERATION_MAIN);
	if(pAc) {
		pAc->Start(parr, null);	// Send Data to AppControl
		delete pAc;
	}

	parr->RemoveAll(true);
	delete parr;
}

void UtilApp::AppDial(const Osp::Base::String & number)
{
	ArrayList* parr = null;
	parr = new ArrayList();
	parr->Construct();

	String strNumber = "tel:";
	strNumber.Replace("-", "", 0);
	strNumber.Append(number);

	parr->Add(* (new String(strNumber)));

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_DIAL, "");
	if(pAc)
	{
	   pAc->Start(parr, null);
	   delete pAc;
	}

	parr->RemoveAll(true);
	delete parr;
}

void UtilApp::AppProviderSetting(const Osp::Base::String & category, IAppControlEventListener * plistener)
{
	ArrayList* pDataList = new ArrayList();
	String* pData = new String("category:");  // "category:Location"
	pData->Append(category);

	pDataList->Construct();
	pDataList->Add(*pData);

	AppControl* pAc = AppManager::FindAppControlN(APPCONTROL_PROVIDER_SETTINGS, "");
	if(pAc){
		AppLogDebug("hmmm created ...");
		pAc->Start(pDataList, plistener);
		delete pAc;
	}
	else
		AppLogDebug("hmmm No No No!!");

	pDataList->RemoveAll(true);
	delete pDataList;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// 삼성 앱스 어플 실행
void UtilApp::RunSamsungApps()
{
	// 삼성 앱스 어플리케이션 ID를 반환
	String strSamsungAppsId = GetSamsungAppsId();

	// 자기자신의 App ID를 반환
	String strThisAppId = GetAppId();

	// 특정 어플을 실행
	RunApplication(strSamsungAppsId, strThisAppId);
}

// 삼성 앱스 어플리케이션 ID를 반환
String UtilApp::GetSamsungAppsId()
{
	String strSamsungAppsId, key(L"SamsungAppsAppId");
	SystemInfo::GetValue(key, strSamsungAppsId);

	return strSamsungAppsId;
}

// 자기자신의 App ID를 반환
String UtilApp::GetAppId()
{
	return Application::GetInstance()->GetAppId();
}

// 특정 어플을 실행
void UtilApp::RunApplication(const String & strAppId, const String & strData)
{
	ArrayList *pArgList = null;
	if( strData.GetLength() > 0 ) {
		pArgList = new ArrayList();
		pArgList->Construct();
		String *aArg = new String(strData);
		pArgList->Add(*aArg);
	}

	AppManager::GetInstance()->LaunchApplication(strAppId, pArgList, AppManager::LAUNCH_OPTION_DEFAULT);

	if( pArgList ) {
		pArgList->RemoveAll();
		delete pArgList;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

