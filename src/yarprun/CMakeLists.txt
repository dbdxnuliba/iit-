# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

get_property(YARP_OS_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)

include_directories(${YARP_OS_INCLUDE_DIRS})
include_directories(SYSTEM ${ACE_INCLUDE_DIRS})

add_executable(yarprun yarprun.cpp)
target_link_libraries(yarprun YARP::YARP_OS
                              YARP::YARP_init
                              YARP::YARP_run)

install(TARGETS yarprun COMPONENT utilities DESTINATION ${CMAKE_INSTALL_BINDIR})
set_property(TARGET yarprun PROPERTY FOLDER "Command Line Tools")
