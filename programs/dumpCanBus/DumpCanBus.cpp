// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "DumpCanBus.hpp"

namespace teo
{

/************************************************************************/
DumpCanBus::DumpCanBus() { }

/************************************************************************/
//bool DumpCanBus::configure(yarp::os::ResourceFinder &rf)
//{

//    if(rf.check("help"))
//    {
//        printf("DumpCanBus options:\n");
//        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        //CD_DEBUG_NO_HEADER("%s\n",rf.toString().c_str());
//        return false;
//    }

    //CD_DEBUG("%s\n",rf.toString().c_str());
//    deviceDevCan0.open(rf);
  //  if (!deviceDevCan0.isValid())
  //  {
    //    CD_ERROR("deviceDevCan0 instantiation not worked.\n");
     //   return false;
   // }
   // deviceDevCan0.view(iCanBus);

    //lastNow = yarp::os::Time::now();

    //return this->Thread::start();
//}

/************************************************************************/

bool DumpCanBus::updateModule()
{
    printf("DumpCanBus alive...\n");
    return true;
}

/************************************************************************/
bool interruptModule()
{
    printf("Modulo interrumpido\n");
    return true;
}

/************************************************************************/

bool DumpCanBus::close()
{
    //this->Thread::stop();

    //yarp::os::Time::delay(2);

   // deviceDevCan0.close();

    //yarp::os::Time::delay(2);

    return true;
}

/************************************************************************/
/*
std::string DumpCanBus::msgToStr(can_msg* message)
{


    std::stringstream tmp;
    for(int i=0; i < message->dlc-1; i++)
    {
        tmp << std::hex << static_cast<int>(message->data[i]) << " ";
    }
    tmp << std::hex << static_cast<int>(message->data[message->dlc-1]);
    tmp << ". canId(";
    tmp << std::dec << (message->id & 0x7F);
    tmp << ") via(";
    tmp << std::hex << (message->id & 0xFF80);
    tmp << "), t:" << yarp::os::Time::now() - lastNow << "[s].";

    lastNow = yarp::os::Time::now();

    return tmp.str();
}
*/
/************************************************************************/

}  // namespace teo
