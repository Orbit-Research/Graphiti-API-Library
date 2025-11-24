#include <Graphiti/Connection/Connection_HID.hpp>
#include <iostream>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <algorithm>
#include <iomanip>

GraphitiConnectionHID::GraphitiConnectionHID(const uint16_t vendor_id, const uint16_t product_id)
    : vendor_id_(vendor_id), product_id_(product_id), device_(nullptr) {
}

bool GraphitiConnectionHID::open() {
    if (device_) return true; // Already open

    device_ = hid_open(vendor_id_, product_id_, nullptr);
    if (!device_) {
        std::cerr << "Failed to open HID device (VID: 0x" << std::hex << vendor_id_ 
                    << ", PID: 0x" << product_id_ << ")" << std::endl;
        return false;
    }
    
    // Set non-blocking mode (similar to ASIO behavior)
    hid_set_nonblocking(device_, 1);
    return true;
}

void GraphitiConnectionHID::close() {
    if (device_) {
        hid_close(device_);
    }
}

bool GraphitiConnectionHID::write(const std::vector<unsigned char>& data) {
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

bool GraphitiConnectionHID::writeAndErrorcheck (const std::vector<unsigned char>& report) {
    int result = hid_write(device_, report.data(), report.size());
    if (result < 0) {
        const wchar_t* error_message = hid_error(device_);
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, error_message, -1, NULL, 0, NULL, NULL);
        std::string error_string(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, error_message, -1, &error_string[0], size_needed, NULL, NULL);
        std::cerr << "Write failed: " << error_string << std::endl;
        return false;
    }
    return true;
}

unsigned char GraphitiConnectionHID::writeReportID (const std::vector<unsigned char>& data) {
    std::unordered_set<uint8_t> cmdID_List = {0x15, 0x18, 0x19, 0x2F, 0x30};

    if (cmdID_List.count(data[1])) {
        return 0x03;
    }
    return 0x02;
}


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

void GraphitiConnectionHID::readToBuffer() {
    std::vector<uint8_t> report(4864);
    int result = hid_read(device_, report.data(), report.size());

    if (result <= 0) return;

    for (int i = 1; i < result; ++i) {
        byteBuffer_.push_back(report[i]);
    }
}