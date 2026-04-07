/**
  ******************************************************************************
  * @file    Connection_VCP.hpp
  * @company Orbit Research
  * @version v1.0
  * @date    08-12-25
  * @brief   Virtual COM Port (VCP) Connection for Graphiti devices.
  *
  * @details
  * Implements serial communication (VCP). Supports baud, parity and flow-control
  * configuration and provides read/write semantics for serial transport.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2026 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */


#pragma once

#include <vector>

/**
 * @class GraphitiConnection
 * @brief Abstract base class for Graphiti device connections
 *
 * This interface defines a common API for different
 * communication backends such as HID and VCP.
 */
class GraphitiConnection {
public:
    /**
     * @function ~GraphitiConnection
     * @brief Virtual destructor for safe polymorphic deletion
     * @return void
     */
    virtual ~GraphitiConnection() {}

    /**
     * @function open
     * @brief Opens the connection to the device
     * @return true if the connection is opened successfully, false otherwise
     */
    virtual bool open() = 0;

    /**
     * @function close
     * @brief Closes the connection to the device
     * @return void
     */
    virtual void close() = 0;

    /**
     * @function write
     * @brief Writes data to the device
     * @param data Vector containing bytes to be written
     * @return true if write operation succeeds, false otherwise
     */
    virtual bool write(const std::vector<unsigned char>& data) = 0;

    /**
     * @function read
     * @brief Reads data from the device
     * @param size Number of bytes to read
     * @return Vector containing the read bytes
     */
    virtual std::vector<unsigned char> read(size_t size) = 0;
};


