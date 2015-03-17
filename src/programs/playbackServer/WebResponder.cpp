// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "PlaybackServer.hpp"

namespace teo
{

/************************************************************************/

bool WebResponder::read(yarp::os::ConnectionReader &in) {
    //-- Kind of boilerplate for mini web server
    Bottle got, response;
    if (!got.read(in)) return false;
    ConnectionWriter *out = in.getWriter();
    if (out==NULL) return true;
    response.addString("web");
    CD_INFO("Got: %s\n", got.toString().c_str());
    std::string page = got.get(0).asString();

    //-- Special page response
    //if (page=="index.html") {
    //    response.addString( readFile("index.html"));
    //    //--Add logic here
    //    return response.write(*out);
    //}

    //-- Default page response
    response.addString( readFile(page) );
    return response.write(*out);

    return true;
}

/************************************************************************/

std::string WebResponder::readFile(const std::string& filePath) {
    printf("filePath: %s\n",filePath.c_str());
    // [thanks:author] Tyler McHenry @ nerdland.net and KeithB @ ndssl.vbi.vt.edu
    // [thanks:link] http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring [2012-02-06]
    std::ifstream t(filePath.c_str());
    std::string str;
    if(!t.is_open()) {
        str += "Could not open file \"";
        str += filePath;
        str += "\".";
        return str;
    }
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(t)),
                std::istreambuf_iterator<char>());
    t.close();

}

/************************************************************************/

}  // namespace teo

