/*
 * XmlBusSeoul.h
 *
 *  Created on: 2012. 7. 11.
 *      Author: benny
 */

#ifndef XMLBUSSEOUL_H_
#define XMLBUSSEOUL_H_

#include <FXml.h>

using namespace Osp::Base;
using namespace Osp::Xml;
using namespace Osp::Base::Collection;

class XmlBusSeoulParser {
public:
	XmlBusSeoulParser() {};
	virtual ~XmlBusSeoulParser() {};

public:
	bool 	ParseXml(ByteBuffer* pBuffer, ArrayList * pTo, bool bUpdate = false);

	Osp::Base::String & 	GetCode()  		{  return __responseCode; };
	Osp::Base::String & 	GetMessage()	{  return __responseMessage; };

protected:
	Osp::Base::String 			__responseCode;
	Osp::Base::String 			__responseMessage;

private:
	virtual	bool 	do_commonHeader(xmlDocPtr doc, xmlNodePtr theNode);
	virtual	bool	do_messageHeader(xmlDocPtr doc, xmlNodePtr theNode);
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 노선 일반 정보 (특정 정류소)
 */

class XmlBusSeoulParseLinesByStop : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseLinesByStop() {};
	virtual ~XmlBusSeoulParseLinesByStop() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 *
 */

class XmlBusSeoulParseTimeByLineByStop : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseTimeByLineByStop() {};
	virtual ~XmlBusSeoulParseTimeByLineByStop() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 노선 도착  정보 (특정 정류소)
 */

class XmlBusSeoulParseArrivesByStop : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseArrivesByStop() {};
	virtual ~XmlBusSeoulParseArrivesByStop() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 노선 도착  정보
 */

class XmlBusSeoulParseArrivesByLine : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseArrivesByLine() {};
	virtual ~XmlBusSeoulParseArrivesByLine() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * 노선  정보 () -> 전체 노선 정보
 */

class XmlBusSeoulParseLines : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseLines() {};
	virtual ~XmlBusSeoulParseLines() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 노선  정보 (특정노선)
 */

class XmlBusSeoulParseLineInfo : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseLineInfo() {};
	virtual ~XmlBusSeoulParseLineInfo() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 노선 위치 정보
 */

class XmlBusSeoulParsePositionInfo : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParsePositionInfo() {};
	virtual ~XmlBusSeoulParsePositionInfo() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};


/*
 * 정류소  정보 (특정노선)
 */

class XmlBusSeoulParseLineStops : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseLineStops() {};
	virtual ~XmlBusSeoulParseLineStops() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 정류소  정보 (특정이름)
 */

class XmlBusSeoulParseStopName : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseStopName() {};
	virtual ~XmlBusSeoulParseStopName() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

/*
 * 정류소  정보 (주변)
 */

class XmlBusSeoulParseNearStops : public XmlBusSeoulParser
{
public:
	XmlBusSeoulParseNearStops() {};
	virtual ~XmlBusSeoulParseNearStops() {};

private:
	virtual	bool	do_messageBody(xmlDocPtr doc, xmlNodePtr theNode, ArrayList * pTo, bool bUpdate);
};

#endif /* XMLBUSSEOUL_H_ */
