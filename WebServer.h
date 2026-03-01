#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Request.h"

/**
 * @file WebServer.h
 * @brief Declaration of the WebServer class that processes Requests.
 */


/**
 * @class WebServer
 * @brief Simulated web server that processes a single Request at a time.
 *
 * A WebServer either has an active Request or is idle.
 * Each call to processTick() advances the current Request by one clock cycle.
 */
class WebServer {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes the server as idle, with zero remaining time and
     * logging of server messages disabled by default.
     */
    WebServer() : working(false), timeLeft(0), logServerMessages("no") {}

    /**
     * @brief Enable or disable verbose server logging.
     *
     * If @p value is exactly "yes", per-server messages will be printed
     * to the console when a request starts and finishes.
     *
     * @param value Logging flag (typically "yes" or "no").
     */
    void setLogServerMessages(const std::string& value);

    /**
     * @brief Assign a new Request to this server.
     *
     * The server becomes busy and @c timeLeft is initialized from
     * Request::neededTime.
     *
     * @param req The Request to start processing.
     */
    void setRequest(const Request& req);  


    /**
     * @brief Query whether the server is currently idle.
     * @return @c true if the server has no active request, @c false otherwise.
     */
    bool isIdle() const;     
    
    
    /**
     * @brief Process one clock tick of work.
     *
     * If the server is working, this decrements @c timeLeft by one.
     * When @c timeLeft reaches zero, the request is considered complete and
     * the server becomes idle again.
     *
     * @return @c true if a request finished on this tick, otherwise @c false.
     */
    bool processTick();                   

private:
    /** @brief Whether this server is currently processing a request. */
    bool working;

    /** @brief Remaining processing time (in clock cycles) for the active request. */
    int timeLeft;

    /** @brief The current Request being processed. */
    Request currRequest;

    /**
     * @brief Flag controlling server log output.
     *
     * If exactly "yes", messages for starting and finishing requests are printed.
     */
    std::string logServerMessages;
};

#endif
