# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

cmake_minimum_required(VERSION 3.0)

find_package(YARP REQUIRED)

yarp_begin_plugin_library(baz)
add_subdirectory(bazBot)
add_subdirectory(bazDevice)
yarp_end_plugin_library(baz)
