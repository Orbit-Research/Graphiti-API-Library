/**
  ******************************************************************************
  * @file    lib/include/Connection/Connection_HID.hpp
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


#ifndef GRAPHITI_CONNECTION_VCP_HPP
#define GRAPHITI_CONNECTION_VCP_HPP

#include <string>
#define ASIO_STANDALONE
#include <asio.hpp>
using namespace asio;
#include "Connection.hpp"

/**
 * @class GraphitiConnectionVCP
 * @brief Virtual COM Port (VCP) based Graphiti connection implementation
 */
class GraphitiConnectionVCP : public GraphitiConnection {
public:
    /**
     * @function GraphitiConnectionVCP
     * @brief Constructs a VCP connection using a serial port name
     * @param port Serial port identifier (e.g. COM3, /dev/ttyUSB0)
     */
    explicit GraphitiConnectionVCP(const std::string& port);

    /**
     * @function open
     * @brief Opens the serial port connection
     * @return true if the port is opened successfully, false otherwise
     */
    bool open();

    /**
     * @function close
     * @brief Closes the serial port connection
     * @return void
     */
    void close();

    /**
     * @function write
     * @brief Writes data to the serial port
     * @param data Vector containing data to be written
     * @return true if write operation succeeds, false otherwise
     */
    bool write(const std::vector<unsigned char>& data);

    /**
     * @function read
     * @brief Reads data from the serial port
     * @param size Number of bytes to read
     * @return Vector containing the read bytes
     */
    std::vector<unsigned char> read(size_t size);

private:
    /** ASIO IO context */
    asio::io_context io_;

    /** Serial port instance */
    asio::serial_port serial_;

    /** Serial port name */
    std::string port_;
};

#endif // GRAPHITI_CONNECTION_VCP_HPP
