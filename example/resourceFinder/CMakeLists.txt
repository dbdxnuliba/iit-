# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

cmake_minimum_required(VERSION 3.0)
project(resourceFinder)

find_package(YARP REQUIRED)

add_executable(rf_basic tutorial_rf_basic.cpp)
target_link_libraries(rf_basic ${YARP_LIBRARIES})

add_executable(rf_advanced tutorial_rf_advanced.cpp)
target_link_libraries(rf_advanced ${YARP_LIBRARIES})

# to install data files:
set(conf randomMotion/config.ini)
# alternatively, if you are too lazy to list all files explicitly, you can "GLOB" them with:
# file(GLOB conf conf/*.ini)

yarp_install(FILES ${conf} DESTINATION ${YARP_CONTEXTS_INSTALL_DIR}/randomMotion)

