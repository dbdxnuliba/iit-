# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

# Make sure that YARP_DIR is set in your environment to wherever you
# compiled YARP.

cmake_minimum_required(VERSION 3.0)

find_package(YARP REQUIRED)

add_executable(nameserver_proxy main.cpp)
target_link_libraries(nameserver_proxy ${YARP_LIBRARIES})

