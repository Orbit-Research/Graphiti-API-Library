#include <vector>
#include <set>
#include <string>
#include <cstring>

#include <Graphiti/CWrapper/capi.h>
#include <Graphiti/Extension.hpp>

// Opaque handle = C++ class
struct GraphitiHandle {
    GraphitiExtension api;
};

// Constructor/Destructor
extern "C" {
    GraphitiHandle* graphiti_create() {
        return new GraphitiHandle();
    }

    GraphitiHandle* graphiti_createWithConnection(void* connection) {
        return new GraphitiHandle{static_cast<GraphitiConnection*>(connection)};
    }

    void graphiti_destroy(GraphitiHandle* handle) {
        delete handle;
    }

    // Connection
    void graphiti_setConnection(GraphitiHandle* handle, void* connection) {
        handle->api.setConnection(static_cast<GraphitiConnection*>(connection));
    }

    bool graphiti_startUpVCP(GraphitiHandle* handle, const char* portName, 
        bool keyEventsBool, bool touchEventsBool
    ){
        return handle->api.startUpVCP(portName, keyEventsBool, touchEventsBool);
    }

    void graphiti_shutDownVCP(GraphitiHandle* handle, 
        bool keyEventsBool, bool touchEventsBool
    ){
        handle->api.shutDownVCP(keyEventsBool, touchEventsBool);
    }

    bool graphiti_startUpHID(GraphitiHandle* handle, uint16_t vendor_id, uint16_t product_id, 
        bool keyEventsBool, bool touchEventsBool
    ){
        return handle->api.startUpHID(vendor_id, product_id, keyEventsBool, touchEventsBool);
    }

    void graphiti_shutDownHID(GraphitiHandle* handle, 
        bool keyEventsBool, bool touchEventsBool
    ){
        handle->api.shutDownHID(keyEventsBool, touchEventsBool);
    }

    int graphiti_index(GraphitiHandle* handle, int row, int col) {
        return handle->api.index(row, col);
    }

    void graphiti_setPin(GraphitiHandle* handle, int row, int col, int height){
        handle->api.setPin(row, col, height);
    }

    void graphiti_clearScreen(GraphitiHandle* handle) {
        handle->api.clearScreen();
    }

    void graphiti_sleep(GraphitiHandle* handle, int time){
        handle->api.sleep(time);
    }

    // Thread Control
    void graphiti_startResponseThread(GraphitiHandle* handle) {
        handle->api.startResponseThread();
    }

    void graphiti_stopResponseThread(GraphitiHandle* handle) {
        handle->api.stopResponseThread();
    }

    void graphiti_getNextOutputEvent(GraphitiHandle* handle, char* out, size_t max_len) {
        if (!out || max_len == 0) return;

        auto event = handle->api.getNextOutputEvent();
        if (event && !event->empty()) {
            std::strncpy(out, event->c_str(), max_len - 1);
            out[max_len - 1] = '\0';  // ensure null-termination
        } else {
            out[0] = '\0';  // write empty string
        }
    }

    int graphiti_getNextDisplayStatusEvent(GraphitiHandle* handle, int* buffer, int max_len) {
        if (!buffer || max_len <= 0) return 0;

        auto event = handle->api.getNextDisplayStatusEvent();
        if (!event) return 0;

        int count = std::min<int>(event->size(), max_len);
        memcpy(buffer, event->data(), count * sizeof(int));
        return count;
    }

    int graphiti_getNextKeyEvent(GraphitiHandle* handle, char** out_keys, int max_string_len) {
        if (!handle || !out_keys || max_string_len <= 0)
            return 0;

        constexpr int MAX_KEYS = 14;

        auto event = handle->api.getNextKeyEvent();  // std::optional<std::vector<std::string>>
        if (!event)
            return 0;

        int i = 0;
        for (const std::string& key : *event) {
            if (i >= MAX_KEYS) break;
            std::strncpy(out_keys[i], key.c_str(), max_string_len - 1);
            out_keys[i][max_string_len - 1] = '\0';
            ++i;
        }

        return i;
    }

    void graphiti_getNextGestureEvent(GraphitiHandle* handle, char* out, size_t max_len) {
        if (!out || max_len == 0) return;

        auto event = handle->api.getNextOutputEvent();
        if (event && !event->empty()) {
            std::strncpy(out, event->c_str(), max_len - 1);
            out[max_len - 1] = '\0';  // ensure null-termination
        } else {
            out[0] = '\0';  // write empty string
        }
    }

    int graphiti_getNextDrawEvent(GraphitiHandle* handle, PinInfo_C* out_pins, int max_len) {
        if (!handle || !out_pins || max_len <= 0)
            return 0;

        auto event = handle->api.getNextDrawEvent();  // returns optional<DrawEvent>
        if (!event)
            return 0;

        int count = std::min<int>(event->pins.size(), max_len);
        for (int i = 0; i < count; ++i) {
            out_pins[i].rowID      = event->pins[i].rowID;
            out_pins[i].columnID   = event->pins[i].columnID;
            out_pins[i].height     = event->pins[i].height;
            out_pins[i].blinkRate  = event->pins[i].blinkRate;
        }

        return count;
    }

    void graphiti_sendACK(GraphitiHandle* handle) {
        handle->api.sendACK();
    }

    void graphiti_sendNACK(GraphitiHandle* handle) {
        handle->api.sendNACK();
    }

    void graphiti_getSoftwareVersion(GraphitiHandle* handle) {
        handle->api.getSoftwareVersion();
    }

    void graphiti_getHardwareVersion(GraphitiHandle* handle) {
        handle->api.getHardwareVersion();
    }

    void graphiti_getSerialNumber(GraphitiHandle* handle) {
        handle->api.getSerialNumber();
    }

    void graphiti_getBatteryStatus(GraphitiHandle* handle) {
        handle->api.getBatteryStatus();
    }

    void graphiti_getResolutionInformation(GraphitiHandle* handle) {
        handle->api.getResolutionInformation();
    }

    void graphiti_getDeviceOrientation(GraphitiHandle* handle) {
        handle->api.getDeviceOrientation();
    }

    void graphiti_getHeightInformation(GraphitiHandle* handle) {
        handle->api.getHeightInformation();
    }

    void graphiti_getDeviceName(GraphitiHandle* handle) {
        handle->api.getDeviceName();
    }

    void graphiti_updateDisplay(GraphitiHandle* handle, const uint8_t* screen_data, int length) {
        std::vector<uint8_t> data(screen_data, screen_data + length);
        handle->api.updateDisplay(data);
    }

    void graphiti_setDisplay(GraphitiHandle* handle) {
        handle->api.setDisplay();
    }

    void graphiti_clearDisplay(GraphitiHandle* handle) {
        handle->api.clearDisplay();
    }

    void graphiti_updateSinglePixel(GraphitiHandle* handle, int row, int column, int height, int blinkRate) {
        handle->api.updateSinglePixel(row, column, height, blinkRate);
    }

    void graphiti_updateSingleRow(GraphitiHandle* handle, int row, const uint8_t* rowData, int length) {
        std::vector<uint8_t> data(rowData, rowData + length);
        handle->api.updateSingleRow(row, data);
    }

    void graphiti_updateSingleColumn(GraphitiHandle* handle, int column, const uint8_t* columnData, int length) {
        std::vector<uint8_t> data(columnData, columnData + length);
        handle->api.updateSingleColumn(column, data);
    }

    void graphiti_getAllPixelsPositionStatus(GraphitiHandle* handle) {
        handle->api.getAllPixelsPositionStatus();
    }

    void graphiti_getSinglePixelPositionStatus(GraphitiHandle* handle, int row, int column) {
        handle->api.getSinglePixelPositionStatus(row, column);
    }

    void graphiti_getSingleRowPixelPositionStatus(GraphitiHandle* handle, uint8_t row) {
        handle->api.getSingleRowPixelPositionStatus(row);
    }

    void graphiti_getSingleColumnPixelPositionStatus(GraphitiHandle* handle, uint8_t column) {
        handle->api.getSingleColumnPixelPositionStatus(column);
    }

    void graphiti_showMessage(GraphitiHandle* handle, const char* message, const uint8_t* selectFlags, int flagLength, int blinkRate) {
        std::string msg(message);
        std::vector<uint8_t> flags(selectFlags, selectFlags + flagLength);
        handle->api.showMessage(msg, flags, blinkRate);
    }

    void graphiti_setCursor(GraphitiHandle* handle, int row, int column, int height, int length, int blinkRate) {
        handle->api.setCursor(row, column, height, length, blinkRate);
    }

    void graphiti_sendImageInterruptible(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageInterruptible(std::string(name), std::string(filepath));
    }

    void graphiti_sendImageBlocking(GraphitiHandle* handle, const char* name, const char* filepath) {
        handle->api.sendImageBlocking(std::string(name), std::string(filepath));
    }

    void graphiti_setKeyEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setKeyEvent(enabled);
    }

    void graphiti_setTouchEvent(GraphitiHandle* handle, bool enabled) {
        handle->api.setTouchEvent(enabled);
    }

    void graphiti_getLastTouchPointStatus(GraphitiHandle* handle) {
        handle->api.getLastTouchPointStatus();
    }

    void graphiti_vibratorControlCommand(GraphitiHandle* handle, int frequencyRange, int dutyCycle, int duration) {
        handle->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
    }

    void graphiti_getDateAndTime(GraphitiHandle* handle) {
        handle->api.getDateAndTime();
    }

    void graphiti_setDateAndTime(GraphitiHandle* handle, int day, int month, int year, int hour, int minute, int second) {
        handle->api.setDateAndTime(day, month, year, hour, minute, second);
    }
}