// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "FakeJoint.hpp"

// ------------------ IEncodersTimedRaw Related -----------------------------------------

bool roboticslab::FakeJoint::getEncodersTimedRaw(double *encs, double *time)
{
    CD_ERROR("Mising implementation\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::FakeJoint::getEncoderTimedRaw(int j, double *encs, double *time)
{
    //CD_INFO("(%d)\n",j);  //-- Too verbose in controlboardwrapper2 stream.

    //-- Check index within range
    if ( j != 0 ) return false;

    return true;
}

// -----------------------------------------------------------------------------
