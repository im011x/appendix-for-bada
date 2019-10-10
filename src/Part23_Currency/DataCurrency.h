/*
 * DataCurrency.h
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#ifndef DATACURRENCY_H_
#define DATACURRENCY_H_

#include <FBase.h>

class DataCurrency : public Osp::Base::Object
{
public:
	Osp::Base::String 		__currencyName;
	Osp::Base::String 		__currencyRate[7];
};

#endif /* DATACURRENCY_H_ */
