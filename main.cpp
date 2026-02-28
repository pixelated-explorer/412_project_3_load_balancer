#include <iostream>
#include <cstdlib>
#include <ctime>

#include "LoadBalancer.h"

int main() {
    // 1) Seed random once
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2) Get configuration from user
    int numServers;
    int runTime;
    int waitCycles;

    std::cout << "Enter initial number of servers: ";
    std::cin >> numServers;

    std::cout << "Enter total run time (clock cycles): ";
    std::cin >> runTime;

    std::cout << "Enter wait cycles between scaling actions: ";
    std::cin >> waitCycles;

    // 3) Create load balancer
    // start lastTime at 0 so it can scale after waitCycles
    std::cout << "Initializing " << numServers << " servers" << std::endl;
    LoadBalancer lb(numServers, waitCycles, 0);

    // 4) Create initial servers and fill queue (servers * 100)
    lb.makeServers(numServers);
    lb.initQueue(numServers * 100);

    // 5) Main simulation loop
    for (int currTime = 0; currTime < runTime; ++currTime) {
        lb.ticking(currTime);
        // later: logging, colored output, firewall checks, etc.
    }

    return 0;
}
