#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cctype>
#include "LoadBalancer.h"

/**
 * @file main.cpp
 * @brief Entry point for the load balancer simulation.
 *
 * This program configures a LoadBalancer either interactively or via a
 * simple configuration file, runs the simulation for the requested
 * number of clock cycles, and writes both console output and a log file.
 */

 /**
 * @brief Program entry point.
 *
 * Steps:
 *  - Ask the user for a log file name.
 *  - Ask whether to enable verbose per-server logging.
 *  - Optionally load configuration parameters from a file in the
 *    @c configs/ directory.
 *  - Prompt for any parameters not supplied by the configuration file.
 *  - Create a LoadBalancer, seed the initial request queue, and run the
 *    simulation for the requested number of ticks.
 *  - Print and log a summary of results.
 *
 * @return 0 on success, non-zero on failure to open the log file.
 */
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::string logfile;
    std::cout << "Provide unique log file name: ";
    std::cin >> logfile;
    
    std::string logServerMessages;
    std::string logServer; 
    std::cout << "Do you wish to see server log output to the console? Answer 'yes' or anything else for no: ";
    std::cin >> logServer; 

    if (logServer == "yes") {
        logServerMessages = "yes"; 
    } else {
        logServerMessages = "no";
    }

    std::filesystem::create_directories("logs"); 
    std::ofstream logFile("logs/" + logfile + ".txt");
    if(!logFile) {
        std::cout << "Failed to open log file!\n";
        return 1; 
    }

    int numServers = 0;
    int runTime = 0;
    int waitCycles = 0;
    int maxNewRequestsPerTick = 0;
    int baseProcessTime = 0;
    int arrivalProb = 0; 

    int blockStart = -1; 
    int blockEnd = -1;


    std::string useConfig;
    std::cout << "Load configuration from file? (yes/no): ";
    std::cin >> useConfig;

    if (useConfig == "yes") {
        std::string cfgName;
        std::cout << "Enter config filename: ";
        std::cin >> cfgName;

        
        std::string fullPath = "configs/" + cfgName;
        std::ifstream cfg(fullPath);
        if (!cfg) {
            std::cout << "Could not open config file, falling back to manual input.\n";
        } else {
            std::string line;
            while (std::getline(cfg, line)) {
                if (line.empty()) continue;
                std::istringstream iss(line);
                std::string key, eq;
                if (!(iss >> key >> eq)) continue;
                if (eq != "=") continue;

                if (key == "numServers") iss >> numServers;
                else if (key == "runTime") iss >> runTime;
                else if (key == "waitCycles") iss >> waitCycles;
                else if (key == "arrivalProb") iss >> arrivalProb;
                else if (key == "maxNewRequestsPerTick") iss >> maxNewRequestsPerTick;
                else if (key == "baseProcessTime") iss >> baseProcessTime;
                else if (key == "blockStart") iss >> blockStart;
                else if (key == "blockEnd") iss >> blockEnd;
            }
        }
    }

    if (numServers <= 0) {
        std::cout << "Enter initial number of servers: ";
        std::cin >> numServers;
    }

    if (runTime <= 0) {
        std::cout << "Enter total run time (clock cycles): ";
        std::cin >> runTime;
    }

    if (waitCycles <= 0) {
        std::cout << "Enter wait cycles between scaling actions: ";
        std::cin >> waitCycles;
    }

    if (arrivalProb < 0 || arrivalProb > 100) {
        std::cout << "Enter new request arrival probability (0 to 100 percent) per tick: ";
        std::cin >> arrivalProb;
    } 

    if (maxNewRequestsPerTick <= 0) {
        std::cout << "Enter max new requests per tick: ";
        std::cin >> maxNewRequestsPerTick;
    }

    if (baseProcessTime <= 0) {
        std::cout << "Enter base process time (in cycles) per request: ";
        std::cin >> baseProcessTime;
    }

    if (blockStart < -1) {
        std::cout << "Enter block start time for ip: "; 
        std::cin >> blockStart; 
    }

    if (blockEnd < 0) {
        std::cout << "Enter block end time for ip: ";
        std::cin >> blockEnd; 
    }

    logFile << "===== CONFIGURATION =====\n";
    logFile << "Log server messages: " << logServerMessages << "\n";
    logFile << "Initial servers: " << numServers << "\n";
    logFile << "Run time (ticks): " << runTime << "\n";
    logFile << "Wait cycles between scaling: " << waitCycles << "\n";
    logFile << "Arrival probability (%): " << arrivalProb << "\n";
    logFile << "Max new requests per tick: " << maxNewRequestsPerTick << "\n";
    logFile << "Base process time (cycles): " << baseProcessTime << "\n";
    logFile << "Block start (IP first octet): " << blockStart << "\n";
    logFile << "Block end (IP first octet): " << blockEnd << "\n";
    logFile << "=========================\n\n";

    // start lastTime at 0 so it can scale after waitCycles
    std::cout << "Initializing " << numServers << " servers" << std::endl;
    LoadBalancer lb(numServers, waitCycles, 0, maxNewRequestsPerTick, baseProcessTime, blockStart, blockEnd, arrivalProb, logServerMessages, &logFile);

    lb.makeServers(numServers);
    lb.initQueue(numServers * 100);

    for (int currTime = 0; currTime < runTime; ++currTime) {
        lb.ticking(currTime);
    }
    lb.printResults();
    logFile.close();
    return 0;
}
