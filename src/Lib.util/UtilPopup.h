/*
 * UtilPopup.h
 *
 *  Created on: 2012. 6. 24.
 *      Author: benny
 */

#ifndef UTILPOPUP_H_
#define UTILPOPUP_H_

#include <FUi.h>

using namespace Osp::Base;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @class PopupComm
 * 통신중 팝업 창
 */

class PopupComm :
	public Osp::Ui::Controls::Popup
	, public Osp::Ui::IAnimationEventListener
{
public:
	PopupComm();
	virtual ~PopupComm(void);

private:
	Osp::Ui::Controls::Animation* 				__pAnimation;
	static Osp::Base::Collection::ArrayList * 	__PAnimationFrameList;

	static Osp::Graphics::Bitmap* 				__PLoadingBitmap;
	Osp::Ui::Controls::Label*					__pLabel;

public:
	static Osp::Base::Collection::ArrayList * CreateProgressAnimation();

public:
	void ConstructAnimation(Osp::Base::Collection::ArrayList * pArr);

public:
	virtual void OnUserEventReceivedN(RequestId  requestId, Osp::Base::Collection::IList *  pArgs);
	virtual void OnAnimationStopped(const Osp::Ui::Control& source);
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 통신 시작, 종료 사이에 보여지는 팝업
class PopupCommHelper {
public:
	PopupCommHelper() {};
	virtual ~PopupCommHelper() {};

protected:
	static PopupComm * 			__PPopupComm;

public:
	static 	void 	Show();
	static 	void 	Hide();
};



#endif /* UTILPOPUP_H_ */
