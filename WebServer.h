#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer {
public:
    WebServer() : working(false), timeLeft(0) {}

    void setRequest(const Request& req);  
    bool isIdle() const;                  
    void processTick();                   

private:
    bool working;
    int timeLeft;
    Request currRequest;
};


#endif
