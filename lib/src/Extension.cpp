#include <cstdio>

#include <Graphiti/Extension.hpp>

GraphitiExtension::GraphitiExtension()
    : Graphiti_API(),
      screen((ROW_COUNT * COLUMN_COUNT) * 2)
{}

GraphitiExtension::GraphitiExtension(GraphitiConnection* connection)
    : Graphiti_API(connection),
      screen((ROW_COUNT * COLUMN_COUNT) * 2)
{}

GraphitiExtension::~GraphitiExtension() {}

bool GraphitiExtension::startUpVCP(std::string port, bool keyEventsBool, bool touchEventsBool){
    vcpConn = new GraphitiConnectionVCP(port); // Your COM port
    if (!vcpConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return false;
    }

    setConnection(vcpConn);
    
    startUpSequence(keyEventsBool, touchEventsBool);

    return true;
}

bool GraphitiExtension::startUpHID(uint16_t vendor_id, uint16_t product_id, bool keyEventsBool, bool touchEventsBool){
    hidConn = new GraphitiConnectionHID(vendor_id, product_id); // Your COM port
    if (!hidConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return false;
    }

    setConnection(hidConn);
    
    startUpSequence(keyEventsBool, touchEventsBool);

    return true;
}

void GraphitiExtension::startUpSequence(bool keyEventsBool, bool touchEventsBool) {
    startResponseThread();
    sleep(2);

    //Trying touch event first

    if(touchEventsBool) {
        #ifdef DEBUG
            std::cout << "Enabling Touch event" << std::endl;
        #endif 
        setTouchEvent(true);
        sleep(2);
    }

    if(keyEventsBool) {
        #ifdef DEBUG
            std::cout << "Enabling Key event" << std::endl;
        #endif 
        setKeyEvent(keyEventsBool);
        sleep(2);
    }
}

void GraphitiExtension::shutDownVCP(bool keyEventsBool, bool touchEventsBool){
    shutDownSequence("VCP", keyEventsBool, touchEventsBool);
}

void GraphitiExtension::shutDownHID(bool keyEventsBool, bool touchEventsBool){
    shutDownSequence("HID", keyEventsBool, touchEventsBool);
}

void GraphitiExtension::shutDownSequence(std::string connection_type, bool keyEventsBool, bool touchEventsBool){
    if(touchEventsBool) {
        setTouchEvent(false);
        sleep(2);
    }

    if(keyEventsBool) {
        setKeyEvent(false);
        sleep(2);
    }

    clearDisplay();
    sleep(5);

    if(connection_type == "VCP") {
        vcpConn->close();
    } else {
        hidConn->close();
    }

    stopResponseThread();

    if(connection_type == "VCP") {
        delete vcpConn;
    } else {
        delete hidConn;
    }
}

void GraphitiExtension::keyLoop(
    std::function<void(const std::set<std::string>&, void*)> key_func, 
    bool *loop_condition,
    void* parameters
) {
    while(*loop_condition) {
        sleep(5);

        std::optional<std::set<std::string>> keyEvent = getNextKeyEvent();

        if(keyEvent.has_value()) {
            //Key funciton effects(ei. display changes)
            key_func(keyEvent.value(), parameters);
        }
    }
}

void GraphitiExtension::drawLoop(
    std::function<void(const Graphiti_API::DrawEvent&, void*)> draw_func, 
    bool *loop_condition,
    void* parameters
) {
    while(*loop_condition) {

        #ifdef DEBUG
            std::cout << "getNextDrawEvent" << std::endl;
        #endif 

        std::optional<Graphiti_API::DrawEvent> drawEvent = getNextDrawEvent();

        #ifdef DEBUG
            std::cout << "return from getNextDrawEvent" << std::endl;
        #endif 

        if(drawEvent.has_value()) {
            //Draw event function call when there is value
            #ifdef DEBUG
                std::cout << "getNextDrawEvent has a value" << std::endl;
            #endif 
            draw_func(drawEvent.value(), parameters);
        } else {
             #ifdef DEBUG
                std::cout << "getNextDrawEvent has no value" << std::endl;
            #endif 
        }
    }
}

int GraphitiExtension::index(int row, int col) {
    return ((row * COLUMN_COUNT) + col) * 2;
}

void GraphitiExtension::setPin(int row, int col, int height) {
    screen[index(row, col)] = height;
}

void GraphitiExtension::clearScreen() {
    for (int i = 0; i < screen.size(); ++i) {
        screen[i] = 0;
    }
}

void GraphitiExtension::sleep(int time){
    std::this_thread::sleep_for(std::chrono::seconds(time));
}