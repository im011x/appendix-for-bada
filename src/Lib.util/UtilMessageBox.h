/*
 * UtilMessageBox.h
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#ifndef UTILMESSAGEBOX_H_
#define UTILMESSAGEBOX_H_

#include "../Lib/BMessageBox.h"

class UtilMessageBox
{
public:
	static 	int 			__MessageBoxWidth;
	static 	int 			__MessageBoxHeight;
	static 	String 			__MessageBoxTitle;
	static 	Color 			__MessageBoxTitleColor;

public:
	static 	void 	Show(const String & title, const String & message);
	static 	void 	ShowAndWait(const String & title, const String & message, int seconds=2);

	static 	BMessageBox * 	ConfirmWithCallback(const String & title, const String & message, Osp::Ui::IActionEventListener* plistener=null, int confirmID = BMESSAGE_ID_CONFIRM, const String & confirmStr = "확인");
	static 	BMessageBox * 	YesNoWithCallback(const String & title, const String & message, Osp::Ui::IActionEventListener* plistener=null, int yesID = BMESSAGE_ID_YES, int noID = BMESSAGE_ID_NO, const String & yesStr = "예", const String & noStr = "아니오");

	static 	BMessageBox * 	ConfirmWithInterface(const String & title, const String & message, IMessageBoxHandler * pInterface, const int ID = 0, const String & confirmStr = "확인");
	static 	BMessageBox * 	YesNoWithInterface(const String & title, const String & message, IMessageBoxHandler * pInterface, const int ID = 0,const String & yesStr = "예", const String & noStr = "아니오");
};

#endif /* UTILMESSAGEBOX_H_ */
