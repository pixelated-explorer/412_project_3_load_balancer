#include <cstdlib> 
#include <iostream>
#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(int numServers_, int waitCycles_, int lastTime_) {
    numServers = numServers_; 
    waitCycles = waitCycles_; 
    lastTime = lastTime_; 
}

void LoadBalancer::makeRequests()
{
    int newReqs = std::rand() % 4;  // 0–3 new requests this tick

    for (int i = 0; i < newReqs; ++i) {
        int a1 = std::rand() % 256;
        int a2 = std::rand() % 256;
        int a3 = std::rand() % 256;
        int a4 = std::rand() % 256;

        int b1 = std::rand() % 256;
        int b2 = std::rand() % 256;
        int b3 = std::rand() % 256;
        int b4 = std::rand() % 256;

        std::string ipIn  = std::to_string(a1) + "." + std::to_string(a2) + "." +
                            std::to_string(a3) + "." + std::to_string(a4);
        std::string ipOut = std::to_string(b1) + "." + std::to_string(b2) + "." +
                            std::to_string(b3) + "." + std::to_string(b4);

        int neededTime = (std::rand() % 10) + 1;   // 1–10 cycles
        int jt = std::rand() % 2;
        std::string jobType = (jt == 0) ? "stream" : "process";

        Request req(ipIn, ipOut, neededTime, jobType);
        reqQueue.push(req);
    }
}

void LoadBalancer::makeServers(int numToSpawn) {
    for (int i = 0; i < numToSpawn; ++i) {
        WebServer server; 
        serverList.push_back(server); 
    }

    // update numServers...
    numServers = static_cast<int>(serverList.size());
}

void LoadBalancer::initQueue(int numRequests) {
    for (int i = 0; i < numRequests; ++i) {
        int a1 = std::rand() % 256;
        int a2 = std::rand() % 256;
        int a3 = std::rand() % 256;
        int a4 = std::rand() % 256;

        int b1 = std::rand() % 256;
        int b2 = std::rand() % 256;
        int b3 = std::rand() % 256;
        int b4 = std::rand() % 256;

        std::string ipIn  = std::to_string(a1) + "." + std::to_string(a2) + "." +
                            std::to_string(a3) + "." + std::to_string(a4);

        std::string ipOut = std::to_string(b1) + "." + std::to_string(b2) + "." +
                            std::to_string(b3) + "." + std::to_string(b4);

        // neededTime between 1 and 10 cycles, 
        int neededTime = (std::rand() % 10) + 1;   // 1–10 [web:44]

        // simple random job type: 0 = stream, 1 = process
        int jt = std::rand() % 2;
        std::string jobType = (jt == 0) ? "stream" : "process";

        Request req(ipIn, ipOut, neededTime, jobType);
        reqQueue.push(req);
    }
}

void LoadBalancer::monitorQueue(int time)
{
    int qSize = static_cast<int>(reqQueue.size());
    int lower = 50 * numServers;
    int upper = 80 * numServers;

    int deltaTime = time - lastTime;
    if (deltaTime < waitCycles) {
        return;
    }

    if (qSize < lower && numServers > 1) {
        serverList.pop_back();
        numServers = static_cast<int>(serverList.size());
        lastTime = time;

        std::cout << "[Tick " << time << "] Scaling DOWN: now "
                  << numServers << " servers, queue size " << qSize << "\n";
    }
    else if (qSize > upper) {
        WebServer server;
        serverList.push_back(server);
        numServers = static_cast<int>(serverList.size());
        lastTime = time;

        std::cout << "[Tick " << time << "] Scaling UP: now "
                  << numServers << " servers, queue size " << qSize << "\n";
    }
}

void LoadBalancer::ticking(int time)
{
    // 1) new incoming requests
    makeRequests();

    // 2) assign work to idle servers
    for (int i = 0; i < numServers; ++i) {
        if (serverList[i].isIdle() && !reqQueue.empty()) {
            Request nextReq = reqQueue.front();
            reqQueue.pop();
            serverList[i].setRequest(nextReq);
        }
    }

    // 3) let each server process one tick
    for (int i = 0; i < numServers; ++i) {
        serverList[i].processTick();
    }

    // 4) adjust server count if needed
    monitorQueue(time);
}