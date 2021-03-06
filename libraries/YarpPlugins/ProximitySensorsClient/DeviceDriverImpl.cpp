// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ProximitySensorsClient.hpp"

#include <string>
#include <sstream>

#include <yarp/os/Value.h>

#include <ColorDebug.h>

// ------------------- DeviceDriver Related ------------------------------------

bool roboticslab::ProximitySensorsClient::open(yarp::os::Searchable& config)
{
    CD_INFO("Starting ProximitySensorsClient plugin.\n");
    CD_DEBUG("config: %s.\n", config.toString().c_str());

    std::string local = config.check("local", yarp::os::Value(DEFAULT_LOCAL), "local port").asString();
    std::string remote = config.check("remote", yarp::os::Value(DEFAULT_REMOTE), "remote port").asString();

    sr.setReference(this);
    sr.open(local);
    sr.useCallback();

    std::string carrier;

    if (config.check("usePortMonitor", "enable port monitoring and additional options"))
    {
        std::string pmType = config.check("portMonitorType", yarp::os::Value(DEFAULT_PORTMONITOR_TYPE),
                "port monitor type").asString();
        std::string pmContext = config.check("portMonitorContext", yarp::os::Value(DEFAULT_PORTMONITOR_CONTEXT),
                "port monitor context").asString();
        std::string pmFile = config.check("portMonitorFile", yarp::os::Value(DEFAULT_PORTMONITOR_FILE),
                "port monitor file").asString();

        std::ostringstream oss;
        oss << "tcp+recv.portmonitor+type." << pmType << "+context." << pmContext << "+file." << pmFile;

        carrier = oss.str();

        CD_INFO("Using carrier: %s\n", carrier.c_str());
    }

    thresholdGripper = config.check("thresholdGripper", yarp::os::Value(DEFAULT_THRESHOLD_GRIPPER),
            "sensor threshold (gripper reach distance)").asDouble();
    thresholdAlertHigh = config.check("thresholdAlertHigh", yarp::os::Value(DEFAULT_THRESHOLD_ALERT_HIGH),
            "sensor threshold (proximity alert, high level)").asDouble();
    thresholdAlertLow = config.check("thresholdAlertLow", yarp::os::Value(DEFAULT_THRESHOLD_ALERT_LOW),
            "sensor threshold (proximity alert, low level)").asDouble();

    if (!yarp::os::Network::connect(remote, local, carrier))
    {
        CD_ERROR("Unable to connect to remote port \"%s\".\n", remote.c_str());
        close();
        return false;
    }

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::ProximitySensorsClient::close()
{
    CD_INFO("Closing ProximitySensorsClient plugin.\n");
    sr.disableCallback();
    sr.close();
    return true;
}

// -----------------------------------------------------------------------------
