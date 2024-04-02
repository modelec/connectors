#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *ip, int port) : TCPClient(ip, port) {
}

void MyTCPClient::handleMessage(const std::string &message) {
    std::cout << message << std::endl;

    std::vector<std::string> token = TCPSocket::split(message, ";");

    if (token.size() != 4) {
        std::cerr << "Invalid message format" << std::endl;
        return;
    }

    if (token[1] == "arduino" || token[1] == "all") {
        if (token[2] == "ping") {
            write_2_arduino(&serial, "ping\n");
        }
        else if (token[2] == "go") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "G " + args[0] + " " + args[1] + "\n";
            write_2_arduino(&serial, command.c_str());
        } else if (token[2] == "angle") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "R " + args[0] + "\n";
            write_2_arduino(&serial, command.c_str());
        } else if (token[2] == "set") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "S " + args[0] + " " + args[1] + " " + args[2] + "\n";
            write_2_arduino(&serial, command.c_str());
        } else if (token[2] == "speed") {
            std::string command = "V " + token[3] + "\n";

            write_2_arduino(&serial, command.c_str());
        }
    }
}

void MyTCPClient::init() {
    this->serial = init_serial();

    std::cout << "Serial is open ? : " << this->serial.isDeviceOpen() << std::endl;

    this->sendMessage("arduino;strat;ready;1");
}

MyTCPClient::~MyTCPClient() {
    this->serial.closeDevice();
}

void MyTCPClient::handleMessageFromArduino(const std::string &message) {
    if (message == "pong") {
        this->sendMessage("arduino;ihm;pong;1");
    }
    // TODO handle the response from arduino
}
