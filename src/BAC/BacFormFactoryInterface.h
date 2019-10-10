/*
 * BacFormIFactory.h
 *
 *  Created on: 2012. 4. 20.
 *      Author: SONG
 */

#ifndef BACFORMFACTORYINTERFACE_H_
#define BACFORMFACTORYINTERFACE_H_

#include <FBase.h>
#include <FUi.h>
#include "BacDefine.h"






class BacFormFactoryInterface {
public:
	BacFormFactoryInterface();
	virtual ~BacFormFactoryInterface();

protected:
	RequestId 	__homeID;

public:
	virtual Osp::Ui::Controls::Form * 	CreateForm(RequestId formID) = 0;

	//
	void SetHomeID(RequestId home) {  __homeID = home;  };
	RequestId GetHomeID()  { return __homeID; };
};

#endif /* BACFORMFACTORYINTERFACE_H_ */
