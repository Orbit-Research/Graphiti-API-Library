/** ******************************************************************************
 * @file    main.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   Runtime demo using Graphiti_API over VCP/HID.
 *
 * @details
 * Demonstrates connecting to a Graphiti device (VCP or HID), updating the
 * display buffer, reading output/key events and running a small display loop.
 * Useful as a simple example for integrating `Graphiti_API` in an application.
 *
 *******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include <Graphiti/API.hpp>
#include <Graphiti/Connection/Connection_VCP.hpp>
#include <Graphiti/Connection/Connection_HID.hpp>

std::vector<uint8_t> scr((ROW_COUNT * COLUMN_COUNT) * 2);
GraphitiConnectionVCP* vcpConn = nullptr;
GraphitiConnectionHID* hidConn = nullptr;
Graphiti_API* graphiti = nullptr;

/**
 * @Function index
 * @brief Calculate the buffer index for a given row and column.
 * @param row Row index.
 * @param col Column index.
 * @return int Buffer index.
 */
int index(int row, int col) {
    return ((row * COLUMN_COUNT) + col) * 2;
}


/**
 * @Function setPin
 * @brief Set a pin height in the shared screen buffer.
 * @param row Row index.
 * @param col Column index.
 * @param height Height value.
 * @return void
 */
void setPin(int row, int col, int height) {
    scr[index(row, col)] = height;
}


/**
 * @Function clearBoard
 * @brief Clear the global screen buffer (set all entries to zero).
 * @return void
 */
void clearBoard() {
    for (int i = 0; i < scr.size(); ++i) {
        scr[i] = 0;
    }
}


/**
 * @Function RunDisplay
 * @brief Example display loop that periodically updates the screen and polls events.
 * @return void
 */
void RunDisplay() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        setPin(20, 30, 4);

        graphiti->updateDisplay(scr);

        graphiti->getSoftwareVersion();

        auto output = graphiti->getNextOutputEvent();
        if(output.has_value()) {
            std::cout << output.value() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto keyEventOption = graphiti->getNextKeyEvent();

        std::set<std::string> keyEvent;

        if(keyEventOption.has_value()) {
            keyEvent = keyEventOption.value();
        }

        if(keyEvent.contains("1")){
            std::cout << "Key Event 1 on Main found" << std::endl;
        }
    }
}


/**
 * @Function vcpTest
 * @brief Initialize VCP connection and Graphiti_API wrapper.
 * @return int 0 on success, 1 on failure.
 */
int vcpTest() {
    vcpConn = new GraphitiConnectionVCP("COM4"); // Your COM port
    if (!vcpConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return 1;
    }
    graphiti = new Graphiti_API(vcpConn);
    return 0;
}


/**
 * @Function hidTest
 * @brief Initialize HID connection and Graphiti_API wrapper.
 * @return int 0 on success, 1 on failure.
 */
int hidTest() {
    // Select device VID/PID for HID connection
    bool useOldDevice = false; // Set true for old device, false for new device
    uint16_t vid, pid;
    if (useOldDevice) {
        vid = 0x1FC9; pid = 0x8217;
    } else {
        vid = 0x0483; pid = 0xA366;
    }
    hidConn = new GraphitiConnectionHID(vid, pid);
    if (!hidConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return 1;
    }
    graphiti = new Graphiti_API(hidConn);
    return 0;
}


/**
 * @Function main
 * @brief Main entry point for the runtime demo.
 * @return int Exit code.
 */
int main()
{
    if (vcpTest() == 1) {
        return 1;
    }

    graphiti->startResponseThread();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    graphiti->clearDisplay();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    graphiti->setKeyEvent(true);

    RunDisplay();

    delete graphiti;
    delete vcpConn;

    return 0;
}