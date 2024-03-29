# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project(YARP_gsl)

set(YARP_gsl_HDRS include/yarp/gsl/api.h
                  include/yarp/gsl/Gsl.h)
set(YARP_gsl_SRCS src/Gsl.cpp)
set(YARP_gsl_IMPL_HDRS include/yarp/gsl/impl/gsl_structs.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Source Files"
             FILES ${YARP_gsl_SRCS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${YARP_gsl_HDRS}
                   ${YARP_gsl_IMPL_HDRS})

set_property(GLOBAL APPEND PROPERTY YARP_TREE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
get_property(YARP_TREE_INCLUDE_DIRS GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)

include_directories(${YARP_TREE_INCLUDE_DIRS})

add_library(YARP_gsl ${YARP_gsl_SRCS} ${YARP_gsl_HDRS})
add_library(YARP::YARP_gsl ALIAS YARP_gsl)

target_include_directories(YARP_gsl PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(YARP_gsl PUBLIC YARP::YARP_sig)

set_property(TARGET YARP_gsl PROPERTY PUBLIC_HEADER ${YARP_gsl_HDRS})
set_property(TARGET YARP_gsl PROPERTY PRIVATE_HEADER ${YARP_gsl_IMPL_HDRS})

install(TARGETS YARP_gsl
        EXPORT YARP
        COMPONENT runtime
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp/gsl
        PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp/gsl/impl)

set_property(GLOBAL APPEND PROPERTY YARP_LIBS YARP_gsl)
set_property(TARGET YARP_gsl PROPERTY INCLUDE_DIRS ${YARP_TREE_INCLUDE_DIRS})
set_property(TARGET YARP_gsl PROPERTY FOLDER "Libraries")
