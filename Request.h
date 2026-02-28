#ifndef REQUEST_H 
#define REQUEST_H 

#include <string> 

struct Request {
    std::string ipIn; 
    std::string ipOut; 
    int neededTime; 
    std::string jobType;

    Request() : neededTime(0) {} 

    Request(const std::string& in, const std::string& out, int timeNeeded, const std::string& type) {
        ipIn = in; 
        ipOut = out; 
        neededTime = timeNeeded;
        jobType = type; 
    }
};

#endif