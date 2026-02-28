#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <filesystem>

#include "LoadBalancer.h"

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    std::string logfile;
    std::cout << "Provide unique log file name: ";
    std::cin >> logfile;
    
    std::filesystem::create_directories("logs"); 
    std::ofstream logFile("logs/" + logfile + ".txt");
    if(!logFile) {
        std::cout << "Failed to open log file!\n";
        return 1; 
    }

    int numServers;
    int runTime;
    int waitCycles;
    int maxNewRequestsPerTick;
    int baseProcessTime;

    int blockStart; 
    int blockEnd;

    std::cout << "Enter initial number of servers: ";
    std::cin >> numServers;

    std::cout << "Enter total run time (clock cycles): ";
    std::cin >> runTime;

    std::cout << "Enter wait cycles between scaling actions: ";
    std::cin >> waitCycles;

    std::cout << "Enter max new requests per tick: ";
    std::cin >> maxNewRequestsPerTick;

    std::cout << "Enter base process time (in cycles) per request: ";
    std::cin >> baseProcessTime;

    std::cout << "Enter block start time for ip: "; 
    std::cin >> blockStart; 

    std::cout << "Enter block end time for ip: ";
    std::cin >> blockEnd; 

    // start lastTime at 0 so it can scale after waitCycles
    std::cout << "Initializing " << numServers << " servers" << std::endl;
    LoadBalancer lb(numServers, waitCycles, 0, maxNewRequestsPerTick, baseProcessTime, blockStart, blockEnd, &logFile);

    lb.makeServers(numServers);
    lb.initQueue(numServers * 100);

    for (int currTime = 0; currTime < runTime; ++currTime) {
        lb.ticking(currTime);
    }
    lb.printResults();
    logFile.close();
    return 0;
}
