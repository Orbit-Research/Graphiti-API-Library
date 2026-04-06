/**
  ******************************************************************************
  * @file    Connection_HID.hpp
  * @company Orbit Research
  * @version v1.0
  * @date    08-12-25
  * @brief   HID-based Connection implementation for Graphiti.
  *
  * @details
  * Implements device communication using the system HID API (e.g. hidapi).
  * Handles enumeration, open/close and IN/OUT transfers for Graphiti devices.
  *
  ******************************************************************************
  * @attention
  * Copyright (c) 2026 Orbit Research LLC
  * Distributed under GNU General Public License V3.0
  ******************************************************************************
  */



#ifndef GRAPHITI_CONNECTION_HID_HPP
#define GRAPHITI_CONNECTION_HID_HPP

#include <string>
#include <hidapi.h>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <deque>
#include "Connection.hpp"

/**
 * @class GraphitiConnectionHID
 * @brief HID-based Graphiti connection implementation
 */
class GraphitiConnectionHID : public GraphitiConnection {
public:
    /**
     * @function GraphitiConnectionHID
     * @brief Constructs a HID connection using vendor and product IDs
     * @param vendor_id USB vendor ID
     * @param product_id USB product ID
     */
    explicit GraphitiConnectionHID(const uint16_t vendor_id,
                                   const uint16_t product_id);

    /**
     * @function open
     * @brief Opens the HID device connection
     * @return true if connection is successfully opened, false otherwise
     */
    bool open();

    /**
     * @function close
     * @brief Closes the HID device connection
     * @return void
     */
    void close();

    /**
     * @function write
     * @brief Writes data to the HID device
     * @param data Vector containing data to be written
     * @return true if write operation succeeds, false otherwise
     */
    bool write(const std::vector<unsigned char>& data);

    /**
     * @function read
     * @brief Reads data from the HID device
     * @param size Number of bytes to read
     * @return Vector containing the read bytes
     */
    std::vector<unsigned char> read(size_t size);

private:
    const int vendor_id_;
    const int product_id_;
    hid_device* device_;
    std::deque<uint8_t> byteBuffer_;

    /**
     * @function writeAndErrorcheck
     * @brief Writes a HID report and performs error checking
     * @param report HID report data
     * @return true if report is written successfully, false otherwise
     */
    bool writeAndErrorcheck(const std::vector<unsigned char>& report);

    /**
     * @function writeReportID
     * @brief Generates and returns report ID for outgoing data
     * @param data Payload data
     * @return Generated report ID
     */
    unsigned char writeReportID(const std::vector<unsigned char>& data);

    /**
     * @function readToBuffer
     * @brief Reads data from HID device and stores it in internal buffer
     * @return void
     */
    void readToBuffer();
};

#endif // GRAPHITI_CONNECTION_HID_HPP
