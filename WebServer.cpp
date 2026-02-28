#include "WebServer.h"
#include "Colors.h"
#include <iostream>


void WebServer::setLogServerMessages(const std::string& value)
{
    logServerMessages = value;  
}

void WebServer::setRequest(const Request& req)
{
    currRequest = req;
    timeLeft = req.neededTime;
    working = true;

    if (logServerMessages == "yes") {
        std::cout << COLOR_GREEN 
                  << "Server starting request "
                  << currRequest.ipIn << " -> " << currRequest.ipOut
                  << " (" << currRequest.jobType
                  << ", time=" << currRequest.neededTime << ")" << COLOR_RESET << "\n";
    }
}


bool WebServer::isIdle() const
{
    return !working;
}

bool WebServer::processTick()
{
    if (!working) return false;

    timeLeft -= 1;
    if (timeLeft <= 0) {
        working = false;   
        timeLeft = 0;
        
        if (logServerMessages == "yes") {
            std::cout << COLOR_GREEN << "Server finished request "
                      << currRequest.ipIn << " -> " << currRequest.ipOut
                      << " (" << currRequest.jobType << ")" << COLOR_RESET << "\n";
        }
        return true;
    }
    return false;
}
