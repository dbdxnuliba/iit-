/*
 * Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
 * Copyright (C) 2006-2010 RobotCub Consortium
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms of the
 * BSD-3-Clause license. See the accompanying LICENSE file for details.
 */

#ifndef YARP_OS_NETFLOAT32_H
#define YARP_OS_NETFLOAT32_H

#include <yarp/conf/numeric.h>
#include <yarp/os/api.h>

////////////////////////////////////////////////////////////////////////
//
// The goal of this file is just to define a 32 bit signed little-endian
// floating point type
//
// If you are having trouble with it, and your system has a 32 bit
// little-endian type called e.g. ___my_system_float, you can replace
// this whole file with:
//    typedef ___my_system_float NetFloat32;
//
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//   YARP_FLOAT32 should be a 32-bit float
//   YARP_BIG_ENDIAN should be defined if we are big endian
//   YARP_LITTLE_ENDIAN should be defined if we are little endian


#ifdef YARP_FLOAT32

namespace yarp {
    namespace os {
        /**
         * Definition of the NetFloat32 type
         */

#ifdef YARP_LITTLE_ENDIAN

        typedef YARP_FLOAT32 NetFloat32;

#else // YARP_LITTLE_ENDIAN

        typedef YARP_FLOAT32 RawNetFloat32;
        union UnionNetFloat32 {
            YARP_FLOAT32 d;
            unsigned char c[4];
        };
        class YARP_OS_API NetFloat32 {
        private:
            double raw_value;
            double swap(double x) const;
            RawNetFloat32 get() const;
            void set(RawNetFloat32 v);
        public:
            NetFloat32();
            NetFloat32(RawNetFloat32 val);
            operator RawNetFloat32() const;
            RawNetFloat32 operator+(RawNetFloat32 v) const;
            RawNetFloat32 operator-(RawNetFloat32 v) const;
            RawNetFloat32 operator*(RawNetFloat32 v) const;
            RawNetFloat32 operator/(RawNetFloat32 v) const;
            void operator+=(RawNetFloat32 v);
            void operator-=(RawNetFloat32 v);
            void operator*=(RawNetFloat32 v);
            void operator/=(RawNetFloat32 v);
        };

#endif // YARP_LITTLE_ENDIAN

    }
}

#else // YARP_FLOAT32

#error "NetFloat32 not defined"

#endif // YARP_FLOAT32

#endif // YARP_OS_NETFLOAT32_H
