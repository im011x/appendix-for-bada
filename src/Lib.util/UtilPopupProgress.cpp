/*
 * UtilPopupProgress.cpp
 *
 *  Created on: 2012. 8. 6.
 *      Author: benny
 */

#include "UtilPopupProgress.h"
#include "UtilApp.h"

using namespace Osp::Base;
using namespace Osp::Base::Collection;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

Osp::Base::Collection::ArrayList 	* 		PopupProgress::__PAnimationFrameList = null;
Osp::Graphics::Bitmap				* 		PopupProgress::__PLoadingBitmap = null;

PopupProgress * 			PopupProgressHelper::__PPopupProgress = null;

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

PopupProgress::PopupProgress(bool deleteSelf) : __selfDestroy(deleteSelf), __pAnimation(null)
{
	Osp::Ui::Controls::Popup::Construct(false, Dimension(490, 800));

	this->SetColor(Color(128/2, 128/2, 128/2, 128));

	if (PopupProgress::__PLoadingBitmap == null) {
		PopupProgress::__PLoadingBitmap = UtilApp::GetBitmap2N("common/panel_bg_message.png");
	}

	__pBGPanel = new Panel();
	__pBGPanel->Construct(Rectangle(10, 200, 450, 400));
	this->AddControl(*__pBGPanel);

	Label * pBG = new Label();
	pBG->Construct(Rectangle(0, 0, 450, 400), "");
	pBG->SetBackgroundBitmap(*(PopupProgress::__PLoadingBitmap));
	__pBGPanel->AddControl(*pBG);
}

PopupProgress::~PopupProgress(void)
{
	if (__pAnimation) {
		__pAnimation->Stop();
	}

	this->RemoveAllControls();

	PopupProgressHelper::__PPopupProgress = null;
}

ArrayList * PopupProgress::CreateProgressAnimation()
{
	if (PopupProgress::__PAnimationFrameList != null) {
		return PopupProgress::__PAnimationFrameList;
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

	long duration = 500 / 8 * 2;

	PopupProgress::__PAnimationFrameList = new ArrayList();
	PopupProgress::__PAnimationFrameList->Construct();

	for (int i=0; i<8; i++) {
		Bitmap * pBitmap = UtilApp::GetBitmap2N(fileNamesProgressAnimation[i]);

		if (pBitmap == null) {
			AppLogDebug("애니메이션 이미지를 가져오기 못 했습니다. - missing filename = %S", fileNamesProgressAnimation[i].GetPointer());
			continue;
		}
		AnimationFrame* pAniFrame = new AnimationFrame(*pBitmap, duration);
		PopupProgress::__PAnimationFrameList->Add(*pAniFrame);
		delete pBitmap;
	}

	return PopupProgress::__PAnimationFrameList;
}


void PopupProgress::ConstructAnimation(ArrayList * pArr)
{
	if (__pAnimation == null)  {
		__pAnimation = new Animation();
		__pAnimation->Construct(Rectangle(160, (400-120)/2, 120, 120), *pArr);
		__pAnimation->SetRepeatCount(80);
		__pAnimation->AddAnimationEventListener(*this);
		__pBGPanel->AddControl(*__pAnimation);
	}

	__pAnimation->Play();
}

void PopupProgress::Show(const String & message, IPopupProgressUserAction* plistener, int confirmID, const String & confirmStr)
{
	__pLabelMessage = new Label();
	__pLabelMessage->Construct(Rectangle(10, 10, 450, 150), message);
	//__pLabelMessage->SetTextColor(Color::COLOR_RED);
	__pLabelMessage->SetTextColor(Color::COLOR_BLACK);
	__pLabelMessage->SetTextConfig(30, LABEL_TEXT_STYLE_NORMAL);
	__pBGPanel->AddControl(*__pLabelMessage);

	Button * pButton = new Button();
	pButton->Construct(Rectangle(20, 300, 400, 60), confirmStr);
	__pBGPanel->AddControl(*pButton);

	pButton->SetActionId(confirmID);
	pButton->AddActionEventListener(*this);

	__pUserAgent = plistener;

	Osp::Ui::Controls::Popup::Show();
}

void PopupProgress::SetMessage(const String & message)
{
	__pLabelMessage->SetText(message);
	__pLabelMessage->Draw();
	__pLabelMessage->Show();
}


void PopupProgress::OnAnimationStopped(const Osp::Ui::Control& source)
{
	if (__pAnimation) {
		__pAnimation->Play();
	}
}

void PopupProgress::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	if (__pUserAgent) {
		__pUserAgent->OnPopupProgressUserAction(actionId);
	}

	if (__selfDestroy)
		delete this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PopupProgressHelper::Show(const String & message, IPopupProgressUserAction* plistener, int confirmID, const String & confirmStr, bool deleteSelf)
{
	// 일단 생성
	ArrayList * pArr = PopupProgress::CreateProgressAnimation();

	PopupProgressHelper::__PPopupProgress = new PopupProgress(deleteSelf);
	PopupProgressHelper::__PPopupProgress->ConstructAnimation(pArr);
	PopupProgressHelper::__PPopupProgress->Show(message, plistener, confirmID, confirmStr);
}

void PopupProgressHelper::Show(const String & message)
{
	if (PopupProgressHelper::__PPopupProgress != null)  {
		PopupProgressHelper::__PPopupProgress->SetMessage(message);
	}
}

void PopupProgressHelper::Hide()
{
	if (PopupProgressHelper::__PPopupProgress != null)  {
		delete PopupProgressHelper::__PPopupProgress;
		//PopupProgressHelper::__PPopupProgress = null;
	}
}


