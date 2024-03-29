# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

get_property(YARP_OS_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)
get_property(YARP_sig_INCLUDE_DIRS TARGET YARP_sig PROPERTY INCLUDE_DIRS)
include_directories(${YARP_OS_INCLUDE_DIRS}
                    ${YARP_sig_INCLUDE_DIRS})

set(QtYARPViewPlugin_SRCS qtyarpview_plugin.cpp
                          videoproducer.cpp
                          signalhandler.cpp
                          qtyarpview.cpp
                          ImagePort.cpp)

set(QtYARPViewPlugin_HDRS qtyarpview.h
                          FpsStats.h
                          videoproducer.h
                          signalhandler.h
                          ImagePort.h
                          qtyarpview_plugin.h)
set(QtYARPViewPlugin_QRC_FILES res.qrc)
set(QtYARPViewPlugin_QML_FILES YARPViewMenu.qml
                               VideoSurface.qml
                               YARPViewStatusBar.qml)

qt5_add_resources(QtYARPViewPlugin_QRC_GEN_SRCS ${QtYARPViewPlugin_QRC_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Source Files"
             FILES ${QtYARPViewPlugin_SRCS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${QtYARPViewPlugin_HDRS})
source_group("Resources Files" FILES ${QtYARPViewPlugin_QRC_FILES})
source_group("QML Files" FILES ${QtYARPViewPlugin_QML_FILES})
source_group("Generated Files" FILES ${QtYARPViewPlugin_QRC_GEN_SRCS})

add_library(QtYARPViewPlugin MODULE ${QtYARPViewPlugin_SRCS}
                                    ${QtYARPViewPlugin_HDRS}
                                    ${QtYARPViewPlugin_QRC_FILES}
                                    ${QtYARPViewPlugin_QRC_GEN_SRCS}
                                    ${QtYARPViewPlugin_QML_FILES})
target_link_libraries(QtYARPViewPlugin YARP::YARP_OS
                                       YARP::YARP_init
                                       YARP::YARP_sig
                                       Qt5::Widgets
                                       Qt5::Qml
                                       Qt5::Quick
                                       Qt5::Multimedia)

qtyarp_qml_plugin(QtYARPViewPlugin "${QtYARPViewPlugin_QMLDIR}")

install(TARGETS QtYARPViewPlugin
        COMPONENT utilities
        RUNTIME DESTINATION "${CMAKE_INSTALL_QMLDIR}/${QtYARPViewPlugin_QMLDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_QMLDIR}/${QtYARPViewPlugin_QMLDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_QMLDIR}/${QtYARPViewPlugin_QMLDIR}")

install(FILES qmldir DESTINATION "${CMAKE_INSTALL_QMLDIR}/${QtYARPViewPlugin_QMLDIR}")

set_property(TARGET QtYARPViewPlugin PROPERTY FOLDER "Plugins/QML")
