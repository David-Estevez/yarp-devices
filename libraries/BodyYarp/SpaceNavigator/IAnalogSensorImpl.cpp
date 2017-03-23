// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "SpaceNavigator.hpp"

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::read(yarp::sig::Vector &out)
{
    double dx = 0.0;
    double dy = 0.0;
    double dz = 0.0;
    double droll = 0.0;
    double dpitch = 0.0;
    double dyaw = 0.0;

    int button1 = 0;
    int button2 = 0;

    spnav_event sev;

    if (spnav_poll_event(&sev) != 0)
    {
        if (sev.type == SPNAV_EVENT_MOTION)
        {
            dx = sev.motion.z;
            dy = - sev.motion.x;
            dz = sev.motion.y;
            droll = sev.motion.rz;
            dpitch = - sev.motion.rx;
            dyaw = sev.motion.ry;
        }
        else if (sev.type == SPNAV_EVENT_BUTTON && sev.button.press == 1)
        {
            if (sev.button.bnum == 0)
            {
                button1 = 1;
            }
            else if (sev.button.bnum == 1)
            {
                button2 = 1;
            }
        }
    }

    out.resize(DEFAULT_NUM_CHANNELS);

    out[0] = enforceDeadband(enforceRange(dx/FULL_SCALE_X));
    out[1] = enforceDeadband(enforceRange(dy/FULL_SCALE_Y));
    out[2] = enforceDeadband(enforceRange(dz/FULL_SCALE_Z));
    out[3] = enforceDeadband(enforceRange(droll/FULL_SCALE_ROLL));
    out[4] = enforceDeadband(enforceRange(dpitch/FULL_SCALE_PITCH));
    out[5] = enforceDeadband(enforceRange(dyaw/FULL_SCALE_YAW));

    out[6] = button1;
    out[7] = button2;

    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::getState(int ch)
{
    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::getChannels()
{
    return DEFAULT_NUM_CHANNELS;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::calibrateSensor()
{
    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::calibrateSensor(const yarp::sig::Vector& value)
{
    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::calibrateChannel(int ch)
{
    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------

int roboticslab::SpaceNavigator::calibrateChannel(int ch, double value)
{
    return yarp::dev::IAnalogSensor::AS_OK;
}

// -----------------------------------------------------------------------------
