// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "TechnosoftIpos.hpp"

// ######################### IPositionControlRaw Related #########################

bool roboticslab::TechnosoftIpos::getAxes(int *ax)
{
    *ax = 1;
    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::positionMoveRaw(int j, double ref)    // encExposed = ref;
{
    CD_INFO("(%d,%f)\n",j,ref);

    //-- Check index within range
    if ( j != 0 ) return false;

    //*************************************************************
    uint8_t msg_position_target[]= {0x23,0x7A,0x60,0x00,0x00,0x00,0x00,0x00}; // Position target

    int position = ref * this->tr * (encoderPulses / 360.0);  // Appply tr & convert units to encoder increments
    memcpy(msg_position_target+4,&position,4);

    if( ! send( 0x600, 8, msg_position_target ) )
    {
        CD_ERROR("Could not send \"position target\". %s\n", msgToStr(0x600, 8, msg_position_target).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"position target\". %s\n", msgToStr(0x600, 8, msg_position_target).c_str() );
    //*************************************************************
    //uint8_t msg_start[]={0x1F,0x00}; // Start the movement with "Discrete motion profile (change set immediately = 0)".
    uint8_t msg_start[]= {0x3F,0x00}; // Start the movement with "Continuous motion profile (change set immediately = 1)".

    if( ! send( 0x200, 2, msg_start ) )
    {
        CD_ERROR("Could not send \"start position\". %s\n", msgToStr(0x200, 2, msg_start).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"start position\". %s\n", msgToStr(0x200, 2, msg_start).c_str() );
    //*************************************************************

    //-- Needed to send next. Sets "Do not assume target position" so later it accepts "Assume target position (update the new motion parameters)".
    //*************************************************************
    uint8_t msg_pos_reset[]= {0x0F,0x00}; // Stop a position profile

    if( ! send( 0x200, 2, msg_pos_reset) )
    {
        CD_ERROR("Could not send \"reset position\". %s\n", msgToStr(0x200, 2, msg_pos_reset).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"reset position\". %s\n", msgToStr(0x200, 2, msg_pos_reset).c_str() );
    //*************************************************************

    //-- it will save the value
    targetPositionSemaphore.wait();
    targetPosition = ref;
    targetPositionSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::positionMoveRaw(const double *refs)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::relativeMoveRaw(int j, double delta)
{
    CD_INFO("(%d, %f)\n",j,delta);

    //-- Check index within range
    if ( j != 0 ) return false;


    //*************************************************************
    uint8_t msg_position_target[]= {0x23,0x7A,0x60,0x00,0x00,0x00,0x00,0x00}; // Position target

    //-- 11.38 = ( 4 * 1024 pulse / 360 deg ) // deg -> pulse = UI (pos)
    double val = encoderPulses / 360.0; // encoderPulses: value encompasses the pulses-per-slot factor (usually 4) and number of total slots of the encoder (currently: 4 * 1024).
    int sendDelta = delta * this->tr * val;  // Appply tr & convert units to encoder increments
    memcpy(msg_position_target+4,&sendDelta,4);

    if( ! send( 0x600, 8, msg_position_target ) )
    {
        CD_ERROR("Could not send \"position target\". %s\n", msgToStr(0x600, 8, msg_position_target).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"position target\". %s\n", msgToStr(0x600, 8, msg_position_target).c_str() );
    //*************************************************************
    //uint8_t msg_start_rel[]={0x5F,0x00}; // Start the movement with "Discrete motion profile (change set immediately = 0)".
    uint8_t msg_start_rel[]= {0x7F,0x00}; // Start the movement with "Continuous motion profile (change set immediately = 1)".


    if( ! send( 0x200, 2, msg_start_rel ) )
    {
        CD_ERROR("Could not send \"start rel position. %s\n", msgToStr(0x200, 2, msg_start_rel).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"start rel position\". %s\n", msgToStr(0x200, 2, msg_start_rel).c_str() );;
    //*************************************************************

    //-- Needed to send next. Sets "Do not assume target position" so later it accepts "Assume target position (update the new motion parameters)".
    //*************************************************************
    uint8_t msg_pos_reset[]= {0x0F,0x00}; // Stop a position profile

    if( ! send( 0x200, 2, msg_pos_reset) )
    {
        CD_ERROR("Could not send \"reset position\". %s\n", msgToStr(0x200, 2, msg_pos_reset).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"reset position\". %s\n", msgToStr(0x200, 2, msg_pos_reset).c_str() );
    //*************************************************************

    //-- it will save the value
    targetPositionSemaphore.wait();
    targetPosition = delta;
    targetPositionSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::relativeMoveRaw(const double *deltas)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::checkMotionDoneRaw(int j, bool *flag)
{
    CD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( j != 0 ) return false;

    //*************************************************************
    uint8_t msgStatus[] = {0x40,0x41,0x60,0x00,0x00,0x00,0x00,0x00}; // Manual 2064h: Memory position
    if( ! send( 0x600, 8, msgStatus))
    {
        CD_ERROR("Could not send status query. %s\n", msgToStr(0x600, 8, msgStatus).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"msgStatus\". %s\n", msgToStr(0x600, 8, msgStatus).c_str() );

    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    yarp::os::Time::delay(DELAY);  //-- Wait for read update. Could implement semaphore waiting for specific message...
    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    targetReachedReady.wait();
    *flag = targetReached;
    targetReachedReady.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::checkMotionDoneRaw(bool *flag)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::setRefSpeedRaw(int j, double sp)
{
    CD_INFO("(%d, %f)\n",j,sp);

    //-- Check index within range
    if ( j != 0 ) return false;

    //*************************************************************

    uint8_t msg_posmode_speed[]= {0x23,0x81,0x60,0x00,0x00,0x00,0x00,0x00}; // Manual 6081h: Profile velocity

    //uint16_t sendRefSpeed = sp * this->tr * 0.01138;  // Appply tr & convert units to encoder increments
    //memcpy(msg_posmode_speed+6,&sendRefSpeed,2);
    //float sendRefSpeed = sp * this->tr / 22.5;  // Apply tr & convert units to encoder increments
    //int32_t sendRefSpeedFormated = roundf(sendRefSpeed * 65536);  // 65536 = 2^16

    //-- 65536 for FIXED32
    //-- 0.01138 = ( 4 * 1024 pulse / 360 deg ) * (0.001 s / sample)   // deg/s -> pulse/sample  = UI (vel)
    //-- encoderPulses: value encompasses the pulses-per-slot factor (usually 4) and number of total slots of the encoder (currently: 4 * 1024)
    double val = (encoderPulses / 360.0) * 0.001;     //-- if encoderPulses is 4096 (4 * 1024), val = 0,011377778
    int32_t sendRefSpeedFormated = sp * this->tr * (65536 * val); //-- if encoderPulses is 4096 -> 65536 * 0.01138 = 745.8
    memcpy(msg_posmode_speed+4,&sendRefSpeedFormated,4);

    if( ! send( 0x600, 8, msg_posmode_speed) )
    {
        CD_ERROR("Could not send \"posmode_speed\". %s\n", msgToStr(0x600, 8, msg_posmode_speed).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"posmode_speed\". %s\n", msgToStr(0x600, 8, msg_posmode_speed).c_str() );
    //*************************************************************

    //-- it will save the value
    refSpeedSemaphore.wait();
    refSpeed = sp;
    refSpeedSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::setRefSpeedsRaw(const double *spds)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::setRefAccelerationRaw(int j, double acc)
{
    CD_INFO("(%d, %f)\n",j,acc);

    //-- Check index within range
    if ( j != 0 ) return false;

    //*************************************************************
    uint8_t msg_posmode_acc[]= {0x23,0x83,0x60,0x00,0x00,0x00,0x00,0x00}; // Manual 6083h: Profile acceleration

    //int sendRefAcc = acc * this->tr * 0.00001138;  // Appply tr & convert units to encoder increments
    //memcpy(msg_posmode_acc+4,&sendRefAcc,4);
    //int32_t sendRefAccFormated = roundf(sendRefAcc * 65536);  // 65536 = 2^16

    //-- 65536 for FIXED32
    //-- 0.00001138 = ( 4 * 1024 pulse / 360 deg ) * (0.000001 s^2 / sample^2)   // deg/s^2 -> pulse/sample^2 = UI (acc)
    //-- encoderPulses: value encompasses the pulses-per-slot factor (usually 4) and number of total slots of the encoder (currently: 4 * 1024)
    double val = (encoderPulses / 360.0) * 0.000001;     //-- if encoderPulses is 4096 (4 * 1024), val = 0.00001138
    int32_t sendRefAccFormated = acc * this->tr * (65536 * val); //-- 65536 * 0.00001138 = 0.7458
    memcpy(msg_posmode_acc+4,&sendRefAccFormated,4);

    if( ! send( 0x600, 8, msg_posmode_acc) )
    {
        CD_ERROR("Could not send \"posmode_acc\". %s\n", msgToStr(0x600, 8, msg_posmode_acc).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"posmode_acc\". %s\n", msgToStr(0x600, 8, msg_posmode_acc).c_str() );
    //*************************************************************

    //-- it will save the value
    refAccelSemaphore.wait();
    refAcceleration = acc ;
    refAccelSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::setRefAccelerationsRaw(const double *accs)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getRefSpeedRaw(int j, double *ref)
{
    CD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( j != 0 ) return false;

    uint8_t msg_posmode_speed[]= {0x40,0x81,0x60,0x00,0x00,0x00,0x00,0x00}; // Manual 6081h: Profile velocity

    if( ! send( 0x600, 8, msg_posmode_speed) )
    {
        CD_ERROR("Could not send \"posmode_speed\" query. %s\n", msgToStr(0x600, 8, msg_posmode_speed).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"posmode_speed\" query. %s\n", msgToStr(0x600, 8, msg_posmode_speed).c_str() );

    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    yarp::os::Time::delay(DELAY);  //-- Wait for read update. Could implement semaphore waiting for specific message...
    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    refSpeedSemaphore.wait();
    *ref = refSpeed;
    refSpeedSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getRefSpeedsRaw(double *spds)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getRefAccelerationRaw(int j, double *acc)
{
    CD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( j != 0 ) return false;

    uint8_t msg_posmode_acc[]= {0x40,0x83,0x60,0x00,0x00,0x00,0x00,0x00}; // Manual 6083h: Profile acceleration

    if( ! send( 0x600, 8, msg_posmode_acc) )
    {
        CD_ERROR("Could not send \"posmode_acc\" query. %s\n", msgToStr(0x600, 8, msg_posmode_acc).c_str() );
        return false;
    }
    CD_SUCCESS("Sent \"posmode_acc\" query. %s\n", msgToStr(0x600, 8, msg_posmode_acc).c_str() );

    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    yarp::os::Time::delay(DELAY);  //-- Wait for read update. Could implement semaphore waiting for specific message...
    //* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    refAccelSemaphore.wait();
    *acc = refAcceleration;
    refAccelSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getRefAccelerationsRaw(double *accs)
{
    CD_ERROR("\n");
    return false;
}

// -----------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::stopRaw(int j)
{
    CD_INFO("(%d)\n",j);

    //-- Check index within range
    if ( j != 0 ) return false;

    CD_WARNING("Not implemented yet (TechnosoftIpos).\n");

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::stopRaw()
{
    CD_ERROR("\n");
    return false;
}

// ######################### IPositionControl2Raw Related #########################

bool roboticslab::TechnosoftIpos::positionMoveRaw(const int n_joint, const int *joints, const double *refs)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::relativeMoveRaw(const int n_joint, const int *joints, const double *deltas)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::checkMotionDoneRaw(const int n_joint, const int *joints, bool *flags)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::setRefSpeedsRaw(const int n_joint, const int *joints, const double *spds)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------
/*
bool roboticslab::TechnosoftIpos::setRefAccelerationsRaw(const int n_joint, const int *joints, const double *accs)
{
    return true;
}
*/

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getRefSpeedsRaw(const int n_joint, const int *joints, double *spds)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------

/*
bool roboticslab::TechnosoftIpos::getRefAccelerationsRaw(const int n_joint, const int *joints, double *accs)
{
    return true;
}
*/

// --------------------------------------------------------------------------------

/*
bool roboticslab::TechnosoftIpos::stopRaw(const int n_joint, const int *joints)
{
    return true;
}
*/

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getTargetPositionRaw(const int joint, double *ref)
{
    CD_INFO("\n");

    targetPositionSemaphore.wait();
    *ref = targetPosition;
    targetPositionSemaphore.post();

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getTargetPositionsRaw(double *refs)
{
    CD_WARNING("Missing implementation\n");

    return true;
}

// --------------------------------------------------------------------------------

bool roboticslab::TechnosoftIpos::getTargetPositionsRaw(const int n_joint, const int *joints, double *refs)
{
    CD_WARNING("Missing implementation\n");

    return true;
}
