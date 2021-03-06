// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TextilesHand.hpp"

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::setCanBusPtr(yarp::dev::ICanBus *canDevicePtr)
{

    this->canDevicePtr = canDevicePtr;
    CD_SUCCESS("Ok pointer to CAN bus device %d.\n",canId);

}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::start()
{

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::readyToSwitchOn()
{

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::switchOn()
{

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::enable()
{

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::recoverFromError()
{

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::TextilesHand::interpretMessage(const yarp::dev::CanMessage & message)
{

    return true;

}  //-- ends interpretMessage

// -----------------------------------------------------------------------------
