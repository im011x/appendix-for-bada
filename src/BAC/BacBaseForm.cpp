/*
 * BacBaseForm.cpp
 *
 *  Created on: 2012. 4. 20.
 *      Author: SONG
 */

#include "BacBaseForm.h"
#include "BacNavigationController.h"
#include "UtilPopup.h"


Color BacBaseForm::__colorClientBackground = Color::COLOR_WHITE;


BacBaseForm::BacBaseForm() : __pNaviagationController(null), __pClientPanel(null), __pClientScrollPanel(null), __pArrTimerHandlers(null)
{
	__PushIs = PUSH;
	__pAnimation = null;
}

BacBaseForm::~BacBaseForm()
{
}

Rectangle BacBaseForm::GetClientRect() const
{
	Rectangle clientArea = GetClientAreaBounds();
	clientArea.y = 0;
	return clientArea;
}

result BacBaseForm::OnInitializing(void)
{
	if (checkLogin() == false) {
		return E_FAILURE;
	}

	Header * pHeader = GetHeader();
	if (pHeader) {
		buildHeader();
	}

	Footer* pFooter = GetFooter();
	if (pFooter) {
		buildFooter();
	}

	Rectangle clientArea = GetClientRect();
	AppLogDebug("Client area = (%d,%d) - (%d, %d)", clientArea.x, clientArea.y, clientArea.width, clientArea.height);

	switch (__bottomPanelStype) {
	case BOTTOM_PANEL_SCROLL:
	{
		__pClientScrollPanel = new ScrollPanel();
		__pClientScrollPanel->Construct(clientArea);
		this->AddControl(*__pClientScrollPanel);

		//clientArea.y = 0;

		__pClientPanel = new Panel();
		__pClientPanel->Construct(clientArea);
		__pClientScrollPanel->AddControl(*__pClientPanel);
	}
		break;
	case BOTTOM_PANEL:
	{
		__pClientPanel = new Panel();
		__pClientPanel->Construct(clientArea);
		this->AddControl(*__pClientPanel);
	}
		break;
	default:
		break;
	}

#ifdef _DEBUG
	if (__pClientScrollPanel)
		__pClientScrollPanel->SetBackgroundColor(Color::COLOR_GREY);
#endif

	if (__pClientPanel)
		__pClientPanel->SetBackgroundColor(__colorClientBackground);

	return E_SUCCESS;
}

result BacBaseForm::OnTerminating(void)
{
	DeleteTimers();

	return E_SUCCESS;
}

void BacBaseForm::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	AppLogDebug("Navigation Controller processes the action ID = 0x%x", actionId);

	if (__pNaviagationController) {
		__pNaviagationController->SendUserEvent(actionId, null);
	}
}

bool BacBaseForm::AtSleep()
{
	return true;
}

bool BacBaseForm::AtWakeup()
{
	return true;
}

void BacBaseForm::AtTimer(HandleTimer * pTimerHandler)
{
	AppLogDebug("이곳에 호출되어서는 안 됩니다. 각 폼에서 이 virtual 함수를 override 하여 사용하세요~");
}

void BacBaseForm::AtLocation()
{

}

Panel * BacBaseForm::GetBottomPanel()
{
	if (__pClientScrollPanel)
		return __pClientScrollPanel;

	return __pClientPanel;
}

void BacBaseForm::GotoForm(RequestId formID)
{
	if (__pNaviagationController) {
		__pNaviagationController->SendUserEvent(formID, null);
	}
}

void BacBaseForm::GotoBack()
{
	if (__pNaviagationController) {
		__pNaviagationController->SendUserEvent(REQUEST_GOTO_BACK, null);
	}
}

void BacBaseForm::GotoBread(const String & name)
{
	if (__pNaviagationController) {
		String * pArg = new String(name);
		ArrayList * pArgs = new ArrayList();
		pArgs->Construct(1);
		pArgs->Add(*pArg);

		__pNaviagationController->SendUserEvent(REQUEST_GOTO_BREAD, pArgs);
	}
}

void BacBaseForm::GotoHome(bool bShow)
{
	if (__pNaviagationController) {
		__pNaviagationController->ActivateHome(bShow);
	}
}

void BacBaseForm::Flush(bool includeMe)
{
	if (__pNaviagationController == null) {
		return;
	}

	if (includeMe)
		__pNaviagationController->FlushEverything();
	else
		__pNaviagationController->FlushButNotMe();

}

void BacBaseForm::SetTitleAsParent()
{
	if (__pNaviagationController) {
		__Title = __pNaviagationController->GetParentForm()->__Title;
	}
}

HandleTimer * BacBaseForm::CreateTimer(int seconds, int miliseconds, bool forever)
{
	if (__pArrTimerHandlers == null) {
		__pArrTimerHandlers = new ArrayList();
		__pArrTimerHandlers->Construct();
	}

	HandleTimer * pTimeHandler = new HandleTimer();
	pTimeHandler->Create(this);
	pTimeHandler->Start(1000 * seconds + miliseconds);
	pTimeHandler->SetForever(forever);

	__pArrTimerHandlers->Add(*pTimeHandler);

	return pTimeHandler;
}

void BacBaseForm::DeleteTimer(HandleTimer * p)
{
	if (__pArrTimerHandlers == null) {
		return;
	}

	for (int i=0; i<__pArrTimerHandlers->GetCount(); i++) {
		HandleTimer * pHandler = static_cast<HandleTimer*> (__pArrTimerHandlers->GetAt(i));

		if (pHandler == p) {
			__pArrTimerHandlers->RemoveAt(i, false);
			delete pHandler;
			break;
		}
	}
}

void BacBaseForm::DeleteTimers()
{
	if (__pArrTimerHandlers) {
		__pArrTimerHandlers->RemoveAll(true);
		delete __pArrTimerHandlers;
		__pArrTimerHandlers = null;
	}
}


void BacBaseForm::transitAnimationStart()
{
	ArrayList * pArr = PopupComm::CreateProgressAnimation();

	if (__pAnimation == null) {
		__pAnimation = new Animation();
		__pAnimation->Construct(Rectangle(180, 250, 120, 120), *pArr);
		__pAnimation->SetRepeatCount(8*5);
		__pAnimation->AddAnimationEventListener(*this);
		AddControl(*__pAnimation);
	}

	__pAnimation->Play();

	RequestRedraw(true);
}

void BacBaseForm::transitAnimationStop()
{
	if (__pAnimation) {
		RemoveControl(*__pAnimation);
		RequestRedraw(true);
		__pAnimation = null;
	}
}

void BacBaseForm::OnAnimationStopped(const Osp::Ui::Control& source)
{
	if (__pAnimation) {
		__pAnimation->Play();
	}
}
