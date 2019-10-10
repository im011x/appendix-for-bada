/*
 * BMessageBox.h
 *
 *  Created on: 2012. 2. 29.
 *      Author: SONG
 */

#ifndef BMESSAGEBOX_H_
#define BMESSAGEBOX_H_

#include <FBase.h>
#include <FUi.h>
#include <FGraphics.h>

using namespace Osp::Ui;
using namespace Osp::Ui::Controls;
using namespace Osp::Graphics;
using namespace Osp::Base;
using namespace Osp::Base::Runtime;

enum {
	BMESSAGE_ID_CONFIRM = 30
	, BMESSAGE_ID_YES = 31
	, BMESSAGE_ID_NO = 32
};

class IMessageBoxHandler
{
public:
	IMessageBoxHandler() {};
	virtual ~IMessageBoxHandler() {};

public:
	virtual 	void 	OnMessageBoxConfirm(int nID) = 0;
	virtual 	void 	OnMessageBoxYes(int nID) = 0;
	virtual 	void 	OnMessageBoxNo(int nID) = 0;
};


class BMessageBox
	: public Osp::Ui::Controls::Popup
	, public Osp::Ui::IActionEventListener
{
public:
	BMessageBox();
	virtual ~BMessageBox();

public:
	static 	Color 		__TitleColor;

protected:
	Osp::Ui::IActionEventListener * 	__pActionListener;
	IMessageBoxHandler 	* 				__pHandler;
	Dimension 							__dimSize;
	int 								__nID;

	HorizontalAlignment 			__alignMessageHorz;
	VerticalAlignment 				__alignMessageVert;

protected:
	void 	addTitle(Rectangle rect, const String & title);
	void 	addMessage(Rectangle rect, const String & message);
	void 	addButton(Rectangle rect, const String & title, const Osp::Ui::IActionEventListener* plistener, int nID);

public:
	void 	SetDelegate(IMessageBoxHandler * p)  	{  __pHandler = p;  };
	void 	SetID(int n) 							{  __nID = n; };
	void 	SetWidth(int w) 						{ if (w > 384) __dimSize.width = w; };
	void 	SetHeight(int h) 						{ if (h > 126) __dimSize.height = h; };

	void 	SetAlignHorz(HorizontalAlignment horz) 	{ __alignMessageHorz = horz;  };
	void 	SetAlignVert(VerticalAlignment vert) 	{ __alignMessageVert = vert;  };

	void 	Wait(int seconds);
	void 	Show(const String & title, const String & message);

	void 	ShowConfirm(const String & title, const String & message, const Osp::Ui::IActionEventListener* plistener=null, int confirmID = BMESSAGE_ID_CONFIRM, const String & confirmStr = "확인");
	void 	ShowYesNo(const String & title, const String & message, const Osp::Ui::IActionEventListener* plistener=null, int yesID = BMESSAGE_ID_YES, int noID = BMESSAGE_ID_NO, const String & yesStr = "예", const String & noStr = "아니오");

	void 	ShowConfirm(const String & title, const String & message, IMessageBoxHandler * pHandler, const String & confirmStr = "확인");
	void 	ShowYesNo(const String & title, const String & message, IMessageBoxHandler * pHandler, const String & yesStr = "예", const String & noStr = "아니오");

public:
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
};



#endif /* BMESSAGEBOX_H_ */
