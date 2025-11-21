#include <jni.h>
#include <string>
#include <vector>
#include "binding_jgraphiti_Graphiti.h"           // auto-generated JNI header
#include "internal_handle.hpp"

#pragma comment(lib, "libGraphiti.dll.a")

std::string jstringToString(JNIEnv* env, jstring jstr) {
    if (!jstr) return "";
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    std::string str(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    return str;
}

extern "C" {

// Extension methods

JNIEXPORT jlong JNICALL Java_binding_jgraphiti_Graphiti_graphiti_1create(JNIEnv*, jobject) {
    return reinterpret_cast<jlong>(new GraphitiHandle());
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_graphiti_1destroy(JNIEnv*, jobject, jlong handle) {
    delete reinterpret_cast<GraphitiHandle*>(handle);
}

JNIEXPORT jboolean JNICALL Java_binding_jgraphiti_Graphiti_startUpVCP(JNIEnv* env, jobject obj, jlong handle, 
    jstring portName, jboolean keyEventsBool, jboolean touchEventsBool
) {
    std::string port = jstringToString(env, portName);
    return reinterpret_cast<GraphitiHandle*>(handle)->api.startUpVCP(port, keyEventsBool, touchEventsBool);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_shutDownVCP(JNIEnv* env, jobject obj, jlong handle, 
    jboolean keyEventsBool, jboolean touchEventsBool
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.shutDownVCP(keyEventsBool, touchEventsBool);
}

JNIEXPORT jboolean JNICALL Java_binding_jgraphiti_Graphiti_startUpHID(JNIEnv* env, jobject obj, jlong handle, 
    jint vendor_id, jint product_id, jboolean keyEventsBool, jboolean touchEventsBool
) {
    return reinterpret_cast<GraphitiHandle*>(handle)->api.startUpHID(vendor_id, product_id, keyEventsBool, touchEventsBool);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_shutDownHID(JNIEnv* env, jobject obj, jlong handle, 
    jboolean keyEventsBool, jboolean touchEventsBool
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.shutDownHID(keyEventsBool, touchEventsBool);
}

JNIEXPORT jint JNICALL Java_binding_jgraphiti_Graphiti_index(JNIEnv* env, jobject obj, jlong handle, jint row, jint col) {
    return reinterpret_cast<GraphitiHandle*>(handle)->api.index(row, col);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setPin(JNIEnv* env, jobject obj, jlong handle, jint row, jint col, jint height) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setPin(row, col, height);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_clearScreen(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.clearScreen();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sleep(JNIEnv* env, jobject obj, jlong handle, jint time) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sleep(time);
}

//Event handlers

JNIEXPORT jstring JNICALL Java_binding_jgraphiti_Graphiti_getNextOutputEvent(JNIEnv* env, jobject obj, jlong handle) {
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextOutputEvent();
    return event ? env->NewStringUTF(event->c_str()) : nullptr;
}

JNIEXPORT jstring JNICALL Java_binding_jgraphiti_Graphiti_getNextGestureEvent(JNIEnv* env, jobject obj, jlong handle) {
    auto event = reinterpret_cast<GraphitiHandle*>(handle)->api.getNextOutputEvent();
    return event ? env->NewStringUTF(event->c_str()) : nullptr;
}

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

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_startResponseThread(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.startResponseThread();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendNACK_stopResponseThread(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.stopResponseThread();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendACK(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendACK();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendNACK(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendNACK();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSoftwareVersion(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSoftwareVersion();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getHardwareVersion(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getHardwareVersion();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSerialNumber(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSerialNumber();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getBatteryStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getBatteryStatus();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getResolutionInformation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getResolutionInformation();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDeviceOrientation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDeviceOrientation();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getHeightInformation(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getHeightInformation();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDeviceName(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDeviceName();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateDisplay(JNIEnv* env, jobject obj, jlong handle,
    jbyteArray screen
) {
    jbyte* screen_java = env->GetByteArrayElements(screen, nullptr);
    jsize length = env->GetArrayLength(screen);
    std::vector<uint8_t> screen_vector(screen_java, screen_java + length);  // Copies data
    env->ReleaseByteArrayElements(screen, screen_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateDisplay(screen_vector);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setDisplay(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setDisplay();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_clearDisplay(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.clearDisplay();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSinglePixel(JNIEnv* env, jobject obj, jlong handle, 
    jint row, jint column, jint height, jint blinkRate
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSinglePixel(row, column, height, blinkRate);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSingleRow(JNIEnv* env, jobject obj, jlong handle,
    jint row, jbyteArray rowData
) {
    jbyte* rowData_java = env->GetByteArrayElements(rowData, nullptr);
    jsize length = env->GetArrayLength(rowData);
    std::vector<uint8_t> rowData_vector(rowData_java, rowData_java + length);  // Copies data
    env->ReleaseByteArrayElements(rowData, rowData_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSingleRow(row, rowData_vector);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_updateSingleColumn(JNIEnv* env, jobject obj, jlong handle,
    jint column, jbyteArray columnData
) {
    jbyte* columnData_java = env->GetByteArrayElements(columnData, nullptr);
    jsize length = env->GetArrayLength(columnData);
    std::vector<uint8_t> columnData_vector(columnData_java, columnData_java + length);  // Copies data
    env->ReleaseByteArrayElements(columnData, columnData_java, JNI_ABORT);

    reinterpret_cast<GraphitiHandle*>(handle)->api.updateSingleColumn(column, columnData_vector);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getAllPixelsPositionStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getAllPixelsPositionStatus();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSinglePixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint row, jint column
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSinglePixelPositionStatus(row, column);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSingleRowPixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint row
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSingleRowPixelPositionStatus(row);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getSingleColumnPixelPositionStatus(JNIEnv* env, jobject obj, jlong handle,
    jint column
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getSingleColumnPixelPositionStatus(column);
}

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

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setCursor(JNIEnv* env, jobject obj, jlong handle,
    jint row, jint column, jint height, jint length, jint blinkRate
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setCursor(row, column, height, length, blinkRate);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendImageInterruptible(JNIEnv* env, jobject obj, jlong handle, 
    jstring name, jstring filepath
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendImageInterruptible(jstringToString(env, name), jstringToString(env, filepath));
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_sendImageBlocking(JNIEnv* env, jobject obj, jlong handle,
    jstring name, jstring filepath
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.sendImageBlocking(jstringToString(env, name), jstringToString(env, filepath));
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setKeyEvent(JNIEnv* env, jobject obj, jlong handle, jboolean enabled) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setKeyEvent(enabled);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setTouchEvent(JNIEnv* env, jobject obj, jlong handle, jboolean enabled) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setTouchEvent(enabled);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getLastTouchPointStatus(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getLastTouchPointStatus();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_vibratorControlCommand(JNIEnv* env, jobject obj, jlong handle, 
    jint frequencyRange, jint dutyCycle, jint duration
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.vibratorControlCommand(frequencyRange, dutyCycle, duration);
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_getDateAndTime(JNIEnv* env, jobject obj, jlong handle) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.getDateAndTime();
}

JNIEXPORT void JNICALL Java_binding_jgraphiti_Graphiti_setDateAndTime(JNIEnv* env, jobject obj, jlong handle, 
    jint day, jint month, jint year, jint hour, jint minute, jint second
) {
    reinterpret_cast<GraphitiHandle*>(handle)->api.setDateAndTime(day, month, year, hour, minute, second);
}

} // extern "C"
