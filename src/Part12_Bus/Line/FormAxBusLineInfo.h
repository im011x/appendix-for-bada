/*
 * FormAxBusLineInfo.h
 *
 *  Created on: 2012. 7. 12.
 *      Author: benny
 */

#ifndef FORMAXBUSLINEINFO_H_
#define FORMAXBUSLINEINFO_H_

#include "FormAxBus.h"
#include "HttpBusSeoul.h"
#include "XmlBusSeoul.h"

class FormAxBusLineInfo
	: public FormAxBus
	, public IHttpFirstResponder
{
public:
	FormAxBusLineInfo();
	virtual ~FormAxBusLineInfo();
	virtual bool AtInitialize();

// Implementation
protected:
	const static 	int 		ID_MESSAGEBOX_CONFIRM = 100;
	const static 	int 		ID_MESSAGEBOX_NO_KEYWORD = 101;
	const static 	int 		ID_MESSAGEBOX_NO_DATA = 102;
	const static 	int 		ID_MESSAGEBOX_BAD_DATA = 103;
	const static 	int 		ID_DO_1 = 201;
	const static 	int 		ID_DO_2 = 202;
	const static 	int 		ID_DO_3 = 203;

	HttpBusSeoul * 						__httpSeoulBus;
	ArrayList * 						__pArrSpecificLine;
	XmlBusSeoulParseLineInfo 			__parser;

	void 		invokeQuery();
	void 		updateContent();
	void		saveDb();

protected:
	Panel * 				__pPanelTop;
	Panel * 				__pPanelMidL;
	Panel * 				__pPanelMidR;
	Panel * 				__pPanelBtm;

	MyLabel * 				__pLabelLineName;
	MyLabel * 				__pLabelLineType;
	MyLabel * 				__pLabelInfo1;
	MyLabel * 				__pLabelInfo2;
	MyLabel * 				__pLabelInfo3;
	MyLabel * 				__pLabelInfo4;
	MyLabel * 				__pLabelInfo5;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// IHttpFirstResponder
	virtual void 	OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer);
	virtual void 	OnBadResponse(HandleHttp * pHttp, const String & message);

};

#endif /* FORMAXBUSLINEINFO_H_ */
