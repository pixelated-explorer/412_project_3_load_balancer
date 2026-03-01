#ifndef LOADBALANCER_H 
#define LOADBALANCER_H 

#include <string> 
#include <vector>
#include <queue>
#include <fstream>

#include "WebServer.h"
#include "Request.h"

/**
 * @file LoadBalancer.h
 * @brief Declaration of the LoadBalancer class that manages servers and a queue of Requests.
 */


/**
 * @class LoadBalancer
 * @brief Simulates a load balancer managing a pool of WebServer instances.
 *
 * The LoadBalancer maintains a queue of Requests, dynamically adjusts the
 * number of servers based on queue length, and collects statistics such as
 * scale ups/downs, completed requests, and busy server ticks.
 */
class LoadBalancer {
    public: 
        /**
         * @brief Construct a LoadBalancer with initial configuration.
         *
         * @param numServers Initial number of web servers.
         * @param waitCycles Number of cycles to wait between scaling actions.
         * @param lastTime Initial last-scaling timestamp (typically 0).
         * @param maxNewRequestsPerTick Maximum new requests that may arrive per tick.
         * @param baseProcessTime Base processing time added to each Request.
         * @param blockStart First octet of IPs to block (inclusive), or -1 to disable.
         * @param blockEnd Last octet of IPs to block (inclusive).
         * @param requestArrivalPercent Probability (0–100) that new requests appear each tick.
         * @param logServerMessages Flag ("yes"/"no") controlling per-server log output.
         * @param logStream Optional pointer to a log file stream for summary/tick logs.
         */
        LoadBalancer(int numServers, int waitCycles, int lastTime, int maxNewRequestsPerTick, 
            int baseProcessTime, int blockStart, int blockEnd, int requestArrivalPercent, 
            const std::string& logServerMessages, std::ofstream* logStream);

        /**
         * @brief Create additional servers and append them to the server list.
         *
         * Each new WebServer is configured with the current logServerMessages flag.
         *
         * @param numServerSpawn Number of new servers to create.
         */
        void makeServers(int numServerSpawn);


        /**
         * @brief Initialize the request queue with a given number of random Requests.
         *
         * This is used to seed the simulation with an initial backlog of work.
         *
         * @param numRequests Number of Requests to push into the queue.
         */
        void initQueue(int numRequests); 

        /**
         * @brief Monitor the queue and adjust the number of servers if needed.
         *
         * If the queue size falls below 50 * numServers, the load balancer
         * scales down by one server. If the queue size exceeds 80 * numServers,
         * it scales up by one server. A minimum wait period (waitCycles) is
         * enforced between consecutive scaling actions.
         *
         * @param time Current simulation clock tick.
         */
        void monitorQueue(int time); 

        /**
         * @brief Generate new incoming requests for the current tick.
         *
         * With probability @c requestArrivalPercent, a random number of new
         * Requests (0..maxNewRequestsPerTick) are generated and added to
         * the queue, unless they are blocked by the configured IP firewall range.
         */
        void makeRequests();

        /**
         * @brief Perform one simulation tick.
         *
         * Steps per tick:
         *  - Generate new requests.
         *  - Assign requests from the queue to idle servers.
         *  - Advance each server by one tick.
         *  - Record how many servers are busy.
         *  - Periodically log a status line.
         *  - Call monitorQueue() to potentially scale up/down.
         *
         * @param time Current simulation clock tick.
         */
        void ticking(int time);

        /**
         * @brief Print and log a summary of the simulation results.
         *
         * Outputs final number of servers, min/max servers, min/max queue size,
         * number of scale ups/downs, completed and blocked requests, and total
         * busy server ticks.
         */
        void printResults();
        
    private: 
        int waitCycles;              ///< Cycles to wait between scaling actions.
        int lastTime;                ///< Last time a scaling action occurred.
        int numServers;              ///< Current number of servers.
        int maxNewRequestsPerTick;   ///< Maximum number of new requests per tick.
        int baseProcessTime;         ///< Base processing time per request (cycles).
        int blockStart;              ///< Start of blocked IP first-octet range.
        int blockEnd;                ///< End of blocked IP first-octet range.
        int requestArrivalPercent;   ///< Probability (0–100) of new arrivals each tick.
        std::string logServerMessages; ///< Flag for per-server logging.

        /** @brief Queue of pending Requests waiting to be processed. */
        std::queue<Request> reqQueue; 

        /** @brief Vector of active WebServer instances. */    
        std::vector<WebServer> serverList; 
            
        /** @brief Optional log stream for writing tick and summary information. */
        std::ofstream* log;     
        // simple stats
        long long scaleUps        = 0; ///< Total number of scale-up events.
        long long scaleDowns      = 0; ///< Total number of scale-down events.
        long long blockedRequests = 0; ///< Total number of requests blocked by firewall.
        long long completedRequests = 0; ///< Total number of completed requests.
        long long busyServerTicks = 0; ///< Sum of busy servers over all ticks.

        int maxServersSeen = 0; ///< Maximum number of servers seen at any time.
        int minServersSeen = 0; ///< Minimum number of servers seen at any time.
        int maxQueueSeen   = 0; ///< Maximum observed queue size.
        int minQueueSeen   = 0; ///< Minimum observed queue size.
};


#endif 
