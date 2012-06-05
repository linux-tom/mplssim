#ifndef THREADER_H
#define THREADER_H

#include <pthread.h>
class Threader
{
public:
	Threader() {/* empty */}
	virtual ~Threader() {/* empty */}

	/** Returns true if the thread was successfully started, false if there was an error starting the thread */
	bool Run()
	{
		return (pthread_create(&_thread, NULL, InternalThreadEntryFunc, this) == 0);
	}

	/** Will not return until the internal thread has exited. */
	void WaitForInternalThreadToExit()
	{
		(void) pthread_join(_thread, NULL);
	}

protected:
   /** Implement this method in your subclass with the code you want your thread to run. */
	virtual void InternalThreadEntry() = 0;
	pthread_t _thread;

private:
	static void * InternalThreadEntryFunc(void * This)
	{
		((Threader *)This)->InternalThreadEntry(); return NULL;
	}

};
#endif
