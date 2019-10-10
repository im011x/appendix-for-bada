/*
 * FormAxBusAboutFreq.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSABOUTMINE_H_
#define FORMAXBUSABOUTMINE_H_

#include "FormAxBus.h"
#include "PanelAxBusMyLine.h"
#include "PanelAxBusMyStop.h"
#include "PanelAxFreqUsage.h"

class FormAxBusAboutFreq
	: public FormAxBus
{
public:
	FormAxBusAboutFreq();
	virtual ~FormAxBusAboutFreq();
	virtual bool AtInitialize();
	virtual bool AtWakeup();

// Implementation
public:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;
	const static 	int 		ID_MESSAGEBOX_UPDATE = 104;

protected:
	const static 	int 		ID_TABBAR_FAVORATE_STOP = 111;
	const static 	int 		ID_TABBAR_FAVORATE_LINE = 112;
	const static 	int 		ID_TABBAR_OTHERS = 114;

	// 이 부분 다시
	const 	static 	int 		ID_CONTEXT_LINEINFO = 122;
	const 	static 	int 		ID_CONTEXT_LINESTOPS = 123;
	const 	static 	int 		ID_CONTEXT_LINEMAP = 124;

	TabBar 					* 	__pTab;
	PanelAxBusMyLine 		* 	__pMyLine;
	PanelAxBusMyStop 		* 	__pMyStop;
	PanelAxFreqUsage 		* 	__pUsage;

protected:
	void 		SetShow(int nTab);

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);
	//virtual void OnUserEventReceivedN (RequestId requestId, Osp::Base::Collection::IList *pArgs);
};

#endif /* FORMAXBUSABOUTMINE_H_ */
