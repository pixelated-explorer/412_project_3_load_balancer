#ifndef LOADBALANCER_H 
#define LOADBALANCER_H 

#include <string> 
#include <vector>
#include <queue>

// This will error out for now...
#include "WebServer.h"
#include "Request.h"

class LoadBalancer {
    public: 
        LoadBalancer(int numServers, int waitCycles, int lastTime);
        // Functions 
        void makeServers(int numServerSpawn);
        void initQueue(int numRequests); 
        void monitorQueue(int time); 
        void makeRequests();
        void ticking(int time);
        
    private: 
        int waitCycles; 
        int lastTime; 
        int numServers; 
        std::queue<Request> reqQueue; 
        std::vector<WebServer> serverList; 
};


#endif 
