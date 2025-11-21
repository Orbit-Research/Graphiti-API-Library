#ifndef GRAPHITI_CONNECTION_HID_HPP
#define GRAPHITI_CONNECTION_HID_HPP

#include <string>
#include <hidapi.h>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <deque>
#include "Connection.hpp"

class GraphitiConnectionHID : public GraphitiConnection {
public:
    explicit GraphitiConnectionHID(const uint16_t vendor_id, const uint16_t product_id);

    bool open();
    void close();
    bool write(const std::vector<unsigned char>& data);
    std::vector<unsigned char> read(size_t size);

private:
    const int vendor_id_;
    const int product_id_;
    hid_device* device_;

    std::deque<uint8_t> byteBuffer_;

    bool writeAndErrorcheck (const std::vector<unsigned char>& report);

    unsigned char writeReportID (const std::vector<unsigned char>& data);

    void readToBuffer();
};

#endif // GRAPHITI_CONNECTION_HID_H
