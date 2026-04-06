/** ******************************************************************************
 * @file    Connection_VCP.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   VCP (serial) connection implementation.
 *
 * @details
 * Implements the `GraphitiConnectionVCP` class which wraps a serial port
 * using Asio for communicating with the Graphiti device over a virtual
 * COM port.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <Graphiti/Connection/Connection_VCP.hpp>
#include <iostream>
/**
 * @function GraphitiConnectionVCP::GraphitiConnectionVCP
 * @brief Constructs a new GraphitiConnectionVCP object.
 *
 * Initializes the ASIO I/O context and serial port object
 * with the given COM port name.
 *
 * @param port Serial port name (e.g. "COM4" or "/dev/ttyUSB0")
 */
GraphitiConnectionVCP::GraphitiConnectionVCP(const std::string& port)
    : io_(), serial_(io_), port_(port) {
}
/**
 * @function GraphitiConnectionVCP::open
 * @brief Opens the serial (VCP) connection to the Graphiti device.
 *
 * Configures the baud rate and prepares the port for communication.
 *
 * @return bool True if the serial port was successfully opened,
 *              false otherwise
 */
bool GraphitiConnectionVCP::open() {
    try {
        serial_.open(port_);
        serial_.set_option(asio::serial_port_base::baud_rate(115200));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to open port " << port_ << ": " << e.what() << std::endl;
        return false;
    }
}

/**
 * @function GraphitiConnectionVCP::close
 * @brief Closes the serial port if it is open.
 *
 * Safely releases the serial port resource.
 *
 * @return void
 */
void GraphitiConnectionVCP::close() {
    if (serial_.is_open()) {
        serial_.close();
    }
}

/**
 * @function GraphitiConnectionVCP::write
 * @brief Writes raw bytes to the serial port.
 *
 * @param data Byte buffer to be written to the device
 * @return bool True if write operation succeeded,
 *              false if an error occurred
 */
bool GraphitiConnectionVCP::write(const std::vector<unsigned char>& data) {
    try {
        asio::write(serial_, asio::buffer(data));
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Write failed: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @function GraphitiConnectionVCP::read
 * @brief Reads a fixed number of bytes from the serial port.
 *
 * Blocks until the requested number of bytes are read or
 * an error occurs.
 *
 * @param size Number of bytes to read
 * @return std::vector<unsigned char> Buffer containing the
 *         received bytes (empty on error or timeout)
 */
std::vector<unsigned char> GraphitiConnectionVCP::read(size_t size) {
    std::vector<unsigned char> buffer(size);
    try {
        asio::read(serial_, asio::buffer(buffer, size));
    }
    catch (const asio::system_error& e) {
        // Only print errors that aren't expected during shutdown
        if (e.code() != asio::error::operation_aborted) {
            std::cerr << "Read failed: " << e.what() << std::endl;
        }
        buffer.clear();
    }
    catch (const std::exception& e) {
        std::cerr << "Read failed (non-ASIО): " << e.what() << std::endl;
        buffer.clear();
    }
    return buffer;
}
