# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

get_property(YARP_TREE_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)

project(YARP_logger)

set(YARP_logger_HDRS include/yarp/logger/YarpLogger.h)

set(YARP_logger_IMPL_HDRS )

set(YARP_logger_SRCS src/YarpLogger.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Source Files"
             FILES ${YARP_logger_SRCS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${YARP_logger_HDRS}
                   ${YARP_logger_IMPL_HDRS})

set_property(GLOBAL APPEND PROPERTY YARP_TREE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
get_property(YARP_TREE_INCLUDE_DIRS GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)

include_directories(${YARP_TREE_INCLUDE_DIRS})


if(CREATE_LIB_MATH)
  add_definitions( -DWITH_YARPMATH )
endif()


# FIXME All API should use a YARP_logger_API for __declspec(dllimport/dllexport)
# For now always build the library as STATIC
add_library(YARP_logger STATIC ${YARP_logger_SRCS}
                                ${YARP_logger_HDRS}
                                ${YARP_logger_IMPL_HDRS})
add_library(YARP::YARP_logger ALIAS YARP_logger)

target_include_directories(YARP_logger PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(YARP_logger PRIVATE YARP::YARP_OS
                                          YARP::YARP_sig)

if(CREATE_LIB_MATH)
  target_link_libraries(YARP_logger PRIVATE YARP::YARP_math)
endif()

if(NOT CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS 3.1)
  message(AUTHOR_WARNING "CMAKE_MINIMUM_REQUIRED_VERSION is now ${CMAKE_MINIMUM_REQUIRED_VERSION}. This check can be removed.")
endif()
if(CMAKE_VERSION VERSION_LESS 3.1)
  if(DEFINED CXX11_FLAGS)
    target_compile_options(YARP_logger PUBLIC ${CXX11_FLAGS})
  endif()
else()
  target_compile_features(YARP_logger PUBLIC cxx_override)
endif()

set_property(TARGET YARP_logger PROPERTY PUBLIC_HEADER ${YARP_logger_HDRS})
set_property(TARGET YARP_logger PROPERTY PRIVATE_HEADER ${YARP_logger_IMPL_HDRS})

install(TARGETS YARP_logger
        EXPORT YARP
        COMPONENT runtime
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/logger"
        PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/logger/impl")

set_property(GLOBAL APPEND PROPERTY YARP_LIBS YARP_logger)
set_property(TARGET YARP_logger PROPERTY INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
set_property(TARGET YARP_logger PROPERTY FOLDER "Libraries/Private")
