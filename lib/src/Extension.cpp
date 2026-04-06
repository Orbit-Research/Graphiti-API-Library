/*******************************************************************************
 * @file    Extension.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   GraphitiExtension implementation.
 *
 * @details
 * Implements startup/shutdown sequences, key and draw loops, and convenience
 * helpers for the `GraphitiExtension` wrapper around `Graphiti_API` used by
 * example programs and the test harness.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <cstdio>

#include <Graphiti/Extension.hpp>
#include <thread>

/**
 * @function GraphitiExtension
 * @brief Construct a new GraphitiExtension object (default)
 */
GraphitiExtension::GraphitiExtension()
        : Graphiti_API(),
            screen((ROW_COUNT * COLUMN_COUNT) * 2)
{}

/**
 * @function GraphitiExtension
 * @brief Construct a new GraphitiExtension object with an existing connection
 * @param connection Pointer to GraphitiConnection
 */
GraphitiExtension::GraphitiExtension(GraphitiConnection* connection)
        : Graphiti_API(connection),
            screen((ROW_COUNT * COLUMN_COUNT) * 2)
{}

/**
 * @function ~GraphitiExtension
 * @brief Destroy the GraphitiExtension object
 */
GraphitiExtension::~GraphitiExtension() {}

/**
 * @function startUpVCP
 * @brief Start up the extension using a VCP (serial) connection
 * @param port Serial port name
 * @param keyEventsBool Enable key events
 * @param touchEventsBool Enable touch events
 * @return bool True if connection successful
 */
bool GraphitiExtension::startUpVCP(std::string port, bool keyEventsBool, bool touchEventsBool){
    vcpConn = new GraphitiConnectionVCP(port); // Your COM port
    if (!vcpConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return false;
    }

    setConnection(vcpConn);
    
    startUpSequence(keyEventsBool, touchEventsBool);

    return true;
}

/**
 * @function startUpHID
 * @brief Start up the extension using an HID connection
 * @param vendor_id USB vendor ID
 * @param product_id USB product ID
 * @param keyEventsBool Enable key events
 * @param touchEventsBool Enable touch events
 * @return bool True if connection successful
 */
bool GraphitiExtension::startUpHID(uint16_t vendor_id, uint16_t product_id, bool keyEventsBool, bool touchEventsBool){
    hidConn = new GraphitiConnectionHID(vendor_id, product_id); // Your COM port
    if (!hidConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return false;
    }

    setConnection(hidConn);
    
    startUpSequence(keyEventsBool, touchEventsBool);

    return true;
}

/**
 * @function startUpSequence
 * @brief Run startup sequence (enable events, start thread)
 * @param keyEventsBool Enable key events
 * @param touchEventsBool Enable touch events
 */
void GraphitiExtension::startUpSequence(bool keyEventsBool, bool touchEventsBool) {
    startResponseThread();
    sleep(2);

    //Trying touch event first

    if(touchEventsBool) {
        #ifdef DEBUG
            std::cout << "Enabling Touch event" << std::endl;
        #endif 
        setTouchEvent(true);
        sleep(2);
    }

    if(keyEventsBool) {
        #ifdef DEBUG
            std::cout << "Enabling Key event" << std::endl;
        #endif 
        setKeyEvent(keyEventsBool);
        sleep(2);
    }
}

/**
 * @function shutDownVCP
 * @brief Shut down VCP connection and related services
 * @param keyEventsBool Disable key events
 * @param touchEventsBool Disable touch events
 */
void GraphitiExtension::shutDownVCP(bool keyEventsBool, bool touchEventsBool){
    shutDownSequence("VCP", keyEventsBool, touchEventsBool);
}

/**
 * @function shutDownHID
 * @brief Shut down HID connection and related services
 * @param keyEventsBool Disable key events
 * @param touchEventsBool Disable touch events
 */
void GraphitiExtension::shutDownHID(bool keyEventsBool, bool touchEventsBool){
    shutDownSequence("HID", keyEventsBool, touchEventsBool);
}

/**
 * @function shutDownSequence
 * @brief Common shutdown sequence for both VCP and HID
 * @param connection_type "VCP" or "HID"
 * @param keyEventsBool Disable key events
 * @param touchEventsBool Disable touch events
 */
void GraphitiExtension::shutDownSequence(std::string connection_type, 
    bool keyEventsBool, bool touchEventsBool){
    if(touchEventsBool) {
        setTouchEvent(false);
        sleep(2);
    }
    if(keyEventsBool) {
        setKeyEvent(false);
        sleep(2);
    }

    clearDisplay();
    sleep(5);

    stopResponseThread();  

    if(connection_type == "VCP") {
        vcpConn->close();  
        delete vcpConn;
        vcpConn = nullptr;
    } else {
        hidConn->close();  
        delete hidConn;
        hidConn = nullptr;
    }
}

/**
 * @function keyLoop
 * @brief Event loop that polls for key events and dispatches callbacks
 * @param key_func Callback function for key events
 * @param loop_condition Pointer to loop condition flag
 * @param parameters User parameters for callback
 */
void GraphitiExtension::keyLoop(
    std::function<void(const std::set<std::string>&, void*)> key_func, 
    bool *loop_condition,
    void* parameters
) {
    while(*loop_condition) {
        sleep(5);

        std::optional<std::set<std::string>> keyEvent = getNextKeyEvent();

        if(keyEvent.has_value()) {
            //Key funciton effects(ei. display changes)
            key_func(keyEvent.value(), parameters);
        }
    }
}

/**
 * @function drawLoop
 * @brief Event loop that polls for draw events and dispatches callbacks
 * @param draw_func Callback function for draw events
 * @param loop_condition Pointer to loop condition flag
 * @param parameters User parameters for callback
 */
void GraphitiExtension::drawLoop(
    std::function<void(const Graphiti_API::DrawEvent&, void*)> draw_func, 
    bool *loop_condition,
    void* parameters
) {
    while(*loop_condition) {

        #ifdef DEBUG
            std::cout << "getNextDrawEvent" << std::endl;
        #endif 

        std::optional<Graphiti_API::DrawEvent> drawEvent = getNextDrawEvent();

        #ifdef DEBUG
            std::cout << "return from getNextDrawEvent" << std::endl;
        #endif 

        if(drawEvent.has_value()) {
            //Draw event function call when there is value
            #ifdef DEBUG
                std::cout << "getNextDrawEvent has a value" << std::endl;
            #endif 
            draw_func(drawEvent.value(), parameters);
        } else {
             #ifdef DEBUG
                std::cout << "getNextDrawEvent has no value" << std::endl;
            #endif 
        }
    }
}

/**
 * @function index
 * @brief Compute buffer index for a given row and column
 * @param row Row index
 * @param col Column index
 * @return int Buffer index
 */
int GraphitiExtension::index(int row, int col) {
    return ((row * COLUMN_COUNT) + col) * 2;
}

/**
 * @function setPin
 * @brief Set a pin height in the local screen buffer
 * @param row Row index
 * @param col Column index
 * @param height Pin height
 */
void GraphitiExtension::setPin(int row, int col, int height) {
    screen[index(row, col)] = height;
}

/**
 * @function clearScreen
 * @brief Clear the local screen buffer (set all pins to zero)
 */
void GraphitiExtension::clearScreen() {
    for (int i = 0; i < screen.size(); ++i) {
        screen[i] = 0;
    }
}

/**
 * @function sleep
 * @brief Sleep helper (seconds)
 * @param time Number of seconds to sleep
 */
void GraphitiExtension::sleep(int time){
    std::this_thread::sleep_for(std::chrono::seconds(time));
}