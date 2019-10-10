/*
 * UtilApp.h
 *
 *  Created on: 2012. 6. 24.
 *      Author: benny
 */

#ifndef UTILAPP_H_
#define UTILAPP_H_

#include <FBase.h>
#include <FGraphics.h>
#include <FApp.h>

class UtilApp {
public:
	UtilApp();
	virtual ~UtilApp();

public:
	static Osp::Graphics::Bitmap* 		GetBitmap2N(const Osp::Base::String& name);
	static Osp::Base::DateTime 			GetNow();
	static Osp::Base::String 			GetNowString();
	static Osp::Base::String 			GetNowDate();

	static 	void 						InvokeEMail(const Osp::Base::String & message);
	static 	void 						AppVideo(const Osp::Base::String & mp4Name);
	static 	void 						AppWeb(const Osp::Base::String & url);
	static 	void 						AppCall(const Osp::Base::String & number);
	static 	void 						AppDial(const Osp::Base::String & number);
	static 	void 						AppProviderSetting(const Osp::Base::String & category, Osp::App::IAppControlEventListener * plistener);

	static void RunSamsungApps();
	static Osp::Base::String GetSamsungAppsId();
	static void RunApplication(const Osp::Base::String & strAppId, const Osp::Base::String & strData="");
	static Osp::Base::String GetAppId();

};

#endif /* UTILAPP_H_ */
