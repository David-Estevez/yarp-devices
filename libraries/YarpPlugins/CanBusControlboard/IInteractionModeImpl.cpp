// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CanBusControlboard.hpp"


// ---------------------------- IInteractionMode Related ----------------------------------

bool roboticslab::CanBusControlboard::getInteractionMode(int axis, yarp::dev::InteractionModeEnum* mode)
{
    // CD_DEBUG("(%d)\n",axis);  // -- is printed too many times...
    //*mode = interactionMode[axis];

    //-- Check index within range
    if ( ! this->indexWithinRange(axis) ) return false;

    return iInteractionModeRaw[axis]->getInteractionModeRaw(0, mode);
}

bool roboticslab::CanBusControlboard::getInteractionModes(int n_joints, int *joints, yarp::dev::InteractionModeEnum* modes)
{
    CD_DEBUG("\n");

        bool ok = true;
        for(unsigned int i=0; i < n_joints; i++)
            ok &= getInteractionMode(joints[i],&modes[i]);
        return ok;
}

bool roboticslab::CanBusControlboard::getInteractionModes(yarp::dev::InteractionModeEnum* modes)
{
    CD_DEBUG("\n");

    bool ok = true;
    for(unsigned int i=0; i < nodes.size(); i++)
        ok &= getInteractionMode(i,&modes[i]);
    return ok;
}

bool roboticslab::CanBusControlboard::setInteractionMode(int axis, yarp::dev::InteractionModeEnum mode)
{
    CD_DEBUG("(%d)\n",axis);

    //-- Check index within range
    if ( ! this->indexWithinRange(axis) ) return false;

    return iInteractionModeRaw[axis]->setInteractionModeRaw(0, mode);
}

bool roboticslab::CanBusControlboard::setInteractionModes(int n_joints, int *joints, yarp::dev::InteractionModeEnum* modes)
{
    CD_DEBUG("\n");

       bool ok = true;
       for(int j=0; j<n_joints; j++)
            ok &= this->setInteractionMode(joints[j],modes[j]);

       return ok;
}

bool roboticslab::CanBusControlboard::setInteractionModes(yarp::dev::InteractionModeEnum* modes)
{
    CD_DEBUG("\n");

    bool ok = true;
    for(unsigned int i=0; i<nodes.size(); i++)
        ok &= setInteractionMode(i,modes[i]);
    return ok;
}
