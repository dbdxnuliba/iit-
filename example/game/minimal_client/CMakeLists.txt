# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

cmake_minimum_required(VERSION 3.0)
project(minimal_client)

find_package(YARP REQUIRED)

if(WIN32 AND NOT CYGWIN)
  add_definitions(-DWIN32 -D_WINDOWS)
endif(WIN32 AND NOT CYGWIN)

if(WIN32 AND CYGWIN)
  add_definitions(-DCYGWIN)
endif(WIN32 AND CYGWIN)

add_executable(minimal_client main.cpp)
target_link_libraries(minimal_client ${YARP_LIBRARIES})
