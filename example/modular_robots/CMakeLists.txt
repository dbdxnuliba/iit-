# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

# Make sure that YARP_DIR is set in your environment to wherever you
# compiled YARP.

cmake_minimum_required(VERSION 3.0)

find_package(YARP REQUIRED)

if(NOT YARP_HAS_MATH_LIB)
  message(FATAL_ERROR "Please compile YARP with math library turned on")
endif()

add_executable(yarpwide main.cpp)
target_link_libraries(yarpwide ${YARP_LIBRARIES})
