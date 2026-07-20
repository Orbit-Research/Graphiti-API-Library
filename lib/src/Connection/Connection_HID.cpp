/** ******************************************************************************
 * @file    Connection_HID.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   HID connection implementation.
 *
 * @details
 * Implements the `GraphitiConnectionHID` class which communicates with the
 * Graphiti device using HID APIs on supported platforms.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <Graphiti/Connection/Connection_HID.hpp>
#include <iostream>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <cstring>
#include <algorithm>
#include <iomanip>

/**
 * @function GraphitiConnectionHID::GraphitiConnectionHID
 * @brief Constructs a new GraphitiConnectionHID object.
 *
 * Initializes the HID connection parameters but does not open
 * the device.
 *
 * @param vendor_id USB vendor ID of the Graphiti device
 * @param product_id USB product ID of the Graphiti device
 */
GraphitiConnectionHID::GraphitiConnectionHID(const uint16_t vendor_id, const uint16_t product_id)
    : vendor_id_(vendor_id), product_id_(product_id), device_(nullptr) {
}

/**
 * @function GraphitiConnectionHID::open
 * @brief Opens the HID device and prepares it for I/O operations.
 *
 * Enumerates HID devices matching the vendor and product IDs,
 * selects interface 01, opens the device, and configures it
 * for non-blocking operation.
 *
 * @return bool True if the device was successfully opened, otherwise false
 */
bool GraphitiConnectionHID::open() {
    if (device_) return true; // Already open
    // Try new connection first
    struct hid_device_info* devs_new = hid_enumerate(0x0483, 0xA366);
    struct hid_device_info* cur_new = devs_new;
    std::string chosen_path_str_new;
    while (cur_new) {
        if ((cur_new->interface_number == 1) ||
            (cur_new->path && strstr(cur_new->path, "mi_01") != nullptr)) {
            chosen_path_str_new = cur_new->path;
            break;
        }
        cur_new = cur_new->next;
    }
    if (!chosen_path_str_new.empty()) {
        std::cerr << "Trying new connection (VID: 0x0483, PID: 0xA366)..." << std::endl;
        device_ = hid_open_path(chosen_path_str_new.c_str());
        if (devs_new) hid_free_enumeration(devs_new);
        if (device_) {
            hid_set_nonblocking(device_, 1);
            return true;
        } else {
            std::cerr << "Failed to open HID device path: " << chosen_path_str_new << std::endl;
        }
    } else {
        if (devs_new) hid_free_enumeration(devs_new);
        std::cerr << "New connection not found. Trying old connection (VID: 0x1FC9, PID: 0x8217)..." << std::endl;
        // Try old connection
        struct hid_device_info* devs_old = hid_enumerate(0x1FC9, 0x8217);
        struct hid_device_info* cur_old = devs_old;
        std::string chosen_path_str_old;
        while (cur_old) {
            if (cur_old->path) {
                chosen_path_str_old = cur_old->path;
                break;
            }
            cur_old = cur_old->next;
        }
        if (!chosen_path_str_old.empty()) {
            std::cerr << "Trying old connection (VID: 0x1FC9, PID: 0x8217)..." << std::endl;
            device_ = hid_open_path(chosen_path_str_old.c_str());
            if (devs_old) hid_free_enumeration(devs_old);
            if (device_) {
                hid_set_nonblocking(device_, 1);
                return true;
            } else {
                std::cerr << "Failed to open HID device path: " << chosen_path_str_old << std::endl;
            }
        } else {
            if (devs_old) hid_free_enumeration(devs_old);
            std::cerr << "Old connection not found." << std::endl;
        }
    }
    std::cerr << "Failed to find HID device (tried both new and old connections)" << std::endl;
    return false;
}

/**
 * @function GraphitiConnectionHID::close
 * @brief Closes the HID device and releases associated resources.
 *
 * Safe to call multiple times.
 *
 * @return void
 */
void GraphitiConnectionHID::close() {
    /**
     * @function: GraphitiConnectionHID::close
     * @brief: Close HID device handle if open.
     */
    if (device_) {
        hid_close(device_);
        device_ = nullptr;
    }
}

/**
 * @function GraphitiConnectionHID::write
 * @brief Writes data to the HID device.
 *
 * Automatically fragments large payloads into multiple HID
 * reports when required.
 *
 * @param data Command buffer to send
 * @return bool True if all data was written successfully, otherwise false
 */
bool GraphitiConnectionHID::write(const std::vector<unsigned char>& data) {
    /**
     * @function: GraphitiConnectionHID::write
     * @brief: Send data to HID device, splitting into reports when necessary.
     */
    if (!device_) {
        std::cerr << "Write failed: Device not open" << std::endl;
        return false;
    }

    const size_t REPORT_SIZE = 4864;
    unsigned char REPORT_ID = writeReportID(data);

    std::vector<unsigned char> report;

    bool result = true;

    if(data[1] == 0x2F || data[1] == 0x30) {
        size_t offset = 0;
        while (offset < data.size()) {
            size_t chunk_len = std::min(REPORT_SIZE - 1, data.size() - offset);

            // Allocate buffer for Report ID + chunk
            std::vector<unsigned char> report(REPORT_SIZE, 0);
            report[0] = REPORT_ID; // first byte is the Report ID
            std::copy(data.begin() + offset, data.begin() + offset + chunk_len, report.begin() + 1);

            if(!writeAndErrorcheck(report)) {
                return false;
            }

            offset += chunk_len;
        }
    } else {
        report.reserve(data.size() + 1);
        report.push_back(REPORT_ID);
        report.insert(report.end(), data.begin(), data.end());

        result = writeAndErrorcheck(report);
    }

    return result;
}

/**
 * @function GraphitiConnectionHID::writeAndErrorcheck
 * @brief Writes a single HID report and checks for errors.
 *
 * Converts any wide-character HID error message into UTF-8
 * and logs it.
 *
 * @param report HID report buffer
 * @return bool True if write succeeded, otherwise false
 */
bool GraphitiConnectionHID::writeAndErrorcheck (const std::vector<unsigned char>& report) {
    int result = hid_write(device_, report.data(), report.size());
    if (result < 0) {
        const wchar_t* error_message = hid_error(device_);
        std::string error_string;
        if (error_message) {
            int size_needed = WideCharToMultiByte(CP_UTF8, 0, error_message, -1, NULL, 0, NULL, NULL);
            if (size_needed > 0) {
                error_string.assign(size_needed, 0);
                WideCharToMultiByte(CP_UTF8, 0, error_message, -1, &error_string[0], size_needed, NULL, NULL);
                // strip trailing null added by WideCharToMultiByte
                if (!error_string.empty() && error_string.back() == '\0') error_string.pop_back();
            }
        }
        if (error_string.empty()) error_string = "(unknown HID error)";
        std::cerr << "Write failed: " << error_string << std::endl;
        return false;
    }
    return true;
}

/**
 * @function GraphitiConnectionHID::writeReportID
 * @brief Determines the HID report ID for a given command buffer.
 *
 * @param data Command buffer
 * @return unsigned char Selected HID report ID
 */
unsigned char GraphitiConnectionHID::writeReportID (const std::vector<unsigned char>& data) {
    std::unordered_set<uint8_t> cmdID_List = {0x15, 0x18, 0x19, 0x2F, 0x30};

    if (cmdID_List.count(data[1])) {
        return 0x03;
    }
    return 0x02;
}


/**
 * @function GraphitiConnectionHID::read
 * @brief Reads bytes from the internal receive buffer.
 *
 * Refills the buffer from the HID device if required.
 *
 * @param size Number of bytes to read
 * @return std::vector<unsigned char> Bytes read from the device
 */
std::vector<unsigned char> GraphitiConnectionHID::read(size_t size) {
    std::vector<uint8_t> output;

    while (byteBuffer_.size() < size) {
        readToBuffer(); // fills buffer from HID device
        if (byteBuffer_.empty()) break;
    }

    size_t available = std::min(size, byteBuffer_.size());
    for (size_t i = 0; i < available; ++i) {
        output.push_back(byteBuffer_.front());
        byteBuffer_.pop_front();
    }

    return output;
}

/**
 * @function GraphitiConnectionHID::readToBuffer
 * @brief Reads a single HID report and appends its payload
 *        to the internal byte buffer.
 *
 * @return void
 */
void GraphitiConnectionHID::readToBuffer() {
    std::vector<uint8_t> report(4864);
    int result = hid_read(device_, report.data(), report.size());

    if (result <= 0) return;

    for (int i = 1; i < result; ++i) {
        byteBuffer_.push_back(report[i]);
    }
}