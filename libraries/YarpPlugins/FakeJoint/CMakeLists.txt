# Copyright: (C) 2017 Universidad Carlos III de Madrid
# Author: Juan G. Victores & Raul de Santos Rico
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

yarp_prepare_plugin(FakeJoint
                    CATEGORY device
                    TYPE roboticslab::FakeJoint
                    INCLUDE FakeJoint.hpp
                    DEFAULT ON)

if(NOT SKIP_FakeJoint)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # yarp plugin builder needs this

yarp_add_plugin(FakeJoint FakeJoint.cpp
                          FakeJoint.hpp
                          DeviceDriverImpl.cpp
                          ICanBusSharerImpl.cpp
                          IControlLimits2RawImpl.cpp
                          IControlMode2RawImpl.cpp
                          IEncodersRawImpl.cpp
                          IEncodersTimedRawImpl.cpp
                          IInteractionModeRawImpl.cpp
                          IPositionControl2RawImpl.cpp
                          IPositionDirectRawImpl.cpp
                          ITorqueControlRawImpl.cpp
                          IVelocityControl2RawImpl.cpp)

target_link_libraries(FakeJoint YARP::YARP_OS
                                YARP::YARP_dev
                                ROBOTICSLAB::ColorDebug
                                YarpDevicesInterfaces)

yarp_install(TARGETS FakeJoint
             COMPONENT runtime
             LIBRARY DESTINATION ${ROBOTICSLAB-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${ROBOTICSLAB-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

yarp_install(FILES FakeJoint.ini
             COMPONENT runtime
             DESTINATION ${ROBOTICSLAB-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
