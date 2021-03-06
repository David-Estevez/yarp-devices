# Copyright: (C) 2017 Universidad Carlos III de Madrid
# Author: Juan G. Victores & Raul de Santos Rico
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

yarp_prepare_plugin(CanBusHico
                    CATEGORY device
                    TYPE roboticslab::CanBusHico
                    INCLUDE CanBusHico.hpp
                    DEFAULT ON
                    DEPENDS UNIX)

if(NOT SKIP_CanBusHico)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # yarp plugin builder needs this

yarp_add_plugin(CanBusHico CanBusHico.cpp
                           CanBusHico.hpp
                           hico_api.h
                           DeviceDriverImpl.cpp
                           ICanBusImpl.cpp
                           HicoCanMessage.cpp
                           HicoCanMessage.hpp
                           FilterManager.cpp)

target_link_libraries(CanBusHico YARP::YARP_OS
                                 YARP::YARP_dev
                                 ROBOTICSLAB::ColorDebug
                                 YarpDevicesInterfaces)

yarp_install(TARGETS CanBusHico
             COMPONENT runtime
             LIBRARY DESTINATION ${ROBOTICSLAB-YARP-DEVICES_DYNAMIC_PLUGINS_INSTALL_DIR}
             ARCHIVE DESTINATION ${ROBOTICSLAB-YARP-DEVICES_STATIC_PLUGINS_INSTALL_DIR})

yarp_install(FILES CanBusHico.ini
             COMPONENT runtime
             DESTINATION ${ROBOTICSLAB-YARP-DEVICES_PLUGIN_MANIFESTS_INSTALL_DIR})

endif()
