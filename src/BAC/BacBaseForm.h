/*
 * BacBaseForm.h
 *
 *  Created on: 2012. 4. 20.
 *      Author: SONG
 */

#ifndef BACBASEFORM_H_
#define BACBASEFORM_H_

#include <FBase.h>
#include <FUi.h>
#include "Intent.h"
#include "UtilMessageBox.h"
#include "HandleTimer.h"

typedef enum {
	BOTTOM_PANEL_SCROLL,
	BOTTOM_PANEL
} TYPE_BOTTOM_PANEL;

typedef enum {
	NOT_NECESSARY_LOGIN,
	NECESSARY_LOGIN
} TYPE_LOGIN_AUTHORITY;

typedef enum {
	NO_PUSH,
	PUSH
} TYPE_PUSH_FORM;


class BacNavigationController;

using namespace Osp::Base;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;

class BacBaseForm
	: public Osp::Ui::Controls::Form
	, public Intent
	, public Osp::Ui::IActionEventListener
	, public ITimerDelegate
	, public Osp::Ui::IAnimationEventListener
{
public:
	BacBaseForm();
	virtual ~BacBaseForm();

public:
	result OnInitializing(void);
	result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	virtual bool AtInitialize() = 0;
	virtual bool AtSleep();
	virtual bool AtWakeup();
	virtual void AtTimer(HandleTimer * pTimerHandler);
	virtual void AtLocation();

protected:
	virtual 	bool checkLogin()	{ return true;  };
	virtual 	void buildHeader()	{};
	virtual 	void buildFooter()	{};
	virtual 	Rectangle GetClientRect() const;

protected:
	bool 									__bHome;
	String 									__Title;
	TYPE_PUSH_FORM 							__PushIs;
	RequestId 								__IdFormSelf;
	String 									__NameBread;
	BacNavigationController * 				__pNaviagationController;

	TYPE_BOTTOM_PANEL 						__bottomPanelStype;
	Panel * 								__pClientPanel;
	ScrollPanel* 							__pClientScrollPanel;

	//
	static 	Color 							__colorClientBackground;

	//
	ArrayList 				* 				__pArrTimerHandlers;


public:
	void 							SetHome(bool value = true)  			{ __bHome = value; };
	bool 							IsHome() 								{ return __bHome;  };

	void 							SetPush(TYPE_PUSH_FORM push) 			{ __PushIs = push; };
	TYPE_PUSH_FORM  				GetPush() 								{ return __PushIs; };

	void 							SetFormID(RequestId id) 				{ __IdFormSelf = id; };
	RequestId 						GetFormID() 							{ return __IdFormSelf; };

	void 							SetBreadName(const String & name) 		{ __NameBread = name; };
	const String & 					GetBreadName() 							{ return __NameBread; };

	void SetNaviagtionController(BacNavigationController * pf) 				{ __pNaviagationController = pf; };
	BacNavigationController * 	GetNaviagtionController()  					{ return __pNaviagationController; };

	void 							SetPanelStype(TYPE_BOTTOM_PANEL type)  	{ __bottomPanelStype = type; };

	Panel * 						GetPanel() 								{  return __pClientPanel; };
	Panel * 						GetScrollPanel() 						{  return __pClientScrollPanel; };
	Panel * 						GetBottomPanel();


	static 	void SetDefaultBackgroundColor(const Color & color)  			{ __colorClientBackground = color;  };


	//
	void GotoForm(RequestId formID);

	/**
	 * 현재 폼 이전에 보여졌던 폼으로 이동합니다.
	 */
	void 	GotoBack();

	/**
	 *
	 */
	void 	GotoBread(const String & name);

	/**
	 *
	 */
	void 	GotoHome(bool bShow = true);

	/**
	 *
	 */
	void 	Flush(bool includeMe = true);

	//
	void 	SetTitleAsParent();

	//
	/**
	 * 타이머 사용을 간편하게 하기 위한 함수로서 타이머 생성 함수입니다.
	 * @param seconds - 초
	 * @param miliseconds - 1/1000 초
	 * @param forever - 타이머를 한번만 받을 것인지 계속 받을 것인지 지정
	 * @return - 타이머 핸들
	 */
	HandleTimer * 				CreateTimer(int seconds, int miliseconds=0, bool forever=true);
	void 						DeleteTimer(HandleTimer * p);
	void 						DeleteTimers();

	//
protected:
	Animation	* 						__pAnimation;

	void 	transitAnimationStart();
	void 	transitAnimationStop();

	//
public:
	virtual void OnAnimationStopped(const Osp::Ui::Control& source);

};

#endif /* BACBASEFORM_H_ */
