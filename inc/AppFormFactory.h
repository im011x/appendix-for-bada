/*
 * AppFormFactory.h
 *
 *  Created on: 2012. 6. 13.
 *      Author: benny
 */

#ifndef APPFORMFACTORY_H_
#define APPFORMFACTORY_H_

#include "BacFormFactoryInterface.h"
#include "AppID.h"

class AppFormFactory: public BacFormFactoryInterface {
public:
	AppFormFactory();
	virtual ~AppFormFactory();

public:
	virtual Osp::Ui::Controls::Form * 	CreateForm(RequestId formID);
};

#endif /* APPFORMFACTORY_H_ */
