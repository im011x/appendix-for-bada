/*
 * BacNavigationController.h
 *
 *  Created on: 2012. 4. 20.
 *      Author: SONG
 */

#ifndef BACNAVIGATIONCONTROLLER_H_
#define BACNAVIGATIONCONTROLLER_H_

#include <FBase.h>
#include <FUi.h>
#include "BacBaseForm.h"
#include "BacFormFactoryInterface.h"

class BacNavigationController
	: public Osp::Ui::Controls::Form
{
public:
	BacNavigationController();
	virtual ~BacNavigationController();

	bool Initialize();
	result OnInitializing(void);
	result OnTerminating(void);

// Implementation
protected:
	BacBaseForm 		* 			__pFormHome;
	BacBaseForm 		* 			__pFormActive;

	Osp::Base::Collection::Stack 	__stackNavi;

	BacFormFactoryInterface 	* 			__pFactory;

protected:
	Osp::Ui::Controls::Frame * GetFrame();

	/**
	 * 특정 폼 정보를 스택에 넣습니다.
	 * @param pform - 특정 폼 인스턴스
	 */
	void 				pushNavigationControl(BacBaseForm * pform);

	/**
	 * 스택의 최상단에 있는 ,즉 가장 최근에 넣어진 폼을 꺼내어 돌려줍니다.
	 * @return - 폼 인스턴스
	 */
	BacBaseForm * 		popNavigationControl();

	BacBaseForm * 		popNavigationControlToBread(Osp::Base::String * pName);

	/**
	 * 특정 폼을 돌려줍니다. Back 인경우 스택에서 꺼내거나 특정 폼 ID 경우 해당 폼을 생성해서 돌려 줍니다.
	 * @param requestId
	 * @param pArgs
	 */
	void 				routeRequest(RequestId  requestId, Osp::Base::Collection::IList *  pArgs);


public:

	/**
	 * 스택을 비웁니다. (현재 폼은 스택에 포함되어 있지 않습니다)
	 */
	void 					FlushButNotMe();

	/**
	 * 스택을 비웁니다. FlushNavigationControl() 와 차이점은 현재 폼까지 비웁니다.
	 */

	void 					FlushEverything();

	const BacBaseForm * 	GetParentForm();

	void 					SetFactory(BacFormFactoryInterface * pFactory, RequestId id);

	void 					ActivateHome(bool bShow);
	void 					Activate(bool update);
	void 					DeActivate();

	/**
	 * 이벤트 수신 부분 입니다.
	 * @param requestId - 폼 아이디 혹은 명령 코드 (ex. Back)
	 * @param pArgs
	 */
	virtual void 	OnUserEventReceivedN(RequestId  requestId, Osp::Base::Collection::IList *  pArgs);

};

#endif /* BACNAVIGATIONCONTROLLER_H_ */
