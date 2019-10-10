/*
 * HandleThread.cpp
 *
 *  Created on: 2012. 8. 10.
 *      Author: benny
 */

#include "HandleThread.h"

HandleThread::HandleThread() : __pListThreads(null)
{
	__pListThreads = new LinkedList();
}

HandleThread::~HandleThread()
{
	Kill();

	delete __pListThreads;
	__pListThreads = null;
}

bool HandleThread::Fork(Thread* pInvoking)
{
	__pListThreads->Add(*pInvoking);

	pInvoking->Start();

	AppLogDebug("@@@ start a thread - current thread count = %d", __pListThreads->GetCount());

	return true;
}

bool HandleThread::Kill(Thread* pDeleting)
{
	IEnumerator * penum = __pListThreads->GetEnumeratorN();
	while (penum->MoveNext() == E_SUCCESS) {
		Thread * pthread = static_cast<Thread*> (penum->GetCurrent());
		if (pthread == pDeleting)  {
			AppLogDebug("@@@ stop a thread - current thread count = %d", __pListThreads->GetCount());
			pthread->Stop();
			AppLogDebug("@@@ remove a thread !!!");
			//__pListThreads->Remove(*pthread, true);
			__pListThreads->Remove(*pthread, false);
			AppLogDebug("@@@ success to kill a thread - current thread count = %d", __pListThreads->GetCount());
			return true;
		}
	}

	AppLogDebug("@@@ fail to kill a thread !!!");
	return false;
}

void HandleThread::Kill()
{
	if (__pListThreads == null) {
		return;
	}

	AppLogDebug("@@@ stop all threads to kill (count = %d)", __pListThreads->GetCount());

	// 일단 모두 정지 !
	IEnumerator * penum = __pListThreads->GetEnumeratorN();
	while (penum->MoveNext() == E_SUCCESS) {
		Thread * pthread = static_cast<Thread*> (penum->GetCurrent());
		if (pthread)
			pthread->Stop();
	}

	AppLogDebug("@@@ remove all threads (count = %d)", __pListThreads->GetCount());

	//__pListThreads->RemoveAll(true);
	__pListThreads->RemoveAll(false);

	AppLogDebug("@@@ success to kill all threads !!!");
}

