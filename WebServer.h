#ifndef WEBSERVER_H 
#define WEBSERVER_H 

#include "Request.h"

class WebServer {
    public: 
        WebServer() : working(false), timeLeft(0) {}
        void processRequest(const Request& req);
        void setRequest(const Request& req); 
        bool setReady(); 

    private: 
        bool working; 
        int timeLeft; 
        Request currRequest; 
};


#endif 