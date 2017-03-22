// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

//#ifndef __DUMP_CAN_BUS__
//#define __DUMP_CAN_BUS__

#include <yarp/os/RFModule.h>
//#include <yarp/os/Module.h>
//#include <yarp/os/Network.h>
//#include <yarp/os/ResourceFinder.h>
//#include <yarp/os/Port.h>
//#include <yarp/os/BufferedPort.h>

//#include <yarp/dev/PolyDriver.h>
//#include <yarp/dev/Wrapper.h>

//#include <string>
//#include <sstream>
//#include <stdlib.h>

//#include "ICanBusSharer.h"
//#include "ColorDebug.hpp"


namespace teo
{

/**
 * @ingroup dumpCanBus
 *
 * @brief Launches one CAN bus driver, dumps output.
 *
 */
//class DumpCanBus : public yarp::os::RFModule, public yarp::os::Thread
class DumpCanBus : public yarp::os::RFModule
{
public:
    DumpCanBus();
    //bool configure(yarp::os::ResourceFinder &rf);

protected:

    //yarp::dev::PolyDriver deviceDevCan0;
    //ICanBusHico* iCanBus;

    /** A helper function to display CAN messages. */
    //std::string msgToStr(can_msg* message);
    double lastNow;

    virtual double getPeriod()
    {
        return 1.0;
    }
    virtual bool updateModule();
    virtual bool close();
//        virtual bool interruptModule();
//        virtual int period;

    // -------- Thread declarations. Implementation in ThreadImpl.cpp --------

    /**
     * Main body of the new thread.
     * Override this method to do what you want.
     * After Thread::start is called, this
     * method will start running in a separate thread.
     * It is important that this method either keeps checking
     * Thread::isStopping to see if it should stop, or
     * you override the Thread::onStop method to interact
     * with it in some way to shut the new thread down.
     * There is no really reliable, portable way to stop
     * a thread cleanly unless that thread cooperates.
     */
//    virtual void run();
};

}  // namespace teo

//#endif  // __DUMP_CAN_BUS__

