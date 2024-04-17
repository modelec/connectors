#pragma once

#include "TCPSocket/TCPClient.hpp"
#include "TCPSocket/TCPUtils.hpp"

#include "serialib.h"

#define SERIAL_PORT "/dev/USB_ARDUINO"
#define MAX_MESSAGE_LEN 1048
#define BAUDS 9600 //vitesse des données (bit/sec)
#define TIME_OUT 500

class MyTCPClient : public TCPClient {

public:
    explicit MyTCPClient(const char* ip = "127.0.0.1", int port = 8080);

    void handleMessage(const std::string &message) override;

    void handleMessageFromArduino(const std::string& message);

    void init();

    void start() override;

    void stop() override;

    int write_2_arduino(const std::string& message);

    void read_from_arduino();

private:
    std::string serialPort = SERIAL_PORT;
    int bauds = BAUDS;
    int maxMessageLenght = MAX_MESSAGE_LEN;
    int timeOut = TIME_OUT;
    char buffer[MAX_MESSAGE_LEN] = "";

    bool waitForResponse = false;
    bool waitForPong = false;

    bool isDoingSomething = false;

    serialib serial;

    struct Position {
        struct {
            int x;
            int y;
        } pos;
        float theta;
    } robotPose;

    struct Transit {
        int x;
        int y;
        int endSPeed;
    } transitMode;
};
