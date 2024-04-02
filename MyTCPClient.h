#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

#include "wrapper.h"

class MyTCPClient : public TCPClient {

public:
    explicit MyTCPClient(const char* ip = "127.0.0.1", int port = 8080);

    void handleMessage(const std::string &message) override;

    void handleMessageFromArduino(const std::string& message);

    void init();

    ~MyTCPClient() override;

    serialib* getSerial() {
        return &serial;
    }

private:
    std::string serialPort = SERIAL_PORT;
    int bauds = BAUDS;
    int maxMessageLenght = MAX_MESSAGE_LEN;
    int timeOut = TIME_OUT;
    char buffer[MAX_MESSAGE_LEN] = "";

    serialib serial;
};
