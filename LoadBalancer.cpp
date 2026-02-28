#include <cstdlib> 
#include <iostream>
#include "Colors.h"
#include "LoadBalancer.h"

LoadBalancer::LoadBalancer(int numServers_, int waitCycles_, int lastTime_, 
    int maxNewRequestsPerTick_, int baseProcessTime_, 
    int blockStart_, int blockEnd_, int requestArrivalPercent_, const std::string& logServerMessages_, std::ofstream* logStream) {

    numServers = numServers_; 
    waitCycles = waitCycles_; 
    lastTime = lastTime_; 
    maxNewRequestsPerTick = maxNewRequestsPerTick_;
    baseProcessTime = baseProcessTime_;

    blockStart = blockStart_;
    blockEnd = blockEnd_; 

    requestArrivalPercent = requestArrivalPercent_;

    logServerMessages = logServerMessages_;

    maxServersSeen = minServersSeen = numServers;
    maxQueueSeen = minQueueSeen = 0;
    log = logStream;
}

void LoadBalancer::makeRequests()
{
    // 1) new incoming requests
    int roll = std::rand() % 100;
    if (roll >= requestArrivalPercent) {
        return;  // no new requests this tick
    }

    int newReqs = std::rand() % (maxNewRequestsPerTick + 1);

    for (int i = 0; i < newReqs; ++i) {
        int a1 = std::rand() % 256;
        int a2 = std::rand() % 256;
        int a3 = std::rand() % 256;
        int a4 = std::rand() % 256;

        bool blocked = false;
        if (blockStart >= 0) {  // -1 means disabled
            if (a1 >= blockStart && a1 <= blockEnd) {
                blocked = true;
            }
        }

        if (blocked) {
            blockedRequests++;
            // skip this request, treat as firewall/DOS protection
            continue;
        }

        int b1 = std::rand() % 256;
        int b2 = std::rand() % 256;
        int b3 = std::rand() % 256;
        int b4 = std::rand() % 256;

        std::string ipIn  = std::to_string(a1) + "." + std::to_string(a2) + "." +
                            std::to_string(a3) + "." + std::to_string(a4);
        std::string ipOut = std::to_string(b1) + "." + std::to_string(b2) + "." +
                            std::to_string(b3) + "." + std::to_string(b4);

        // Variation on top of base processing time (maybe remove if just want processing time to be constant?)
        int variation = std::rand() % 5;          // 0–4
        int neededTime = baseProcessTime + variation;
        if (neededTime < 1) neededTime = 1; 
        int jt = std::rand() % 2;
        std::string jobType = (jt == 0) ? "stream" : "process";

        Request req(ipIn, ipOut, neededTime, jobType);
        reqQueue.push(req);
    }
}

void LoadBalancer::makeServers(int numToSpawn) {
    for (int i = 0; i < numToSpawn; ++i) {
        WebServer server; 
        server.setLogServerMessages(logServerMessages);
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

        bool blocked = false;
        if (blockStart >= 0) {  // -1 means disabled
            if (a1 >= blockStart && a1 <= blockEnd) {
                blocked = true;
            }
        }

        if (blocked) {
            blockedRequests++;
            // skip this request, treat as firewall/DOS protection
            continue;
        }

        int b1 = std::rand() % 256;
        int b2 = std::rand() % 256;
        int b3 = std::rand() % 256;
        int b4 = std::rand() % 256;

        std::string ipIn  = std::to_string(a1) + "." + std::to_string(a2) + "." +
                            std::to_string(a3) + "." + std::to_string(a4);

        std::string ipOut = std::to_string(b1) + "." + std::to_string(b2) + "." +
                            std::to_string(b3) + "." + std::to_string(b4);

        // neededTime between 1 and 10 cycles, 
        int variation = std::rand() % 5;          // 0–4
        int neededTime = baseProcessTime + variation;
        if (neededTime < 1) neededTime = 1; 

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

    if (qSize > maxQueueSeen) maxQueueSeen = qSize;
    if (minQueueSeen == 0 || qSize < minQueueSeen) minQueueSeen = qSize;

    int deltaTime = time - lastTime;
    if (deltaTime < waitCycles) {
        return;
    }

    if (qSize < lower && numServers > 1) {
        serverList.pop_back();
        numServers = static_cast<int>(serverList.size());
        lastTime = time;

        scaleDowns++;

        std::cout << COLOR_YELLOW << "[Tick " << time << "] Scaling DOWN: now "
                  << numServers << " servers, queue size " << qSize << COLOR_RESET << "\n";

        if (log) {
            (*log) << "TICK " << time << ": SCALE_DOWN to "
                << numServers << " servers, queue=" << qSize << "\n";
        }
    }
    else if (qSize > upper) {
        WebServer server;
        serverList.push_back(server);
        numServers = static_cast<int>(serverList.size());
        lastTime = time;

        scaleUps++; 

        std::cout << COLOR_YELLOW << "[Tick " << time << "] Scaling UP: now "
                  << numServers << " servers, queue size " << qSize << COLOR_RESET << "\n";

        if (log) {
            (*log) << "TICK " << time << ": SCALE_UP to "
                   << numServers << " servers, queue=" << qSize << "\n";
        }

    }

    if (numServers > maxServersSeen) maxServersSeen = numServers;
    if (numServers < minServersSeen) minServersSeen = numServers;

}

void LoadBalancer::ticking(int time)
{   
    makeRequests();

    int busyNow = 0; 

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
        if (!serverList[i].isIdle()) busyNow++;
        if (serverList[i].processTick()) {
            completedRequests++;                     
        }
    }

    busyServerTicks += busyNow;      

    if (numServers > maxServersSeen) maxServersSeen = numServers;
    if (numServers < minServersSeen) minServersSeen = numServers;

    // log every 100 ticks 

    if (time % 100 == 0) {
        std::cout << COLOR_RED << "TICK " << time << ": servers=" << numServers
               << ", busy=" << busyNow
               << ", queue=" << reqQueue.size() << COLOR_RESET << "\n";
    }

    if (log && time % 100 == 0) {
        (*log) << "TICK " << time << ": servers=" << numServers
               << ", busy=" << busyNow
               << ", queue=" << reqQueue.size() << "\n";
    }

    // 4) adjust server count if needed
    monitorQueue(time);
}

void LoadBalancer::printResults()
{

    std::cout << "===== SUMMARY =====\n";
    std::cout << "Final servers: " << numServers << "\n";
    std::cout << "Min servers: " << minServersSeen
              << ", Max servers: " << maxServersSeen << "\n";
    std::cout << "Min queue: " << minQueueSeen
              << ", Max queue: " << maxQueueSeen << "\n";
    std::cout << "Scale ups: " << scaleUps
              << ", Scale downs: " << scaleDowns << "\n";
    std::cout << "Completed requests: " << completedRequests << "\n";
    std::cout << "Blocked requests (firewall): " << blockedRequests << "\n";
    std::cout << "Busy server ticks: " << busyServerTicks << "\n";


    if (!log) return;

    (*log) << "===== SUMMARY =====\n";
    (*log) << "Final servers: " << numServers << "\n";
    (*log) << "Min servers: " << minServersSeen
           << ", Max servers: " << maxServersSeen << "\n";
    (*log) << "Min queue: " << minQueueSeen
           << ", Max queue: " << maxQueueSeen << "\n";
    (*log) << "Scale ups: " << scaleUps
           << ", Scale downs: " << scaleDowns << "\n";
    (*log) << "Completed requests: " << completedRequests << "\n";
    (*log) << "Blocked requests (firewall): " << blockedRequests << "\n";
    (*log) << "Busy server ticks: " << busyServerTicks << "\n";
}
