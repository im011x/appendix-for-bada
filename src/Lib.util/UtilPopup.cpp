/*
 * UtilPopup.cpp
 *
 *  Created on: 2012. 6. 24.
 *      Author: benny
 */

#include "UtilPopup.h"
#include "UtilApp.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

ArrayList 			* 		PopupComm::__PAnimationFrameList = null;
Bitmap				* 		PopupComm::__PLoadingBitmap = null;

PopupComm 			* 		PopupCommHelper::__PPopupComm = null;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

PopupComm::PopupComm() : __pAnimation(null)
{
	Osp::Ui::Controls::Popup::Construct(false, Dimension(470, 800));

	this->SetColor(Color(128/2, 128/2, 128/2, 0));

	/*
	if (PopupComm::__PLoadingBitmap == null) {
		PopupComm::__PLoadingBitmap = UtilApp::GetBitmap2N("common/panel_bg_message.png");
	}

	__pLabel = new Label();
	__pLabel->Construct(Rectangle((470-440)/2-8, (800-509)/2+48, 440, 509), "");
	__pLabel->SetBackgroundBitmap(*(PopupComm::__PLoadingBitmap));
	this->AddControl(*__pLabel);
	 */

}

PopupComm::~PopupComm(void)
{
	/*
	if (__pAnimation) {
		__pAnimation->Stop();
		AppLogDebug("애니메이션 종료");
	}

	if (__pLabel) {
		delete __pLabel;
		__pLabel = null;
		AppLogDebug("통신 배경 이미지 삭제");
	}
	 */

	// 이게 안 해도 되는 것인가?
}

ArrayList * PopupComm::CreateProgressAnimation()
{
	if (PopupComm::__PAnimationFrameList != null) {
		return PopupComm::__PAnimationFrameList;
	}

	String fileNamesProgressAnimation[] = {
			"orange/progressing00_big.png"
			, "orange/progressing01_big.png"
			, "orange/progressing02_big.png"
			, "orange/progressing03_big.png"
			, "orange/progressing04_big.png"
			, "orange/progressing05_big.png"
			, "orange/progressing06_big.png"
			, "orange/progressing07_big.png"
	};

	long duration = 500 / 8;

	PopupComm::__PAnimationFrameList = new ArrayList();
	PopupComm::__PAnimationFrameList->Construct();


	for (int i=0; i<8; i++) {
		Bitmap * pBitmap = UtilApp::GetBitmap2N(fileNamesProgressAnimation[i]);

		if (pBitmap == null) {
			AppLogDebug("애니메이션 이미지를 가져오기 못 했습니다. - missing filename = %S", fileNamesProgressAnimation[i].GetPointer());
			continue;
		}
		AnimationFrame* pAniFrame = new AnimationFrame(*pBitmap, duration);
		PopupComm::__PAnimationFrameList->Add(*pAniFrame);
		delete pBitmap;
	}

	return PopupComm::__PAnimationFrameList;
}


void PopupComm::ConstructAnimation(ArrayList * pArr)
{
	if (__pAnimation == null)  {
		__pAnimation = new Animation();

		__pAnimation->Construct(Rectangle(160, (800-120)/2, 120, 120), *pArr);

		__pAnimation->SetRepeatCount(80);
		__pAnimation->AddAnimationEventListener(*this);
		AddControl(*__pAnimation);

		//AppLogDebug("애니메이션 생성 - frame 수 = %d", __pAnimation->GetImageCount());
	}

	__pAnimation->Play();

	//AppLogDebug("애니메이션 시작");
}

void PopupComm::OnUserEventReceivedN(RequestId  requestId, Osp::Base::Collection::IList *  pArgs)
{
	AppLog("ProgressPopup 이벤트 (%d) 수신 !!!", requestId);
}

void PopupComm::OnAnimationStopped(const Osp::Ui::Control& source)
{
	if (__pAnimation) {
		__pAnimation->Play();
		//AppLogDebug("애니메이션 다시 시작");
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PopupCommHelper::Show()
{
	// 일단 생성
	ArrayList * pArr = PopupComm::CreateProgressAnimation();

	if (PopupCommHelper::__PPopupComm == null)  {
		PopupCommHelper::__PPopupComm = new PopupComm();
	}

	PopupCommHelper::__PPopupComm->ConstructAnimation(pArr);
	PopupCommHelper::__PPopupComm->Show();

}

void PopupCommHelper::Hide()
{
	if (PopupCommHelper::__PPopupComm != null)  {
		delete PopupCommHelper::__PPopupComm;
		PopupCommHelper::__PPopupComm = null;
	}
}

