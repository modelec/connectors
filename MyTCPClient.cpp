#include "MyTCPClient.h"

MyTCPClient::MyTCPClient(const char *ip, int port) : TCPClient(ip, port), robotPose({{0, 0}, 0}) {
    init();
}

void MyTCPClient::handleMessage(const std::string &message) {
    // std::cout << message << std::endl;

    std::vector<std::string> token = TCPSocket::split(message, ";");

    if (token.size() != 4) {
        std::cerr << "Invalid message format" << std::endl;
        return;
    }

    if (token[1] == "arduino" || token[1] == "all") {
        if (token[2] == "ping") {
            waitForPong = true;
            this->write_2_arduino("p\n");
        }
        else if (token[2] == "go") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "G " + std::to_string(std::stoi(args[0])) + " " + std::to_string(std::stoi(args[1])) + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
            usleep(100);
        } else if (token[2] == "angle") {
            double angle = std::stod(token[3]) / 100;
            //convert to degres
            double angleDegrees = angle * 180 / 3.14159265359;

            std::string command = "R " + std::to_string(angleDegrees) + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
            usleep(100);
        } else if (token[2] == "set pos") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            double angle = std::stod(args[2]) / 100;
            //convert to degres
            double angleDegrees = angle * 180 / 3.14159265359;

            std::string command = "S " + std::to_string(std::stoi(args[0])) + " " + std::to_string(std::stoi(args[1])) + " " + std::to_string(angleDegrees) + "\n";
            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        } else if (token[2] == "speed") {
            std::string command = "V " + token[3] + "\n";

            if (this->write_2_arduino(command) != 1) {
                std::cout << "Error writing to arduino" << std::endl;
            }
        } else if (token[2] == "get pos") {
            std::string toSend = "arduino;strat;set pos;" + std::to_string(this->robotPose.pos.x) + "," + std::to_string(this->robotPose.pos.y) + "," + std::to_string(this->robotPose.theta * 100);

            this->sendMessage(toSend);
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
    if (waitForPong && TCPSocket::startWith(message, "pong")) {
        this->sendMessage("arduino;ihm;pong;1");
        waitForPong = false;
    } else {
        // std::cout << "Received from arduino : " << message << std::endl;
        std::vector<std::string> token = TCPSocket::split(message, ",");
        if (token.size() == 3) {
            if (TCPSocket::startWith(token[0], ".")) {
                this->robotPose.pos.x = std::stoi("0" + token[0]);
            } else {
                this->robotPose.pos.x = std::stoi(token[0]);
            }

            if (TCPSocket::startWith(token[1], ".")) {
                this->robotPose.pos.y = std::stoi("0" + token[1]);
            } else {
                this->robotPose.pos.y = std::stoi(token[1]);
            }

            if (TCPSocket::startWith(token[2], ".")) {
                this->robotPose.theta = std::stof("0" + token[2]) / 100;
            } else {
                this->robotPose.theta = std::stof(token[2]) / 100;
            }
        }
    }
}

int MyTCPClient::write_2_arduino(const std::string &message) {
    std::cout << "Write to arduino : " << message << std::endl;
    int serialResponse = serial.writeString(message.c_str());
    return serialResponse;
}

void MyTCPClient::read_from_arduino() {
    while (running) {
        if (serial.isDeviceOpen() && serial.available()) {
            char buffer[this->maxMessageLenght+1] = {0};
            if (serial.readString(buffer, '\n', this->maxMessageLenght, this->timeOut) > 0) {
                handleMessageFromArduino(buffer);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}