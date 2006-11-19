// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 Lorenzo Natale
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

//added threadRelease/threadInit methods and synchronization -nat 

#ifndef _YARP2_RATETHREAD_
#define _YARP2_RATETHREAD_

#include <yarp/os/Runnable.h>

namespace yarp {
    namespace os {
        class RateThread;
        class RateThreadWrapper;
    }
}

/**
 * \ingroup key_class
 *
 * An abstraction for a periodic thread.
 */
class yarp::os::RateThread {
public:

    /**
     * Constructor.  Thread begins in a dormant state.  Call Thread::start
     * to get things going.
     */
    RateThread(int period);

    virtual ~RateThread();

    /**
     * Initialization method. This function is executed once when
     * the thread starts and before "run". Note: the function
     * is executed by the thread itself.
     */
    virtual bool threadInit();
    
    /**
     * Release method. This function is executed once when
     * the thread exits and after the last run. Note: the function
     * is executed by the thread itself.
     */
    virtual void threadRelease();

    /**
     * Loop function. This is the thread itself.
     */
    virtual void run()=0;

    /** 
     * Call this to start the thread. Blocks until initThread()
     * is executed.
     */
    bool start();
 
    /** 
     * Call this to "step" the thread rather than
     * starting it.  This will execute at most one call
     * to doLoop before returning.
     */
    bool step();
 
    /** 
     * Call this to stop the thread, this call blocks until the 
     * thread is terminated (and releaseThread() called). Actually 
     * calls join.
     */
    void stop();

    /**
     * Returns true when the thread is started, false otherwise.
     */
    bool isRunning();

    /**
     * Returns true when the thread is suspended, false otherwise.
     */
    bool isSuspended();

    /**
     * Set the (new) rate of the thread.
     * @param p the rate [ms]
     * @return true.
     */
    bool setRate(int p);

    /**
     * Return the current rate of the thread.
     * @return thread current rate [ms].
     */
    double getRate();

    /**
     * Suspend the thread, the thread keeps running by doLoop is
     * never executed.
     */
    void suspend();

    /**
     * Resume the thread if previously suspended.
     */
    void resume();

	
    /**
     * Called just before a new thread starts.
     */
    virtual void beforeStart();

    /**
     * Called just after a new thread starts (or fails to start).
     * @param success true iff the new thread started successfully
     */
    virtual void afterStart(bool success);

private:
    bool join(double seconds = -1);

    void *implementation;
};



/**
 * This class takes a Runnable instance and wraps a thread around it.
 * This class is under development - API may change a lot.
 */
class yarp::os::RateThreadWrapper : public RateThread {
private:
    Runnable *helper;
    int owned;
public:
    /**
     * Default constructor.
     */
    RateThreadWrapper(): RateThread(0) {
        helper = 0/*NULL*/;
        owned = false;
    }

    RateThreadWrapper(Runnable *helper): RateThread(0) {
        this->helper = helper;
        owned = true;
    }

    RateThreadWrapper(Runnable& helper): RateThread(0) {
        this->helper = &helper;
        owned = false;
    }
    
    virtual ~RateThreadWrapper() {
        detach();
    }

    void detach() {
        if (owned) {
            if (helper!=0/*NULL*/) {
                delete helper;
            }
        }
        helper = 0/*NULL*/;
        owned = false;
    }
  
    virtual bool attach(Runnable& helper) {
        detach();
        this->helper = &helper;
        owned = false;
        return true;
    }
    
    virtual bool attach(Runnable *helper) {
        detach();
        this->helper = helper;
        owned = true;
        return true;
    }

    bool open(double framerate = -1, bool polling = false);

	void close() {
		RateThread::stop();
    }

	void stop() {
		RateThread::stop();
	}

    virtual void run() {
        if (helper!=0/*NULL*/) {
            helper->run();
        }
    }

	virtual bool threadInit()
	{
		if (helper!=0) {
			return helper->threadInit();
		}
		else
			return true;
	}

	virtual void threadRelease()
	{
		if (helper!=0) {
			helper->threadRelease();
		}
	}

	virtual void afterStart(bool s)
	{
		if (helper!=0) {
			helper->afterStart(s);
		}
	}

	virtual void beforeStart()
	{
		if (helper!=0) {
			helper->beforeStart();
		}
	}

    Runnable *getAttachment() const {
        return helper;
    }
};

#endif

