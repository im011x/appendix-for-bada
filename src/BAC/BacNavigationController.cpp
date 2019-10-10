/*
 * BacNavigationController.cpp
 *
 *  Created on: 2012. 4. 20.
 *      Author: SONG
 */

#include <FApp.h>
#include "BacNavigationController.h"

using namespace Osp::Base;
using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;


BacNavigationController::BacNavigationController() : __pFormHome(null), __pFormActive(null), __pFactory(null)
{
}

BacNavigationController::~BacNavigationController()
{
}

bool BacNavigationController::Initialize()
{
	Form::Construct(0);
	return true;
}

result BacNavigationController::OnInitializing(void)
{
	__stackNavi.Construct();

	return E_SUCCESS;
}

result BacNavigationController::OnTerminating(void)
{
	//__stackNavi.RemoveAll(true);
	// 시스템이 자동으로 지운다. 따라서 난 안 해도 되나 보다...

	return E_SUCCESS;
}

Frame * BacNavigationController::GetFrame()
{
	return Osp::App::Application::GetInstance()->GetAppFrame()->GetFrame();
}

void BacNavigationController::pushNavigationControl(BacBaseForm * pform)
{
	if (pform && pform != __pFormHome) {
		if (pform->GetPush() == NO_PUSH) {
			AppLogDebug("push 원하지 않습니다. (form ID = 0x%x)", pform->GetFormID());
			GetFrame()->RemoveControl(*pform);
		}
		else {
			pform->AtSleep();
			__stackNavi.Push(*pform);
		}
	}
}

BacBaseForm * BacNavigationController::popNavigationControl()
{
	BacBaseForm * pop = static_cast <BacBaseForm*> (__stackNavi.Pop());
	return pop;
}

BacBaseForm * 	BacNavigationController::popNavigationControlToBread(Osp::Base::String * pName)
{
	BacBaseForm * pop = null;
	Frame * pFrame = GetFrame();

	AppLogDebug("before goto bread (%S), 현재 navigation stack count = %d", pName->GetPointer(), __stackNavi.GetCount());
	while (1) {
		pop = static_cast <BacBaseForm*> (__stackNavi.Pop());
		if (pop == null) {
			break;
		}
		if (pName && !pop->GetBreadName().IsEmpty() && pName->CompareTo(pop->GetBreadName()) == 0) {
			AppLogDebug("### bread (%S) 발견", pName->GetPointer());
			break;
		}
		pFrame->RemoveControl(*pop);
	}

	AppLogDebug("after goto bread (%S), 현재 navigation stack count = %d", pName->GetPointer(), __stackNavi.GetCount());

	return pop;
}

void BacNavigationController::FlushButNotMe()
{
	Frame * pFrame = GetFrame();
	if (!pFrame) {
		return;
	}

	int many = __stackNavi.GetCount();

	for (int i=0; i<many; i++) {
		Form * pop = static_cast <Form*> (__stackNavi.Pop());
		if (pop) {
			pFrame->RemoveControl(*pop);
		}
	}

	__stackNavi.RemoveAll(true);
}

void BacNavigationController::FlushEverything()
{
	if (__pFormActive) {
		pushNavigationControl(__pFormActive);
		__pFormActive = null;
	}

	FlushButNotMe();
}

const BacBaseForm * 	BacNavigationController::GetParentForm()
{
	const BacBaseForm * pop = static_cast <const BacBaseForm*> (__stackNavi.Peek());

	if (pop == null) {
		return static_cast <const BacBaseForm*> (__pFormHome);
	}

	return pop;
}

void BacNavigationController::SetFactory(BacFormFactoryInterface * pFactory, RequestId id)
{
	__pFactory = pFactory;

	if (__pFactory) {
		__pFactory->SetHomeID(id);
	}
}

void BacNavigationController::ActivateHome(bool bShow)
{
	FlushEverything();
	routeRequest(__pFactory->GetHomeID(), null);

	//
	if (__pFormActive && bShow == false) {
		__pFormActive->SetShowState(false);
	}
}

void BacNavigationController::Activate(bool update)
{
	AppLogDebug("현재 탭의 폼을 다시 활성화 시킵니다. (%s)", (update ? "보이게" : "안 보이게"));

	if (__pFormActive == null) {
		AppAssert(__pFactory);
		AppLogDebug("아직 Home 화면이 생성되지 않아 신규 생성합니다.");
		this->SendUserEvent(__pFactory->GetHomeID(), null);
	}
	else {
		if (update) {
			// 단순 다시 그리기 해서는 문제 발생 해서 아래와 같은 작업을 해 준다.
			__pFormActive->SetShowState(true);

			__pFormActive->AtWakeup();
			GetFrame()->SetCurrentForm(*__pFormActive);
			__pFormActive->RequestRedraw(true);

			//AppLogDebug("보이게 다시 그립니다.");
		}
		else {
			__pFormActive->SetShowState(false);

			//AppLogDebug("현재 폼을 안 보이게 합니다.");
		}
	}
}

void BacNavigationController::DeActivate()
{
	if (__pFormActive) {
		__pFormActive->SetShowState(false);
	}
}

void BacNavigationController::OnUserEventReceivedN(RequestId  requestId, Osp::Base::Collection::IList *  pArgs)
{
	AppLogDebug("request ID = 0x%08x", requestId);

	if (__pFactory == null) {
		AppLogDebug("@@@ __pFactory == null");
	}
	AppAssert(__pFactory);

	/*
	if (requestId == REQUEST_FORM_EXIT) {
		AppLogDebug("사용자에게 종료 확인 후 종료한다.");
		BaseUtil::MessageBoxYesNoWithInterface("확인", "프로그램을 종료합니다", this);
		return;
	}
	 */

	//
	routeRequest(requestId, pArgs);
}

void BacNavigationController::routeRequest(RequestId  requestId, Osp::Base::Collection::IList *  pArgs)
{
	BacBaseForm * pFormNow = null;
	Frame * pFrame = GetFrame();
	bool bCreatedNow = false;

	if (requestId == REQUEST_GOTO_BACK || requestId == REQUEST_GOTO_BREAD)  {  // Backward moving

		if (requestId == REQUEST_GOTO_BACK) {  // 이전에 생성된 폼으로 돌아 갑니다.
			pFormNow = popNavigationControl();
			if (pFormNow)
				AppLogDebug("pop 결과 form = (0x%x)", pFormNow->GetFormID());
			else
				AppLogDebug("pop 결과 form = null");
		}
		else if (requestId == REQUEST_GOTO_BREAD) {  // 이전에 떨어트린 빵조각을 찾아 갑니다.
			if (pArgs) {
				AppAssert(pArgs->GetCount() > 0);
				String * pName = static_cast<String*> (pArgs->GetAt(0));
				pFormNow = popNavigationControlToBread(pName);

				delete pName;
				delete pArgs;
			}
		}

		// pop 결과가 null 경우 Home 으로 돌아가도록 설정한다.
		//if (pFormNow == null) {
		//	AppLogDebug("현재 pop 결과가 null 이라서 Home 으로 현재 폼을 대신한다...");
		//	pFormNow = __pFormHome;
		//}

		// 지금 현재 active form 을 지운다.
		if (__pFormActive) {
			AppLogDebug("현재 active form (0x%x) 지웁니다.", __pFormActive->GetFormID());
			pFrame->RemoveControl(*__pFormActive);
			__pFormActive = null;
		}
		//
	}
	else if (requestId == __pFactory->GetHomeID()) {  // Home
		if (__pFormHome == null) {
			__pFormHome =  static_cast<BacBaseForm *> (__pFactory->CreateForm(requestId));
			__pFormHome->SetHome();
			bCreatedNow = true;
		}

		pFormNow = static_cast<BacBaseForm*> (__pFormHome);
	}
	else {  // others
		pFormNow = static_cast<BacBaseForm*> (__pFactory->CreateForm(requestId));
		bCreatedNow = true;
	}

	if (pFormNow == null) {
		AppLogDebug("NO form created for ID : 0x%08x", requestId);
		return;
	}

	if (bCreatedNow) {
		pFormNow->SetFormID(requestId);
		pFormNow->SetNaviagtionController(this);
		pFormNow->AtInitialize();
		pFrame->AddControl(*pFormNow);

		// 다음 폼을 생성하니까, 지금 현재 active form 을 저장해 놓는다.
		pushNavigationControl(__pFormActive);
	}
	else {
		pFormNow->SetShowState(true);
		//pFormNow->AtWakeup();  // wakeup 은 Application::OnForeground() 이벤트를 이용하도록 한다.~
	}

	pFrame->SetCurrentForm(*pFormNow);
	pFormNow->RequestRedraw(true);

	__pFormActive = pFormNow;

}
