// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __I_CAN_BUS_SHARER__
#define __I_CAN_BUS_SHARER__

#include <yarp/dev/IEncodersTimed.h>
#include <yarp/dev/CanBusInterface.h>

#define DELAY 0.001  // [s] Required when using same driver.

namespace roboticslab
{

/**
 *
 * @brief Abstract base for a CAN bus sharer.
 *
 */
class ICanBusSharer
{
public:
    /**
     * Destructor.
     */
    virtual ~ICanBusSharer() {}

    virtual bool setCanBusPtr(yarp::dev::ICanBus * canDevicePtr) = 0;

    virtual bool setIEncodersTimedRawExternal(yarp::dev::IEncodersTimedRaw * iEncodersTimedRaw) = 0;

    /** "start". Figure 5.1 Drive’s status machine. States and transitions (p68, 84/263). */
    virtual bool start() = 0;

    /** "ready to switch on", also acts as "shutdown" */
    virtual bool readyToSwitchOn() = 0;

    /** "switch on", also acts as "disable operation" */
    virtual bool switchOn() = 0;

    /** enable */
    virtual bool enable() = 0;

    /** recoverFromError */
    virtual bool recoverFromError() = 0;

    /**
     * Interpret a can bus message.
     * @return true/false.
     */
    virtual bool interpretMessage(const yarp::dev::CanMessage & message) = 0;

};

}  // namespace roboticslab

#endif  //  __I_CAN_BUS_SHARER__
