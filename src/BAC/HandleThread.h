/*
 * HandleThread.h
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#ifndef HANDLETHREAD_H_
#define HANDLETHREAD_H_

#include <FBase.h>

using namespace Osp::Base;
using namespace Osp::Base::Runtime;
using namespace Osp::Base::Collection;

class HandleThread {
public:
	HandleThread();
	virtual ~HandleThread();

private:
	LinkedList 	* 		__pListThreads;

public:
	bool 		Fork(Thread* p);
	bool 		Kill(Thread* p);
	void 		Kill();
};

#endif /* HANDLETHREAD_H_ */
