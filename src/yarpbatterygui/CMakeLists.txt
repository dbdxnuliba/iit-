# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

if(CREATE_YARPBATTERYGUI)

  include(YarpUseQt5)
  include(YarpOSXUtilities)

  get_property(YARP_OS_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)
  get_property(YARP_sig_INCLUDE_DIRS TARGET YARP_sig PROPERTY INCLUDE_DIRS)
  get_property(YARP_dev_INCLUDE_DIRS TARGET YARP_dev PROPERTY INCLUDE_DIRS)
  include_directories(${YARP_OS_INCLUDE_DIRS}
                      ${YARP_sig_INCLUDE_DIRS}
                      ${YARP_dev_INCLUDE_DIRS})

  set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

  set(yarpbatterygui_SRCS main.cpp
                          display.cpp)
  set(yarpbatterygui_HDRS display.h)

  set(yarpbatterygui_QRC_FILES res.qrc)

  set(yarpbatterygui_UI_FILES display.ui)

  qt5_add_resources(yarpbatterygui_QRC_GEN_SRCS ${yarpbatterygui_QRC_FILES})

  qt5_wrap_ui(yarpbatterygui_UI_GEN_SRCS ${yarpbatterygui_UI_FILES})

  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
               PREFIX "Source Files"
               FILES ${yarpbatterygui_SRCS})
  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
               PREFIX "Header Files"
               FILES ${yarpbatterygui_HDRS})
  source_group("Resources Files" FILES ${yarpbatterygui_QRC_FILES})
  source_group("UI Files" FILES ${yarpbatterygui_UI_FILES})
  source_group("Generated Files" FILES ${yarpbatterygui_QRC_GEN_SRCS}
                                       ${yarpbatterygui_UI_GEN_SRCS})

  add_executable(yarpbatterygui WIN32 ${yarpbatterygui_SRCS}
                                      ${yarpbatterygui_HDRS}
                                      ${yarpbatterygui_QRC_FILES}
                                      ${yarpbatterygui_QRC_GEN_SRCS}
                                      ${yarpbatterygui_UI_FILES}
                                      ${yarpbatterygui_UI_GEN_SRCS})

  target_link_libraries(yarpbatterygui YARP::YARP_OS
                                       YARP::YARP_init
                                       YARP::YARP_sig
                                       YARP::YARP_dev
                                       Qt5::Widgets
                                       Qt5::Gui)

  install(TARGETS yarpbatterygui COMPONENT utilities DESTINATION ${CMAKE_INSTALL_BINDIR})

  set_property(TARGET yarpbatterygui PROPERTY FOLDER "Graphical User Interfaces")

  yarp_osx_duplicate_and_add_bundle(TARGET yarpbatterygui
                                    INSTALL_COMPONENT utilities
                                    INSTALL_DESTINATION "${CMAKE_INSTALL_BINDIR}")

endif()
