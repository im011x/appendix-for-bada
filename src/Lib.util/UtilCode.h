/*
 * UtilCode.h
 *
 *  Created on: 2012. 6. 21.
 *      Author: benny
 */

#ifndef UTILCODE_H_
#define UTILCODE_H_

#include <FBase.h>

using namespace Osp::Base;

class UtilCode {
public:
	UtilCode();
	virtual ~UtilCode();

public:
	static ByteBuffer * 	Unicode_to_EuckrN(const String & str);
	static ByteBuffer * 	Euckr_to_Utf8N(const ByteBuffer * ptr);
	static String * 		Euckr_to_UnicodeN(const ByteBuffer * ptr);

};

#endif /* UTILCODE_H_ */
