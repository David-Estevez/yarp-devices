// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CuiAbsolute.hpp"

// ------------------- IControlLimitsRaw Related ------------------------------------

bool roboticslab::CuiAbsolute::setLimitsRaw(int axis, double min, double max)
{
    CD_INFO("(%d,%f,%f)\n",axis,min,max);

    //-- Check index within range
    if ( axis != 0 ) return false;

    //-- Store the new limits locally.
    this->max;
    this->min;

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::CuiAbsolute::getLimitsRaw(int axis, double *min, double *max)
{
    CD_INFO("(%d)\n",axis);

    //-- Check index within range
    if( axis != 0 ) return false;

    //-- Get the limits that have been locally stored.
    *min = this->min;
    *max = this->max;

    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::CuiAbsolute::setVelLimitsRaw(int axis, double min, double max)
{
    CD_WARNING("Not implemented.\n");
    return true;
}

// -----------------------------------------------------------------------------

bool roboticslab::CuiAbsolute::getVelLimitsRaw(int axis, double *min, double *max)
{
    CD_WARNING("Not implemented.\n");
    return true;
}

// -----------------------------------------------------------------------------
