/** ******************************************************************************
 * @file    Graphiti-JNI.cpp
 * @company Orbit Research
 * @version v1.0
 * @date    08-12-25
 * @brief   JNI bridge implementation for Java bindings.
 *
 * @details
 * Implements the native methods declared in the generated JNI header
 * to expose Graphiti functionality to Java. Handles conversions between
 * Java and C++ data types and forwards calls to `GraphitiExtension`.
 *
 ******************************************************************************
 * @attention
 * Copyright (c) 2026 Orbit Research LLC
 * Distributed under GNU General Public License V3.0
 *******************************************************************************/

#include <jni.h>
#include <string>
#include <vector>
#include "binding_jgraphiti_Graphiti.h"           // auto-generated JNI header
#include "internal_handle.hpp"

#pragma comment(lib, "libGraphiti.dll.a")

/**
 * @function jstringToString
 * @brief Convert a Java jstring to a C++ std::string.
 * @param env JNI environment pointer.
 * @param jstr Java string to convert.
 * @return std::string Converted C++ string.
 */
std::string jstringToString(JNIEnv* env, jstring jstr) {
    if (!jstr) return "";
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    std::string str(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    return str;
}

extern "C" {

// Extension methods

/**
 * @function Java_binding_jgraphiti_Graphiti_graphiti_1create
 * @brief JNI wrapper - allocate native GraphitiHandle and return as jlong.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @return jlong Native handle pointer as long.
 */
JNIEXPORT jlong JNICALL Java_binding_jgraphiti_Graphiti_graphiti_1create(JNIEnv* env, jobject obj) {
    return reinterpret_cast<jlong>(new GraphitiHandle());
}

/**
 * @function Java_binding_jgraphiti_Graphiti_graphiti_1destroy
 * @brief JNI wrapper - destroy native GraphitiHandle.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_graphiti_1destroy(JNIEnv* env, jobject obj, jlong handle) {
    delete reinterpret_cast<GraphitiHandle*>(handle);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_startUpVCP
 * @brief JNI wrapper - start up VCP connection on native GraphitiExtension.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param portName Serial port name as Java string.
 * @param keyEventsBool Enable key events.
 * @param touchEventsBool Enable touch events.
 * @return jboolean True if connection successful.
 */
JNIEXPORT jboolean JNICALL Java_binding_jgraphiti_Graphiti_startUpVCP(JNIEnv* env, jobject obj, jlong handle, 
    jstring portName, jboolean keyEventsBool, jboolean touchEventsBool
) {
    std::string port = jstringToString(env, portName);
    return reinterpret_cast<GraphitiHandle*>(handle)->api.startUpVCP(port, keyEventsBool, touchEventsBool);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_shutDownVCP
 * @brief JNI wrapper - shutdown VCP connection and disable events.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param keyEventsBool Disable key events.
 * @param touchEventsBool Disable touch events.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_shutDownVCP(JNIEnv* env, jobject obj, jlong handle, 
    jboolean keyEventsBool, jboolean touchEventsBool
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.shutDownVCP(keyEventsBool, touchEventsBool);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_startUpHID
 * @brief JNI wrapper - start up HID connection on native GraphitiExtension.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param vendor_id USB vendor ID.
 * @param product_id USB product ID.
 * @param keyEventsBool Enable key events.
 * @param touchEventsBool Enable touch events.
 * @return jboolean True if connection successful.
 */
JNIEXPORT jboolean JNICALL Java_binding_jgraphiti_Graphiti_startUpHID(JNIEnv* env, jobject obj, jlong handle, 
    jint vendor_id, jint product_id, jboolean keyEventsBool, jboolean touchEventsBool
) {
    return reinterpret_cast<GraphitiHandle*>(handle)->api.startUpHID(vendor_id, product_id, keyEventsBool, touchEventsBool);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_shutDownHID
 * @brief JNI wrapper - shutdown HID connection and disable events.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param keyEventsBool Disable key events.
 * @param touchEventsBool Disable touch events.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_shutDownHID(JNIEnv* env, jobject obj, jlong handle, 
    jboolean keyEventsBool, jboolean touchEventsBool
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.shutDownHID(keyEventsBool, touchEventsBool);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_index
 * @brief JNI wrapper - compute buffer index for provided row and column.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param col Column index.
 * @return jint Buffer index.
 */
JNIEXPORT jint JNICALL Java_binding_jgraphiti_Graphiti_index(JNIEnv* env, jobject obj, jlong handle, jint row, jint col) {
    return reinterpret_cast<GraphitiHandle*>(handle)->api.index(row, col);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setPin
 * @brief JNI wrapper - set a pin height in the native screen buffer.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param col Column index.
 * @param height Pin height.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setPin(JNIEnv* env, jobject obj, jlong handle, jint row, jint col, jint height) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setPin(row, col, height);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_clearScreen
 * @brief JNI wrapper - clear the native screen buffer.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_clearScreen(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.clearScreen();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sleep
 * @brief JNI wrapper - short sleep helper on native GraphitiExtension.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param time Number of seconds to sleep.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sleep(JNIEnv* env, jobject obj, jlong handle, jint time) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sleep(time);
}

// Event handlers

/**
 * @function Java_binding_jgraphiti_Graphiti_getNextOutputEvent
 * @brief JNI wrapper - retrieve the next output event as a Java String.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @return jstring Java String with the event, or null if none available.
 */
JNIEXPORT jstring JNICALL Java_binding_jgraphiti_Graphiti_getNextOutputEvent(JNIEnv* env, jobject obj, jlong handle) {
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextOutputEvent();
    return event ? env->NewStringUTF(event->c_str()) : nullptr;
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getNextGestureEvent
 * @brief JNI wrapper - retrieve the next gesture event as a Java String.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @return jstring Java String with the gesture event, or null if none available.
 */
JNIEXPORT jstring JNICALL Java_binding_jgraphiti_Graphiti_getNextGestureEvent(JNIEnv* env, jobject obj, jlong handle) {
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextOutputEvent();
    return event ? env->NewStringUTF(event->c_str()) : nullptr;
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getDisplayStatusEvent
 * @brief JNI wrapper - obtain display status event as a Java int array.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @return jintArray Java int array with display status, or null if none.
 */
JNIEXPORT jintArray JNICALL Java_binding_jgraphiti_Graphiti_getDisplayStatusEvent(
    JNIEnv* env, jobject obj, jlong handle) 
{
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextDisplayStatusEvent();
    if (!event || event->empty()) return nullptr;

    // Convert unsigned char* → jint*
    std::vector<jint> convertedData(event->size());
    const auto* src = event->data(); // unsigned char*
    for (size_t i = 0; i < event->size(); i++) {
        convertedData[i] = static_cast<jint>(src[i]);
    }

    jintArray result = env->NewIntArray(event->size());
    env->SetIntArrayRegion(result, 0, event->size(), convertedData.data());
    return result;
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getNextKeyEvent
 * @brief JNI wrapper - return the next key event as a Java Set<String>.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @return jobject Java Set<String> with key events, or null if none.
 */
JNIEXPORT jobject JNICALL Java_binding_jgraphiti_Graphiti_getNextKeyEvent(
    JNIEnv* env, jobject obj, jlong handle) 
{
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextKeyEvent();
    if (!event || event->empty()) return nullptr;

    // 1. Find Java HashSet class
    jclass hashSetClass = env->FindClass("java/util/HashSet");
    jmethodID constructor = env->GetMethodID(hashSetClass, "<init>", "(I)V");
    jmethodID addMethod = env->GetMethodID(hashSetClass, "add", "(Ljava/lang/Object;)Z");

    // 2. Create HashSet with initial capacity
    jobject hashSet = env->NewObject(hashSetClass, constructor, event->size());

    // 3. Add each key
    for (const auto& key : *event) {
        jstring jstr = env->NewStringUTF(key.c_str());
        env->CallBooleanMethod(hashSet, addMethod, jstr);
        env->DeleteLocalRef(jstr);
    }

    return hashSet;  // Return as java.util.Set
}

/**
 * @function Java_com_example_graphiti_Graphiti_getNextDrawEvent
 * @brief JNI wrapper - convert native DrawEvent into Java DrawEvent object.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @return jobject Java DrawEvent object, or null if none.
 */
JNIEXPORT jobject JNICALL Java_com_example_graphiti_Graphiti_getNextDrawEvent(
    JNIEnv* env, 
    jobject obj, 
    jlong handle
) {
    // 1. Get native handle and event
    GraphitiHandle* handlePtr = reinterpret_cast<GraphitiHandle*>(handle);
    auto drawEvent = handlePtr->api.getNextDrawEvent(); // std::optional<DrawEvent>
    
    // 2. Return null if no event
    if (!drawEvent || drawEvent->pins.empty()) {
        return nullptr;
    }

    // 3. Find Java classes
    jclass drawEventClass = env->FindClass("binding/java/Graphiti$DrawEvent");
    jclass pinInfoClass = env->FindClass("binding/java/Graphiti$PinInfo");
    if (!drawEventClass || !pinInfoClass) return nullptr;

    // 4. Get field IDs
    jfieldID lengthField = env->GetFieldID(drawEventClass, "length", "I");
    jfieldID pinsField = env->GetFieldID(drawEventClass, "pins", "[Lcom/example/graphiti/Graphiti$PinInfo;");
    
    jfieldID rowIDField = env->GetFieldID(pinInfoClass, "rowID", "I");
    jfieldID columnIDField = env->GetFieldID(pinInfoClass, "columnID", "I");
    jfieldID heightField = env->GetFieldID(pinInfoClass, "height", "I");
    jfieldID blinkRateField = env->GetFieldID(pinInfoClass, "blinkRate", "I");

    // 5. Create DrawEvent object
    jobject jDrawEvent = env->AllocObject(drawEventClass);
    
    // 6. Create and populate pins array
    jobjectArray jPins = env->NewObjectArray(drawEvent->pins.size(), pinInfoClass, nullptr);
    for (size_t i = 0; i < drawEvent->pins.size(); i++) {
        const auto& pin = drawEvent->pins[i];
        jobject jPin = env->AllocObject(pinInfoClass);
        
        env->SetIntField(jPin, rowIDField, pin.rowID);
        env->SetIntField(jPin, columnIDField, pin.columnID);
        env->SetIntField(jPin, heightField, pin.height);
        env->SetIntField(jPin, blinkRateField, pin.blinkRate);
        
        env->SetObjectArrayElement(jPins, i, jPin);
        env->DeleteLocalRef(jPin);
    }

    // 7. Set fields on DrawEvent
    env->SetIntField(jDrawEvent, lengthField, drawEvent->pins.size());
    env->SetObjectField(jDrawEvent, pinsField, jPins);
    
    // 8. Clean up
    env->DeleteLocalRef(jPins);
    
    return jDrawEvent;
}

/**
 * @function Java_binding_jgraphiti_Graphiti_startResponseThread
 * @brief JNI wrapper - start background response thread on native API.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_startResponseThread(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.startResponseThread();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sendNACK_stopResponseThread
 * @brief JNI wrapper - stop background response thread on native API.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendNACK_stopResponseThread(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.stopResponseThread();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sendACK
 * @brief JNI wrapper - send ACK command to device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendACK(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendACK();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sendNACK
 * @brief JNI wrapper - send NACK command to device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendNACK(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendNACK();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getSoftwareVersion
 * @brief JNI wrapper - request software version from device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSoftwareVersion(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSoftwareVersion();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getHardwareVersion
 * @brief JNI wrapper - request hardware version from device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getHardwareVersion(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getHardwareVersion();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getSerialNumber
 * @brief JNI wrapper - request device serial number.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSerialNumber(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSerialNumber();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getBatteryStatus
 * @brief JNI wrapper - request device battery status.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getBatteryStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getBatteryStatus();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getResolutionInformation
 * @brief JNI wrapper - request resolution info from device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getResolutionInformation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getResolutionInformation();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getDeviceOrientation
 * @brief JNI wrapper - request device orientation.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDeviceOrientation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDeviceOrientation();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getHeightInformation
 * @brief JNI wrapper - request height information from device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getHeightInformation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getHeightInformation();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getDeviceName
 * @brief JNI wrapper - request device name string.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDeviceName(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDeviceName();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_updateDisplay
 * @brief JNI wrapper - update full display buffer from Java byte array.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param screen Java byte array for screen data.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateDisplay(JNIEnv* env, jobject obj, jlong handle,
    jbyteArray screen
) {
    jbyte* screen_java = env->GetByteArrayElements(screen, nullptr);
    jsize length = env->GetArrayLength(screen);
    std::vector<uint8_t> screen_vector(screen_java, screen_java + length);  // Copies data
    env->ReleaseByteArrayElements(screen, screen_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateDisplay(screen_vector);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setDisplay
 * @brief JNI wrapper - set display mode/command.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setDisplay(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setDisplay();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_clearDisplay
 * @brief JNI wrapper - clear the display buffer on device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_clearDisplay(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.clearDisplay();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_updateSinglePixel
 * @brief JNI wrapper - update a single pixel at row/column.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param column Column index.
 * @param height Height value.
 * @param blinkRate Blink rate value.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSinglePixel(JNIEnv* env, jobject obj, jlong handle, 
    jint row, jint column, jint height, jint blinkRate
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSinglePixel(row, column, height, blinkRate);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_updateSingleRow
 * @brief JNI wrapper - update a single row from Java byte array.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param rowData Java byte array for row data.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSingleRow(JNIEnv* env, jobject obj, jlong handle,
    jint row, jbyteArray rowData
) {
    jbyte* rowData_java = env->GetByteArrayElements(rowData, nullptr);
    jsize length = env->GetArrayLength(rowData);
    std::vector<uint8_t> rowData_vector(rowData_java, rowData_java + length);  // Copies data
    env->ReleaseByteArrayElements(rowData, rowData_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSingleRow(row, rowData_vector);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_updateSingleColumn
 * @brief JNI wrapper - update a single column from Java byte array.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param column Column index.
 * @param columnData Java byte array for column data.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSingleColumn(JNIEnv* env, jobject obj, jlong handle,
    jint column, jbyteArray columnData
) {
    jbyte* columnData_java = env->GetByteArrayElements(columnData, nullptr);
    jsize length = env->GetArrayLength(columnData);
    std::vector<uint8_t> columnData_vector(columnData_java, columnData_java + length);  // Copies data
    env->ReleaseByteArrayElements(columnData, columnData_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSingleColumn(column, columnData_vector);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getAllPixelsPositionStatus
 * @brief JNI wrapper - request all pixels position/status.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getAllPixelsPositionStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getAllPixelsPositionStatus();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getSinglePixelPositionStatus
 * @brief JNI wrapper - request position/status for a single pixel.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param column Column index.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSinglePixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint row, jint column
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSinglePixelPositionStatus(row, column);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getSingleRowPixelPositionStatus
 * @brief JNI wrapper - request position/status for a single row.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSingleRowPixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint row
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSingleRowPixelPositionStatus(row);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getSingleColumnPixelPositionStatus
 * @brief JNI wrapper - request position/status for a single column.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param column Column index.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSingleColumnPixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint column
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSingleColumnPixelPositionStatus(column);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_showMessage
 * @brief JNI wrapper - display a message with selection flags and blink rate.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param message Java string message.
 * @param selectFlags Java byte array for selection flags.
 * @param blinkRate Blink rate value.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_showMessage(
    JNIEnv* env, jobject obj, jlong handle,
    jstring message, jbyteArray selectFlags, jint blinkRate
) {
    // Convert jbyteArray → std::vector<uint8_t>
    jbyte* flags = env->GetByteArrayElements(selectFlags, nullptr);
    jsize length = env->GetArrayLength(selectFlags);
    std::vector<uint8_t> flagsVec(flags, flags + length);  // Copies data
    env->ReleaseByteArrayElements(selectFlags, flags, JNI_ABORT);

    // 3. Call C++ API
    reinterpret_cast<GraphitiHandle*>(handle)->api.showMessage(jstringToString(env, message), flagsVec, blinkRate);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setCursor
 * @brief JNI wrapper - set cursor configuration on device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param row Row index.
 * @param column Column index.
 * @param height Height value.
 * @param length Cursor length.
 * @param blinkRate Blink rate value.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setCursor(JNIEnv* env, jobject obj, jlong handle,
    jint row, jint column, jint height, jint length, jint blinkRate
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setCursor(row, column, height, length, blinkRate);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sendImageInterruptible
 * @brief JNI wrapper - start interruptible image transfer.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param name Java string image name.
 * @param filepath Java string file path.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendImageInterruptible(JNIEnv* env, jobject obj, jlong handle, 
    jstring name, jstring filepath
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendImageInterruptible(jstringToString(env, name), jstringToString(env, filepath));
}

/**
 * @function Java_binding_jgraphiti_Graphiti_sendImageBlocking
 * @brief JNI wrapper - start blocking image transfer.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param name Java string image name.
 * @param filepath Java string file path.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendImageBlocking(JNIEnv* env, jobject obj, jlong handle,
    jstring name, jstring filepath
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendImageBlocking(jstringToString(env, name), jstringToString(env, filepath));
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setKeyEvent
 * @brief JNI wrapper - enable or disable key events.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param enabled True to enable, false to disable.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setKeyEvent(JNIEnv* env, jobject obj, jlong handle, jboolean enabled) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setKeyEvent(enabled);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setTouchEvent
 * @brief JNI wrapper - enable or disable touch events.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param enabled True to enable, false to disable.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setTouchEvent(JNIEnv* env, jobject obj, jlong handle, jboolean enabled) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setTouchEvent(enabled);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getLastTouchPointStatus
 * @brief JNI wrapper - request last touch point status from device.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getLastTouchPointStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getLastTouchPointStatus();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_vibratorControlCommand
 * @brief JNI wrapper - control device vibrator with given parameters.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param frequencyRange Frequency range.
 * @param dutyCycle Duty cycle.
 * @param duration Duration in ms.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_vibratorControlCommand(JNIEnv* env, jobject obj, jlong handle, 
    jint frequencyRange, jint dutyCycle, jint duration
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
}

/**
 * @function Java_binding_jgraphiti_Graphiti_getDateAndTime
 * @brief JNI wrapper - request device date and time.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDateAndTime(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDateAndTime();
}

/**
 * @function Java_binding_jgraphiti_Graphiti_setDateAndTime
 * @brief JNI wrapper - set device date and time.
 * @param env JNI environment pointer.
 * @param obj Java object.
 * @param handle Native handle pointer as long.
 * @param day Day value.
 * @param month Month value.
 * @param year Year value.
 * @param hour Hour value.
 * @param minute Minute value.
 * @param second Second value.
 */
JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setDateAndTime(JNIEnv* env, jobject obj, jlong handle, 
    jint day, jint month, jint year, jint hour, jint minute, jint second
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setDateAndTime(day, month, year, hour, minute, second);
}

} // extern "C"
