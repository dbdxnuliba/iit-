# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

cmake_minimum_required(VERSION 3.0)

find_package(YARP REQUIRED)

yarp_prepare_plugin(bazbot
                    CATEGORY device
                    TYPE yarp::dev::bazbot
                    INCLUDE "bazbot.h"
                    DEFAULT ON)

if(NOT SKIP_bazbot)
  set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
  yarp_add_plugin(bazbot bazbot.cpp bazbot.h)
endif(NOT SKIP_bazbot)
