# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

include(YarpInstallationHelpers)
include(YarpDeprecatedOption)

yarp_configure_external_installation(yarp WITH_PLUGINS)

# public libraries
add_subdirectory(libYARP_conf)
add_subdirectory(libYARP_OS)
add_subdirectory(libYARP_sig)
add_subdirectory(libYARP_eigen)
add_subdirectory(libYARP_gsl)
add_subdirectory(libYARP_math)
add_subdirectory(libYARP_rosmsg)
add_subdirectory(libYARP_dev)
add_subdirectory(libYARP_pcl)

# private libraries
add_subdirectory(libYARP_name)
add_subdirectory(libYARP_serversql)
add_subdirectory(libYARP_run)
add_subdirectory(libYARP_manager)
add_subdirectory(libYARP_profiler)
add_subdirectory(libYARP_logger)
add_subdirectory(libYARP_wire_rep_utils)

# plugins
set (YARP_LINK_PLUGINS TRUE)
if(YARP_FORCE_DYNAMIC_PLUGINS OR BUILD_SHARED_LIBS)
  set (YARP_LINK_PLUGINS FALSE)
endif()
add_subdirectory(devices)
add_subdirectory(carriers)
add_subdirectory(libYARP_init)

if(YARP_COMPILE_EXECUTABLES)
    # idl compilers (thrift and ros)
    add_subdirectory(idls)

    # executables
    add_subdirectory(yarpserver)
    add_subdirectory(yarp)
    add_subdirectory(yarp-config)
    add_subdirectory(yarprun)
    add_subdirectory(yarphear)
    add_subdirectory(yarpdev)
    add_subdirectory(yarprobotinterface)
    add_subdirectory(yarpmanager-console)
    add_subdirectory(yarplogger-console)
    add_subdirectory(yarpdatadumper)

    # Qt5 GUIs
    add_subdirectory(yarpview)
    add_subdirectory(yarpscope)
    add_subdirectory(yarpmanager)
    add_subdirectory(yarplogger)
    add_subdirectory(yarpdataplayer)
    add_subdirectory(yarpmotorgui)
    add_subdirectory(yarpbatterygui)
    add_subdirectory(yarplaserscannergui)
    add_subdirectory(yarpviz)
endif()

# Robot Testing Framework addons
if(YARP_COMPILE_RTF_ADDONS)
  add_subdirectory(libYARP_rtf)
  add_subdirectory(rtf-plugins)
endif()

# unmaintained stuff
add_subdirectory(libyarpc)
if(YARP_COMPILE_EXECUTABLES)
  add_subdirectory(yarpfs)
endif()

yarp_deprecated_option(YARP_USE_PERSISTENT_NAMESERVER)
