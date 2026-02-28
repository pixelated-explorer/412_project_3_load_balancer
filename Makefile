CXX     = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

OBJS = main.o LoadBalancer.o WebServer.o

all: loadbalancer

loadbalancer: $(OBJS)
	$(CXX) $(CXXFLAGS) -o loadbalancer $(OBJS)

main.o: main.cpp LoadBalancer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

WebServer.o: WebServer.cpp WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c WebServer.cpp

clean:
	rm -f *.o loadbalancer
