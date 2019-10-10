/*
 * UtilPopupProgress.h
 *
 *  Created on: 2012. 8. 6.
 *      Author: benny
 */

#ifndef UTILPOPUPPROGRESS_H_
#define UTILPOPUPPROGRESS_H_

#include <FBase.h>
#include <FUi.h>

using namespace Osp::Base;

class IPopupProgressUserAction {
public:
	virtual void	OnPopupProgressUserAction(int actionId) = 0;
};


class PopupProgress :
	public Osp::Ui::Controls::Popup
	, public Osp::Ui::IAnimationEventListener
	, public Osp::Ui::IActionEventListener
{
public:
	PopupProgress(bool deleteSelf);
	virtual ~PopupProgress(void);

private:
	bool 										__selfDestroy;
	static Osp::Base::Collection::ArrayList * 	__PAnimationFrameList;
	static Osp::Graphics::Bitmap* 				__PLoadingBitmap;
	Osp::Ui::Controls::Animation* 				__pAnimation;
	Osp::Ui::Controls::Label * 					__pBG;
	Osp::Ui::Controls::Panel * 					__pBGPanel;
	Osp::Ui::Controls::Label * 					__pLabelMessage;

	IPopupProgressUserAction 	* 				__pUserAgent;

public:
	static Osp::Base::Collection::ArrayList * CreateProgressAnimation();

public:
	void ConstructAnimation(Osp::Base::Collection::ArrayList * pArr);
	void Show(const String & message, IPopupProgressUserAction* plistener, int confirmID, const String & confirmStr);
	void SetMessage(const String & message);

public:
	virtual void OnAnimationStopped(const Osp::Ui::Control& source);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 통신 시작, 종료 사이에 보여지는 팝업
class PopupProgressHelper {
public:
	PopupProgressHelper() {};
	virtual ~PopupProgressHelper() {};

protected:
public:
	static PopupProgress * 			__PPopupProgress;

public:
	static 	void 	Show(const String & message, IPopupProgressUserAction* plistener, int confirmID, const String & confirmStr = "확인", bool deleteSelf=true);
	static 	void 	Show(const String & message);
	static 	void 	Hide();

};



#endif /* UTILPOPUPPROGRESS_H_ */
