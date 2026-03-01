/**
 * @mainpage Rayan Mohammed: CSCE 412 Project 3 – Load Balancer
 *
 * This project implements a discrete-event simulation of a web load balancer.
 * The system manages a queue of client Requests and a pool of WebServer
 * instances, dynamically scaling the number of servers based on queue length.
 * 
 * Key features:
 * - Random generation of requests with IP addresses, processing times,
 *   and job types (such as streaming vs. processing).
 * - A LoadBalancer class that assigns work to servers and keeps track of time through 'ticks' or clock cycles.
 * - Dynamic scaling of servers to keep the queue between 50 and 80 times the number of servers runing, with configurations for wait cycles between each scaling action.
 * - An IP range blocker that acts as a simple firewall / DoS protection.
 * - Console color output to highlight scaling events and status,
 *   plus logging to a text file including configuration and summary statistics.
 *
 * The documentation pages for the Request, WebServer, and LoadBalancer
 * classes describe how these components interact to simulate a real time load balancer. 
 */