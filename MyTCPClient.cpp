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
            this->write_2_arduino("ping\n");
        }
        else if (token[2] == "go") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "G " + args[0] + " " + args[1] + "\n";
            int t = this->write_2_arduino(command);
            std::cout << t << std::endl;
        } else if (token[2] == "angle") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            std::string command = "R " + args[0] + "\n";
            int t = this->write_2_arduino(command);
            std::cout << t << std::endl;
        } else if (token[2] == "set") {
            std::vector<std::string> args = TCPSocket::split(token[3], ",");

            double angle = std::stof(args[2]);
            //convert to degres
            angle = angle * 180 / 3.14159265359;

            std::string command = "S " + args[0] + " " + args[1] + " " + angle + "\n";
            int t = this->write_2_arduino(command);
            std::cout << t << std::endl;
        } else if (token[2] == "speed") {
            std::string command = "V " + token[3] + "\n";

            int t = this->write_2_arduino(command);
            std::cout << t << std::endl;
        }
    }
}

void MyTCPClient::init() {
    char errorOpening = this->serial.openDevice(SERIAL_PORT, BAUDS);

    std::cout << "Opening " << SERIAL_PORT << " at " << BAUDS << " bauds" << errorOpening << std::endl;

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
    if (message == "pong") {
        this->sendMessage("arduino;ihm;pong;1");
    } else {
        std::cout << "Received from arduino : " << message << std::endl;
    }
    // TODO handle the response from arduino
}

int MyTCPClient::write_2_arduino(const std::string &message) {
    std::cout << "Write to arduino : " << message << std::endl;
    return serial.writeString(message.c_str());
}

void MyTCPClient::read_from_arduino() {
    while (running) {
        if (serial.isDeviceOpen() && serial.available()) {
            char buffer[MAX_MESSAGE_LEN+1] = {0};
            if (serial.readString(buffer, '\n', MAX_MESSAGE_LEN, TIME_OUT) > 0) {
                handleMessageFromArduino(buffer);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}