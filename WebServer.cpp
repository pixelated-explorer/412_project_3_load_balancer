#include "WebServer.h"
#include <iostream>

void WebServer::setRequest(const Request& req)
{
    currRequest = req;
    timeLeft = req.neededTime;
    working = true;

    std::cout << "Server starting request "
              << currRequest.ipIn << " -> " << currRequest.ipOut
              << " (" << currRequest.jobType
              << ", time=" << currRequest.neededTime << ")\n";
}


bool WebServer::isIdle() const
{
    return !working;
}

void WebServer::processTick()
{
    if (!working) return;

    timeLeft -= 1;
    if (timeLeft <= 0) {
        working = false;   
        timeLeft = 0;

        std::cout << "Server finished request "
                  << currRequest.ipIn << " -> " << currRequest.ipOut
                  << " (" << currRequest.jobType << ")\n";
    }
}
