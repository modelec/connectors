#include "MyTCPClient.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>

std::atomic<bool> keepRunning(true);
std::mutex dataMutex;
std::string sharedData;

void serialReadThread(MyTCPClient* client) {
    while (keepRunning) {
        // std::cout << "Checking for data from arduino..." << std::endl;
        serialib* serial = client->getSerial();
        int dataAvailable = serial->available();
        std::cout << dataAvailable << std::endl;
        // if (dataAvailable > 0) {
            // std::cout << "Data available from arduino : " << dataAvailable << " bytes" << std::endl;
            char buffer[128] = {0};
            if (serial->readString(buffer, '\n', MAX_MESSAGE_LEN, TIME_OUT) > 0) {
                std::lock_guard<std::mutex> guard(dataMutex);
                sharedData = buffer;
                std::cout << "Data received from arduino : " << sharedData << std::endl;
                client->handleMessageFromArduino(sharedData);
            }
        // } else {
            // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        // }
    }
}

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    client.start();

    client.init();

    std::thread readThread(serialReadThread, &client);

    while (true) {
        std::string message;
        std::cout << "Enter message ('quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") {
            client.stop();
            break;
        }

        client.sendMessage(message.c_str());
    }

    readThread.join();

    client.stop();

    return 0;
}