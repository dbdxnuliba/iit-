# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

if(CREATE_LIB_MANAGER)

  get_property(YARP_TREE_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)

  project(YARP_manager)

  set(YARP_manager_HDRS include/yarp/manager/application.h
                        include/yarp/manager/arbitrator.h
                        include/yarp/manager/binexparser.h
                        include/yarp/manager/broker.h
                        include/yarp/manager/data.h
                        include/yarp/manager/execstate.h
                        include/yarp/manager/executable.h
                        include/yarp/manager/fsm.h
                        include/yarp/manager/graph.h
                        include/yarp/manager/kbase.h
                        include/yarp/manager/localbroker.h
                        include/yarp/manager/logicresource.h
                        include/yarp/manager/manager.h
                        include/yarp/manager/manifestloader.h
                        include/yarp/manager/module.h
                        include/yarp/manager/node.h
                        include/yarp/manager/physicresource.h
                        include/yarp/manager/primresource.h
                        include/yarp/manager/resource.h
                        include/yarp/manager/scriptbroker.h
                        include/yarp/manager/singleapploader.h
                        include/yarp/manager/utility.h
                        include/yarp/manager/xmlapploader.h
                        include/yarp/manager/xmlclusterloader.h
                        include/yarp/manager/xmlappsaver.h
                        include/yarp/manager/xmlmodloader.h
                        include/yarp/manager/xmlresloader.h
                        include/yarp/manager/xmltemploader.h
                        include/yarp/manager/yarpbroker.h
                        include/yarp/manager/yarpdevbroker.h
                        include/yarp/manager/ymm-types.h)

  set(YARP_manager_IMPL_HDRS include/yarp/manager/impl/textparser.h)

  set(YARP_manager_SRCS src/application.cpp
                        src/arbitrator.cpp
                        src/binexparser.cpp
                        src/broker.cpp
                        src/data.cpp
                        src/execstate.cpp
                        src/executable.cpp
                        src/graph.cpp
                        src/kbase.cpp
                        src/localbroker.cpp
                        src/logicresource.cpp
                        src/manager.cpp
                        src/module.cpp
                        src/node.cpp
                        src/physicresource.cpp
                        src/primresource.cpp
                        src/resource.cpp
                        src/scriptbroker.cpp
                        src/singleapploader.cpp
                        src/utility.cpp
                        src/xmlapploader.cpp
                        src/xmlclusterloader.cpp
                        src/xmlappsaver.cpp
                        src/xmlmodloader.cpp
                        src/xmlresloader.cpp
                        src/xmltemploader.cpp
                        src/yarpbroker.cpp)

  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
               PREFIX "Source Files"
               FILES ${YARP_manager_SRCS})
  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
               PREFIX "Header Files"
               FILES ${YARP_manager_HDRS}
                     ${YARP_manager_IMPL_HDRS})

  set_property(GLOBAL APPEND PROPERTY YARP_TREE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
  get_property(YARP_TREE_INCLUDE_DIRS GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)

  include_directories(${YARP_TREE_INCLUDE_DIRS})
  include_directories(SYSTEM ${TinyXML_INCLUDE_DIRS})
  if(MSVC)
    include_directories(SYSTEM ${dirent_INCLUDE_DIRS})
  endif()

  if(CREATE_LIB_MATH)
    add_definitions( -DWITH_YARPMATH )
  endif()


  # FIXME All API should use a YARP_manager_API for __declspec(dllimport/dllexport)
  # For now always build the library as STATIC
  add_library(YARP_manager STATIC ${YARP_manager_SRCS}
                                  ${YARP_manager_HDRS}
                                  ${YARP_manager_IMPL_HDRS})
  add_library(YARP::YARP_manager ALIAS YARP_manager)

  target_include_directories(YARP_manager PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  target_link_libraries(YARP_manager PUBLIC YARP::YARP_OS
                                     PRIVATE YARP::YARP_sig
                                             ${TinyXML_LIBRARIES})

  if(CREATE_LIB_MATH)
    target_link_libraries(YARP_manager PRIVATE YARP::YARP_math)
  endif()

  if(NOT CMAKE_MINIMUM_REQUIRED_VERSION VERSION_LESS 3.1)
    message(AUTHOR_WARNING "CMAKE_MINIMUM_REQUIRED_VERSION is now ${CMAKE_MINIMUM_REQUIRED_VERSION}. This check can be removed.")
  endif()
  if(CMAKE_VERSION VERSION_LESS 3.1)
    if(DEFINED CXX11_FLAGS)
      target_compile_options(YARP_manager PUBLIC ${CXX11_FLAGS})
    endif()
  else()
    target_compile_features(YARP_manager PUBLIC cxx_override)
  endif()

  set_property(TARGET YARP_manager PROPERTY PUBLIC_HEADER ${YARP_manager_HDRS})
  set_property(TARGET YARP_manager PROPERTY PRIVATE_HEADER ${YARP_manager_IMPL_HDRS})

  install(TARGETS YARP_manager
          EXPORT YARP
          COMPONENT runtime
          RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
          LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
          PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/manager"
          PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/manager/impl")

  set_property(GLOBAL APPEND PROPERTY YARP_LIBS YARP_manager)
  set_property(TARGET YARP_manager PROPERTY INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
  set_property(TARGET YARP_manager PROPERTY FOLDER "Libraries/Private")
endif()
