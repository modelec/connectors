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
        serialib serial = client->getSerial();
        int dataAvailable = serial.available();
        if (dataAvailable > 0) {
            char buffer[128] = {0};
            if (serial.readString(buffer, '\n', sizeof(buffer) - 1, 100) > 0) {
                std::lock_guard<std::mutex> guard(dataMutex);
                sharedData = buffer;
                std::cout << "Data received from arduino : " << sharedData << std::endl;
                client->handleMessageFromArduino(sharedData);
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

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