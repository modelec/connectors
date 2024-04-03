#include "MyTCPClient.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>


int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    client.start();

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

    return 0;
}