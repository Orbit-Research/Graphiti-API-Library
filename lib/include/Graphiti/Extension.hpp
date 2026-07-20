/** ******************************************************************************
 * @file    Extension.hpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   GraphitiExtension class declaration.
 *
 * @details
 * Declares `GraphitiExtension`, a convenience wrapper around `Graphiti_API`
 * providing startup/shutdown helpers, key/draw loops and a local screen buffer
 * for higher-level usage patterns and example code.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#pragma once

#include <cstdio>
#include <iostream>
#include <chrono>

#include "API.hpp"

#include <Graphiti/Connection/Connection_VCP.hpp>
#include <Graphiti/Connection/Connection_HID.hpp>

#ifdef _WIN32
  #ifdef BUILDING_GRAPHITI_DLL
    /** Windows export macros handled by build defines */
  #else
    /** Windows import macros handled by client code */
  #endif
#else
  #define GRAPHITI_API
#endif

/**
 * @class GraphitiExtension
 * @brief High-level helper class for simplified Graphiti device usage
 *
 * Provides convenience wrappers around Graphiti_API for
 * VCP and HID based connections.
 */
class GRAPHITI_API GraphitiExtension : public Graphiti_API {
public:
    /**
     * @function GraphitiExtension
     * @brief Constructs GraphitiExtension using an existing connection
     * @param connection Pointer to an initialized GraphitiConnection
     */
    GraphitiExtension(GraphitiConnection* connection);

    /**
     * @function GraphitiExtension
     * @brief Default constructor
     */
    GraphitiExtension();

    /**
     * @function ~GraphitiExtension
     * @brief Destroys the GraphitiExtension object and releases resources
     * @return void
     */
    ~GraphitiExtension();

    /**
     * @function startUpVCP
     * @brief Initializes VCP connection and executes startup sequence
     * @param port Serial port name
     * @param keyEventsBool Enable key events
     * @param touchEventsBool Enable touch events
     * @return true on successful startup
     * @return false on failure
     */
    bool startUpVCP(std::string port, bool keyEventsBool, bool touchEventsBool);

    /**
     * @function startUpHID
     * @brief Initializes HID connection and executes startup sequence
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     * @param keyEventsBool Enable key events
     * @param touchEventsBool Enable touch events
     * @return true on successful startup
     * @return false on failure
     */
    bool startUpHID(uint16_t vendor_id, uint16_t product_id,
                    bool keyEventsBool, bool touchEventsBool);

    /**
     * @function shutDownVCP
     * @brief Shuts down VCP connection and disables events
     * @param keyEventsBool Disable key events
     * @param touchEventsBool Disable touch events
     * @return void
     */
    void shutDownVCP(bool keyEventsBool, bool touchEventsBool);

    /**
     * @function shutDownHID
     * @brief Shuts down HID connection and disables events
     * @param keyEventsBool Disable key events
     * @param touchEventsBool Disable touch events
     * @return void
     */
    void shutDownHID(bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Screen indices storing pin heights
     */
    std::vector<uint8_t> screen;

    /**
     * @function keyLoop
     * @brief Event loop for concurrent key handling
     *
     * Reserved key combinations: (5 + 6 + 8), (8 + Down), (7 + 8)
     *
     * @param key_func Callback handling key sets
     * @param loop_condition Pointer controlling loop execution
     * @param @param Optional user-defined @param
     * @return void
     */
    void keyLoop(
        std::function<void(const std::set<std::string>&, void*)> key_func,
        bool* loop_condition,
        void* parameters = nullptr
    );

    /**
     * @function drawLoop
     * @brief Event loop for concurrent draw handling
     * @param draw_func Callback handling draw events
     * @param loop_condition Pointer controlling loop execution
     * @param @param Optional user-defined 
     * @return void
     */
    void drawLoop(
        std::function<void(const Graphiti_API::DrawEvent&, void*)> draw_func,
        bool* loop_condition,
        void* parameters = nullptr
    );

    /**
     * @function setPin
     * @brief Sets the height of a pin on the Graphiti screen
     * @param row Screen row index
     * @param col Screen column index
     * @param height Pin height (0–4)
     * @return void
     */
    void setPin(int row, int col, int height);

    /**
     * @function index
     * @brief Calculates linear index of a pin on the screen
     * @param row Screen row index
     * @param col Screen column index
     * @return int Linear screen index
     */
    int index(int row, int col);

    /**
     * @function clearScreen
     * @brief Clears the Graphiti screen
     * @return void
     */
    void clearScreen();

    /**
     * @function sleep
     * @brief Wrapper sleep function for convenience
     * @param time Sleep duration in seconds
     * @return void
     */
    void sleep(int time);

private:
    /**
     * @brief VCP connection object
     */
    GraphitiConnectionVCP* vcpConn = nullptr;

    /**
     * @brief HID connection object
     */
    GraphitiConnectionHID* hidConn = nullptr;

    /**
     * @function startUpSequence
     * @brief Executes common startup sequence logic
     * @param keyEventsBool Enable key events
     * @param touchEventsBool Enable touch events
     * @return void
     */
    void startUpSequence(bool keyEventsBool, bool touchEventsBool);

    /**
     * @function shutDownSequence
     * @brief Executes common shutdown sequence logic
     * @param connection_type Connection identifier string
     * @param keyEventsBool Disable key events
     * @param touchEventsBool Disable touch events
     * @return void
     */
    void shutDownSequence(std::string connection_type,
                          bool keyEventsBool,
                          bool touchEventsBool);
};
