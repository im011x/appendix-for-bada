/*
 * UtilMessageBox.cpp
 *
 *  Created on: 2012. 7. 3.
 *      Author: benny
 */

#include "UtilMessageBox.h"

int 		UtilMessageBox::__MessageBoxWidth = 300;
int 		UtilMessageBox::__MessageBoxHeight = 400;
//String 		UtilMessageBox::__MessageBoxTitle;
//Color 		UtilMessageBox::__MessageBoxTitleColor = Color::COLOR_BLUE;
String 		UtilMessageBox::__MessageBoxTitle = "부록 알림창";
Color 		UtilMessageBox::__MessageBoxTitleColor = Color(0,87,168);

void UtilMessageBox::Show(const String & title, const String & content)
{
	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	Osp::Ui::Controls::MessageBox myMSGBox;
	myMSGBox.Construct((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), content, MSGBOX_STYLE_OK, 0);
	int modalResult = 0;
	myMSGBox.ShowAndWait(modalResult);  //ShowAndWait() draw, show itself and process events
}

void UtilMessageBox::ShowAndWait(const String & title, const String & message, int seconds)
{
	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	BMessageBox * pMessageBox = new BMessageBox();
	pMessageBox->SetWidth(__MessageBoxWidth);
	pMessageBox->SetHeight(__MessageBoxHeight);
	pMessageBox->Show((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), message);
	pMessageBox->Wait(seconds);
}

BMessageBox * UtilMessageBox::ConfirmWithCallback(const String & title, const String & message, Osp::Ui::IActionEventListener* plistener, int confirmID, const String & confirmStr)
{
	//AppLogDebug("@@@@ IActionEventListener address = 0x%x", plistener);

	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	BMessageBox * pMessageBox = new BMessageBox();
	pMessageBox->SetWidth(__MessageBoxWidth);
	pMessageBox->SetHeight(__MessageBoxHeight);
	pMessageBox->ShowConfirm((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), message, plistener, confirmID, confirmStr);

	return pMessageBox;
}

BMessageBox * UtilMessageBox::YesNoWithCallback(const String & title, const String & message, Osp::Ui::IActionEventListener* plistener, int yesID, int noID, const String & yesStr, const String & noStr)
{
	//AppLogDebug("@@@@ IActionEventListener address = 0x%x", plistener);

	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	BMessageBox * pMessageBox = new BMessageBox();
	pMessageBox->SetWidth(__MessageBoxWidth);
	pMessageBox->SetHeight(__MessageBoxHeight);
	pMessageBox->ShowYesNo((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), message, plistener, yesID, noID, yesStr, noStr);

	return pMessageBox;
}

BMessageBox * UtilMessageBox::ConfirmWithInterface(const String & title, const String & message, IMessageBoxHandler * pInterface, const int ID,const String & confirmStr)
{
	//AppLogDebug("@@@@ IMessageBoxHandler address = 0x%x", pInterface);

	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	BMessageBox * pMessageBox = new BMessageBox();
	pMessageBox->SetID(ID);
	pMessageBox->SetWidth(__MessageBoxWidth);
	pMessageBox->SetHeight(__MessageBoxHeight);
	pMessageBox->ShowConfirm((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), message, pInterface, confirmStr);

	return pMessageBox;
}

BMessageBox * UtilMessageBox::YesNoWithInterface(const String & title, const String & message, IMessageBoxHandler * pInterface, const int ID, const String & yesStr, const String & noStr)
{
	//AppLogDebug("@@@@ IMessageBoxHandler address = 0x%x", pInterface);

	BMessageBox::__TitleColor = UtilMessageBox::__MessageBoxTitleColor;

	BMessageBox * pMessageBox = new BMessageBox();
	pMessageBox->SetID(ID);
	pMessageBox->SetWidth(__MessageBoxWidth);
	pMessageBox->SetHeight(__MessageBoxHeight);
	pMessageBox->ShowYesNo((UtilMessageBox::__MessageBoxTitle.IsEmpty() ? title : UtilMessageBox::__MessageBoxTitle), message, pInterface, yesStr, noStr);

	return pMessageBox;
}
