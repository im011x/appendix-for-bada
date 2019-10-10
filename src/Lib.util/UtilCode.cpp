/*
 * UtilCode.cpp
 *
 *  Created on: 2012. 6. 21.
 *      Author: benny
 */

#include "UtilCode.h"
#include <FText.h>

UtilCode::UtilCode()
{
}

UtilCode::~UtilCode()
{
}

ByteBuffer * UtilCode::Unicode_to_EuckrN(const String & str)
{
	Osp::Text::Encoding* pEncoding = Osp::Text::Encoding::GetEncodingN(L"KSC5601");
	Osp::Base::ByteBuffer* pBuf = pEncoding->GetBytesN(str);

	return pBuf;
}

ByteBuffer * UtilCode::Euckr_to_Utf8N(const ByteBuffer * pByteEuckr)
{
	Osp::Text::Encoding* pEncKSC  = Osp::Text::Encoding::GetEncodingN("KSC5601");
	Osp::Text::Encoding* pEncUTF8 = Osp::Text::Encoding::GetEncodingN("UTF-8");

	ByteBuffer* pUTF8Buf = Osp::Text::Encoding::ConvertN(*pEncKSC, *pEncUTF8, *pByteEuckr);

	AppLogDebug("### Converting KSC5601 to UTF-8 ... current UTF8 string => size changed (%d)->(%d) and content = [%s]",
			pByteEuckr->GetLimit(), pUTF8Buf->GetLimit(), pUTF8Buf->GetPointer());

	delete pEncKSC;
	delete pEncUTF8;

	return pUTF8Buf;
}

String * UtilCode::Euckr_to_UnicodeN(const ByteBuffer * pByteEuckr)
{
	/*
	Osp::Text::Encoding* pEncKSC  = Osp::Text::Encoding::GetEncodingN("KSC5601");
	Osp::Text::Encoding* pEncUTF8 = Osp::Text::Encoding::GetEncodingN("UTF-8");

	ByteBuffer* pUTF8Buf = Osp::Text::Encoding::ConvertN(*pEncKSC, *pEncUTF8, *pByteEuckr);
	 */

	ByteBuffer* pUTF8Buf = UtilCode::Euckr_to_Utf8N(pByteEuckr);

	String * pUnicodeString = null;

	if (pUTF8Buf)  {
		pUnicodeString = new String();
		Osp::Base::Utility::StringUtil::Utf8ToString( (const char *) pUTF8Buf->GetPointer(), *pUnicodeString);
		pUnicodeString->Trim();
	}
	else {
		AppLogDebug("#### Osp::Text::Encoding::ConvertN() 결과가 NULL 입니다! 원래의 KSC코드는 [%s]", pByteEuckr->GetPointer());
	}

	delete pUTF8Buf;

	return pUnicodeString;
}

