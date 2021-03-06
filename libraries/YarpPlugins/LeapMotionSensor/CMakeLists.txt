# Copyright: (C) 2017 Universidad Carlos III de Madrid
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

if(NOT Leap_FOUND AND (NOT DEFINED ENABLE_LeapMotionSensor OR ENABLE_LeapMotionSensor))
    message(WARNING "Leap package not found, disabling LeapMotionSensor")
endif()

yarp_prepare_plugin(LeapMotionSensor
                    CATEGORY device
                    TYPE roboticslab::LeapMotionSensor
                    INCLUDE LeapMotionSensor.hpp
                    DEFAULT ON
                    DEPENDS Leap_FOUND)

if(NOT SKIP_LeapMotionSensor)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR} # yarp plugin builder needs this
                        ${Leap_INCLUDE_DIR})

    yarp_add_plugin(LeapMotionSensor LeapMotionSensor.hpp
                                     DeviceDriverImpl.cpp
                                     IAnalogSensorImpl.cpp)

    target_link_libraries(LeapMotionSensor YARP::YARP_OS
                                           YARP::YARP_dev
                                           YARP::YARP_sig
                                           ${Leap_LIBRARY}
                                           ROBOTICSLAB::ColorDebug)

    yarp_install(TARGETS LeapMotionSensor
                 COMPONENT runtime
                 LIBRARY DESTINATION ${ROBOTICSLAB-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
                 ARCHIVE DESTINATION ${ROBOTICSLAB-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

    yarp_install(FILES LeapMotionSensor.ini
                 COMPONENT runtime
                 DESTINATION ${ROBOTICSLAB-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})

else()

    set(ENABLE_LeapMotionSensor OFF CACHE BOOL "Enable/disable LeapMotionSensor device" FORCE)

endif()
