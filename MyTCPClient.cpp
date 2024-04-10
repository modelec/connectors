#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *ip, int port) : TCPClient(ip, port) {
    init();
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
            this->write_2_arduino("p\n");
        }
        else if (token[2] == "go") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "G " + args[0] + " " + args[1] + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        } else if (token[2] == "angle") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            double angle = std::stof(args[1]) / 100;
            //convert to degres
            double angleDegrees = angle * 180 / 3.14159265359;

            std::string command = "R " + std::to_string(angleDegrees) + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        } else if (token[2] == "set") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            double angle = std::stof(args[2]) / 100;
            //convert to degres
            double angleDegrees = angle * 180 / 3.14159265359;

            std::string command = "S " + args[0] + " " + args[1] + " " + std::to_string(angleDegrees) + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        } else if (token[2] == "speed") {
            std::string command = "V " + token[3] + "\n";

            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        }
    }
}

void MyTCPClient::init() {
    char errorOpening = this->serial.openDevice(this->serialPort.c_str(), this->bauds);

    std::cout << "Opening " << this->serialPort << " at " << this->bauds << " bauds" << errorOpening << std::endl;

    if (errorOpening < 0) {
        std::cout << "Error opening serial port" << std::endl;
    }

    this->sendMessage("arduino;strat;ready;1");
}

void MyTCPClient::start() {
    TCPClient::start();
    std::thread arduinoRecievedThread(&MyTCPClient::read_from_arduino, this);
    arduinoRecievedThread.detach();
}


void MyTCPClient::stop() {
    this->serial.closeDevice();
}

void MyTCPClient::handleMessageFromArduino(const std::string &message) {
    if (!waitForResponse) {
        return;
    }

    if (message == "pong") {
        this->sendMessage("arduino;ihm;pong;1");
        waitForResponse = false;
    } else {
        std::cout << "Received from arduino : " << message << std::endl;
        waitForResponse = false;
    }
    // TODO handle the response from arduino
}

int MyTCPClient::write_2_arduino(const std::string &message) {
    std::cout << "Write to arduino : " << message << std::endl;
    int serialResponse = serial.writeString(message.c_str());
    waitForResponse = true;
    return serialResponse;
}

void MyTCPClient::read_from_arduino() {
    while (running) {
        if (serial.isDeviceOpen() && serial.available()) {
            char buffer[this->maxMessageLenght+1] = {0};
            if (serial.readString(buffer, '\n', this->maxMessageLenght, this->timeOut) > 0) {
                handleMessageFromArduino(buffer);
                std::cout << "Arduino recieved" << buffer << std::endl;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}