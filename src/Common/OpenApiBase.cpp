/*
 * OpenApiBase.cpp
 *
 *  Created on: 2012. 7. 8.
 *      Author: benny
 */

#include "OpenApiBase.h"

Osp::Base::String 		OpenApiBase::__ServiceKey = "gU4Pjlx4%2F6Pg2hAMxvHBx%2BS%2FHiDE7Fcv%2B8nC24x9OFbdLWsoYjeAfyqk7skeI9CmFygXYJx%2Frog4kv8Fz%2BEZSg%3D%3D";
Osp::Base::String 		OpenApiBase::__ServiceKeyDaum = "122cd66194f94151318f4ed96f66252476fba198";

OpenApiBase::OpenApiBase(IHttpFirstResponder * plistener) : __pListener(plistener)
{
	if (__pHttpHandler == null) {
		__pHttpHandler = new HandleHttp();
		__pHttpHandler->SetResponder(__pListener);
	}
}

OpenApiBase::~OpenApiBase()
{
	if (__pHttpHandler) {
		__pHttpHandler->Cancel();

		delete __pHttpHandler;
		__pHttpHandler = null;
	}
}

