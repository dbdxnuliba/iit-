/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_OS_SHAREDLIBRARYCLASS_H
#define YARP_OS_SHAREDLIBRARYCLASS_H

#include <yarp/os/SharedLibraryClassFactory.h>
#include <yarp/os/Network.h>

namespace yarp {
    namespace os {
        template <class T>
        class SharedLibraryClass;
    }
}


/**
 * Container for an object created using a factory provided by a shared library.
 * Used to ensure the object is destroyed by a method also provided by the
 * shared library.  Mixing creation and destruction methods could be very bad.
 */
template <class T>
class yarp::os::SharedLibraryClass {
private:
    T *content;
    SharedLibraryClassFactory<T> *pfactory;
public:

    /**
     * Constructor for empty instance.
     */
    SharedLibraryClass() :
            content(nullptr),
            pfactory(nullptr) {
    }

    /**
     * Constructor for valid instance of a class from a shared library.
     *
     * @param factory the factory to use to construct (and eventually
     * destroy) the instance.
     */
    SharedLibraryClass(SharedLibraryClassFactory<T>& factory) :
            content(nullptr),
            pfactory(nullptr) {
        open(factory);
    }

    /**
     * Construct an instance using the specified factory.  If an
     * instance has already been made, it is destroyed.
     *
     * @param factory the factory to use to construct (and eventually
     * destroy) the instance.
     * @return true on success
     */
    bool open(SharedLibraryClassFactory<T>& factory) {
        close();
        content = factory.create();
        pfactory = &factory;
        factory.addRef();

        return content != nullptr;
    }

    /**
     * Destroy an instance if one has been created.
     *
     * @return true on success
     */
    virtual bool close() {
        if (content != nullptr) {
            pfactory->destroy(content);
            NetworkBase::lock();
            if (pfactory->removeRef() == 0) {
                delete pfactory;
            }
            NetworkBase::unlock();
        }

        content = nullptr;
        pfactory = nullptr;

        return true;
    }

    /**
     * Destructor.
     */
    virtual ~SharedLibraryClass() {
        close();
    }

    /**
     * Gives access to the created instance.  No check made to ensure
     * an instance is in fact present.  Call SharedLibraryClass::isValid
     * first if unsure.
     *
     * @return the created instance
     */
    T& getContent() {
        return *content;
    }

    /**
     * Check whether a valid instance has been created.
     *
     * @return true iff a valid instance has been created
     */
    bool isValid() const {
        return content != nullptr;
    }

    /**
     * Shorthand for SharedLibraryClass::getContent
     *
     * @return the created instance
     */
    T& operator*() {
        return (*content);
    }

    /**
     * A pointer version of SharedLibraryClass::getContent
     *
     * @return a pointer to the created instance, or nullptr if there is
     *         none
     */
    T *operator->() {
        return (content);
    }
};


#endif // YARP_OS_SHAREDLIBRARYCLASS_H
