# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

yarp_prepare_plugin(mjpeg
                    CATEGORY carrier
                    TYPE yarp::os::MjpegCarrier
                    INCLUDE MjpegCarrier.h
                    EXTRA_CONFIG CODE="GET /?ac"
                    DEPENDS "CREATE_OPTIONAL_CARRIERS;YARP_HAS_JPEG")

if(NOT SKIP_mjpeg)
  option(MJPEG_AUTOCOMPRESS "Automatically compress/decompress images on mjpeg stream" TRUE)
  if(MJPEG_AUTOCOMPRESS)
    add_definitions(-DMJPEG_AUTOCOMPRESS)
  endif()

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  include_directories(SYSTEM ${JPEG_INCLUDE_DIR})

  yarp_add_plugin(yarp_mjpeg
                  MjpegCarrier.h
                  MjpegCarrier.cpp
                  MjpegStream.h
                  MjpegStream.cpp
                  MjpegDecompression.h
                  MjpegDecompression.cpp)
  target_link_libraries(yarp_mjpeg YARP::YARP_OS
                                   YARP::YARP_sig
                                   YARP::YARP_wire_rep_utils
                                   ${JPEG_LIBRARY})

  yarp_install(TARGETS yarp_mjpeg
               EXPORT YARP
               COMPONENT runtime
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})
  yarp_install(FILES mjpeg.ini
               COMPONENT runtime
               DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

  set_property(TARGET yarp_mjpeg PROPERTY FOLDER "Plugins/Carrier")
endif()
