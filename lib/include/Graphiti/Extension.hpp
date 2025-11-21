#pragma once

#include <cstdio>
#include <iostream>
#include <chrono>

#include "API.hpp"

#include <Graphiti/Connection/Connection_VCP.hpp>
#include <Graphiti/Connection/Connection_HID.hpp>

#ifdef _WIN32
  #ifdef BUILDING_GRAPHITI_DLL
    #define GRAPHITI_API __declspec(dllexport)
  #else
    #define GRAPHITI_API __declspec(dllimport)
  #endif
#else
  #define GRAPHITI_API
#endif

/**
 * @brief Class to improve the ease of use of the Graphiti
 * Graphiti_API can be used with or without GraphitExtension
 * GraphitiExtension can also be used as a reference to how
 * the Graphiti_API can be used
 */
class GRAPHITI_API GraphitiExtension : public Graphiti_API {
public:
    /**
     * @brief Construct a new Graphiti Extension object
     * 
     */
    GraphitiExtension();

    /**
     * @brief Construct a new Graphiti Extension object
     * 
     * @param connection Connection to device
     */
    GraphitiExtension(GraphitiConnection* connection);   

    
    /**
     * @brief Destroy the Graphiti Extension:: Graphiti Extension object
     * 
     */
    ~GraphitiExtension();

    /**
     * @brief Starts up VCP connection and calls startUpSequence
     * 
     * @param port port name
     * @param keyEventsBool boolean for key events
     * @param touchEventsBool boolean for touch events
     * @return true 
     * @return false 
     */
    bool startUpVCP(std::string port, bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Starts up HID connection and calls startUpSequence
     * 
     * @param vendor_id 
     * @param product_id 
     * @param keyEventsBool 
     * @param touchEventsBool 
     * @return true 
     * @return false 
     */
    bool startUpHID(uint16_t vendor_id, uint16_t product_id, bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Shuts down VCP connection and turns of events
     * 
     * @param keyEventsBool boolean for key events
     * @param touchEventsBool boolean for touch events
     */
    void shutDownVCP(bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Shuts down VCP connection and turns of events
     * 
     * @param keyEventsBool boolean for key events
     * @param touchEventsBool boolean for touch events
     */
    void shutDownHID(bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Screen indicies of pin heights
     * 
     */
    std::vector<uint8_t> screen;

    /**
     * @brief Key Loop for concurrent key responses
     * 
     * Can be used or be used as a reference
     * 
     * Note: Key cobinaitons that are reserved are: (5 + 6 + 8), (8 + Down), (7 + 8)
     * 
     * @param key_func function to use keys
     * @param loop_condition condition for looping
     * @param parameters pointer to other variables used by function
     */
    void keyLoop(
        std::function<void(const std::set<std::string>&, void*)> key_func, 
        bool *loop_condition,
        void* parameters = nullptr
    );

    /**
     * @brief Draw Loop for concurrent draw events
     * 
     * @param draw_func function using draw input
     * @param loop_condition condition for looping
     * @param parameters pointer to other variables used by function
     */
    void drawLoop(
        std::function<void(const Graphiti_API::DrawEvent&, void*)> draw_func,
        bool *loop_condition,
        void* parameters = nullptr
    );

    /**
     * @brief Set the Pin on screen
     * 
     * @param row row
     * @param col column
     * @param height pin heights 0-4
     */
    void setPin(int row, int col, int height);

    /**
     * @brief Gets index of pin on Graphiti
     * 
     * @param row row
     * @param col column
     * @return int index on Graphiti screen
     */
    int index(int row, int col);

    /**
     * @brief Clears Screen display data in GraphitiExtension
     * 
     */
    void clearScreen();

    /**
     * @brief Shorted sleep call
     * Can copy it and make it locally so you can call sleep(2);
     * rather than "graphitiExtension->sleep(2);" for simlpicity
     * 
     * @param time time seconds
     */
    void sleep(int time);

private:
    
    /**
     * @brief VCP connection
     * 
     * Graphiti VCP conneciton object
     */
    GraphitiConnectionVCP* vcpConn = nullptr;

    /**
     * @brief HID connection
     * 
     * Graphiti HID conneciton object
     */
    GraphitiConnectionHID* hidConn = nullptr;

    /**
     * @brief Performs the startup sequence for different conneciton types
     * 
     * @param keyEventsBool 
     * @param touchEventsBool 
     * @return true success
     * @return false failure
     */
    void startUpSequence(bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Performs the shutdown sequence for different conneciton types
     * 
     * @param connection_type 
     * @param keyEventsBool 
     * @param touchEventsBool 
     */
    void shutDownSequence(std::string connection_type, bool keyEventsBool, bool touchEventsBool);
};