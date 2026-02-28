#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

class WebServer {
public:
    WebServer() : working(false), timeLeft(0), logServerMessages("no") {}

    void setLogServerMessages(const std::string& value);
    void setRequest(const Request& req);  
    bool isIdle() const;                  
    bool processTick();                   

private:
    bool working;
    int timeLeft;
    Request currRequest;
    std::string logServerMessages;
};


#endif
