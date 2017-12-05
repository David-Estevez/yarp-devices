# Copyright: (C) 2017 Universidad Carlos III de Madrid
# Author: Juan G. Victores & Raul de Santos Rico
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

cmake_minimum_required(VERSION 2.6)

YARP_PREPARE_PLUGIN(CanBusControlboard CATEGORY device TYPE roboticslab::CanBusControlboard INCLUDE CanBusControlboard.hpp WRAPPER controlboardwrapper2)

IF (NOT SKIP_CanBusControlboard)

  find_package(YARP REQUIRED)

  # Include any directories needed for YARP
  include_directories(${YARP_INCLUDE_DIRS} ${ROBOTICSLAB_YARP_DEVICES_INCLUDE_DIRS} ${COLOR_DEBUG_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

  yarp_add_plugin(CanBusControlboard DeviceDriverImpl.cpp IControlLimits2Impl.cpp IControlMode2Impl.cpp IEncodersImpl.cpp IEncodersTimedImpl.cpp IPositionControl2Impl.cpp IPositionDirectImpl.cpp ITorqueControlImpl.cpp IVelocityControl2Impl.cpp CanBusControlboard.cpp IInteractionModeImpl.cpp ThreadImpl.cpp)
  TARGET_LINK_LIBRARIES(CanBusControlboard ${YARP_LIBRARIES})

  # Exporting dependencies for ROBOTICSLAB_YARP_DEVICESConfig.cmake quite manually for now... 
  set(ROBOTICSLAB_YARP_DEVICES_INCLUDE_DIRS ${ROBOTICSLAB_YARP_DEVICES_INCLUDE_DIRS} ${COLOR_DEBUG_INCLUDE_DIRS} CACHE INTERNAL "appended libraries") 
#  set(ROBOTICSLAB_YARP_DEVICES_LIBRARIES ${ROBOTICSLAB_YARP_DEVICES_LIBRARIES} CanBusControlboard CACHE INTERNAL "appended libraries") 

  #install(TARGETS CanBusControlboard DESTINATION lib)
  yarp_install(TARGETS CanBusControlboard
             EXPORT YARP
             COMPONENT runtime
             LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})

  yarp_install(FILES CanBusControlboard.ini
              COMPONENT runtime
              DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})

ENDIF (NOT SKIP_CanBusControlboard)
