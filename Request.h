#ifndef REQUEST_H 
#define REQUEST_H 

#include <string> 



/**
 * @file Request.h
 * @brief Defines the Request struct used to represent a single web request.
 */

/**
 * @struct Request
 * @brief Represents a single web request in the load balancer simulation.
 *
 * A Request contains:
 * - source IP address (ipIn)
 * - destination IP address (ipOut)
 * - processing time (neededTime) in clock cycles
 * - job type (jobType) such as "stream" or "process"
 */
struct Request {
    /** @brief Incoming (client) IP address. */
    std::string ipIn;

    /** @brief Outgoing (server) IP address. */
    std::string ipOut;

    /** @brief Number of clock cycles required to complete this request. */
    int neededTime;

    /** @brief Job type description, e.g., "stream" or "process". */
    std::string jobType;

    /**
     * @brief Default constructor.
     *
     * Initializes @c neededTime to 0. Other fields are left empty.
     */
    Request() : neededTime(0) {}

    /**
     * @brief Construct a fully specified Request.
     * @param in  Incoming IP address (client).
     * @param out Outgoing IP address (target server).
     * @param timeNeeded Required processing time in clock cycles.
     * @param type Job type string such as "stream" or "process".
     */
    Request(const std::string& in, const std::string& out, int timeNeeded, const std::string& type) {
        ipIn = in; 
        ipOut = out; 
        neededTime = timeNeeded;
        jobType = type; 
    }
};

#endif