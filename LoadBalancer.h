#ifndef LOADBALANCER_H 
#define LOADBALANCER_H 

#include <string> 
#include <vector>
#include <queue>
#include <fstream>

#include "WebServer.h"
#include "Request.h"

class LoadBalancer {
    public: 
        LoadBalancer(int numServers, int waitCycles, int lastTime, int maxNewRequestsPerTick, 
            int baseProcessTime, int blockStart, int blockEnd, std::ofstream* logStream);
        void makeServers(int numServerSpawn);
        void initQueue(int numRequests); 
        void monitorQueue(int time); 
        void makeRequests();
        void ticking(int time);
        void printResults();
        
    private: 
        int waitCycles; 
        int lastTime; 
        int numServers; 
        int maxNewRequestsPerTick;
        int baseProcessTime;
        int blockStart; 
        int blockEnd;
        std::queue<Request> reqQueue; 
        std::vector<WebServer> serverList; 

        std::ofstream* log;     
        // simple stats
        long long scaleUps = 0;
        long long scaleDowns = 0;
        long long blockedRequests = 0;
        long long completedRequests = 0;
        long long busyServerTicks = 0;
        int maxServersSeen = 0;
        int minServersSeen = 0;
        int maxQueueSeen = 0;
        int minQueueSeen = 0;
};


#endif 
