#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>

#include <Graphiti/API.hpp>
#include <Graphiti/Connection/Connection_VCP.hpp>
#include <Graphiti/Connection/Connection_HID.hpp>

std::vector<uint8_t> scr((ROW_COUNT * COLUMN_COUNT) * 2);
GraphitiConnectionVCP* vcpConn = nullptr;
GraphitiConnectionHID* hidConn = nullptr;
Graphiti_API* graphiti = nullptr;

int index(int row, int col) {
    return ((row * COLUMN_COUNT) + col) * 2;
}

void setPin(int row, int col, int height) {
    scr[index(row, col)] = height;
}

void clearBoard() {
    for (int i = 0; i < scr.size(); ++i) {
        scr[i] = 0;
    }
}

void RunDisplay() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        setPin(20, 30, 4);

        graphiti->updateDisplay(scr);

        graphiti->getSoftwareVersion();

        auto output = graphiti->getNextOutputEvent();
        if(output.has_value()) {
            std::cout << output.value() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto keyEventOption = graphiti->getNextKeyEvent();

        std::set<std::string> keyEvent;

        if(keyEventOption.has_value()) {
            keyEvent = keyEventOption.value();
        }

        if(keyEvent.contains("1")){
            std::cout << "Key Event 1 on Main found" << std::endl;
        }
    }
}

int vcpTest() {
    vcpConn = new GraphitiConnectionVCP("COM4"); // Your COM port
    if (!vcpConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return 1;
    }
    graphiti = new Graphiti_API(vcpConn);
    return 0;
}

int hidTest() {
    hidConn = new GraphitiConnectionHID(0x1FC9, 0x8217); // Your COM port
    if (!hidConn->open()) {
        std::cerr << "Failed to open connection.\n";
        return 1;
    }
    graphiti = new Graphiti_API(hidConn);
    return 0;
}

int main()
{
    if (vcpTest() == 1) {
        return 1;
    }

    graphiti->startResponseThread();
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    graphiti->clearDisplay();

    std::this_thread::sleep_for(std::chrono::seconds(2));
    graphiti->setKeyEvent(true);

    RunDisplay();

    delete graphiti;
    delete vcpConn;

    return 0;
}