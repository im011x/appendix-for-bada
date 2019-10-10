/*
 * HttpBusSeoulOnLanesByStop.h
 *
 *  Created on: 2012. 7. 8.
 *      Author: benny
 */

#ifndef HttpBusSeoulOnLanesByStop_H_
#define HttpBusSeoulOnLanesByStop_H_

#include "HandleHttp.h"
#include "DataBusSeoul.h"
#include <FXml.h>

using namespace Osp::Base::Utility;
using namespace Osp::Xml;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
/*

class BusSeoul_StopInfo : public Osp::Base::Object
{
public:
	Osp::Base::String 		__arsID;
	Osp::Base::String 		__stopID;
	Osp::Base::String 		__stopName;
	Osp::Base::String 		__stopType;

	Osp::Base::String 		__X;
	Osp::Base::String 		__Y;

};

class BusSeoul_LineInfo : public Osp::Base::Object
{
public:
	// 5.2.9. 정류소별경유노선목록조회 결과
	Osp::Base::String 		__lineID;
	Osp::Base::String 		__lineName;
	Osp::Base::String 		__lineType;
	Osp::Base::String 		__lineLenght;
	Osp::Base::String 		__linePlaceFrom;
	Osp::Base::String 		__linePlaceTo;
	Osp::Base::String 		__lineInterval;

	bool 					__isLastBus;
	Osp::Base::String 		__timeFirstBus;
	Osp::Base::String 		__timeLastBus;
	Osp::Base::String 		__timeFirstLowBus;
	Osp::Base::String 		__timeLastLowBus;
};

*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
class HttpBusSeoulAnswer : public IHttpFirstResponder
{
public:
	HttpBusSeoulAnswer() {
		__pActionHandler = null;
		__ActionID_OK = 0;
		__ActionID_NoData = 0;
		__ActionID_BadResponse = 0;
	};
	virtual ~HttpBusSeoulAnswer() {};

protected:
	Osp::Ui::IActionEventListener* 		__pActionHandler;
	int 	__ActionID_OK;
	int 	__ActionID_NoData;
	int 	__ActionID_BadResponse;

public:
	void 	SetActionHandle(Osp::Ui::IActionEventListener* p, int nOK, int nNoData, int nBad) {
		__pActionHandler = p;
		__ActionID_OK = nOK;
		__ActionID_NoData = nNoData;
		__ActionID_BadResponse = nBad;
	}
};
*/

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class HttpBusSeoulOnLanesByStop //: public HttpBusSeoulAnswer
{
public:
	HttpBusSeoulOnLanesByStop();
	virtual ~HttpBusSeoulOnLanesByStop();


public:
	virtual bool 	ParseXml(ByteBuffer* pBuffer);

	Osp::Base::String & 	GetCode()  		{  return __responseCode; };
	Osp::Base::String & 	GetMessage()	{  return __responseMessage; };
	//
	Osp::Base::Collection::ArrayList * 	GetArray()	{  return  &__arrLines;  };


protected:
	Osp::Base::String 						__responseCode;
	Osp::Base::String 						__responseMessage;
	Osp::Base::Collection::ArrayList 		__arrLines;

private:
	bool 	do_commonHeader(xmlDocPtr doc, xmlNodePtr theNode);
	bool	do_messageHeader(xmlDocPtr doc, xmlNodePtr theNode);
	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode);

};



#endif /* HttpBusSeoulOnLanesByStop_H_ */
