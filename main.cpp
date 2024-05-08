#include "MyTCPClient.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include <csignal>

std::atomic<bool> shouldStop = false;

void signalHandler( int signum ) {
    shouldStop = true;
}


int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    MyTCPClient client("127.0.0.1", port);

    try{
        client.start();

        while (!client.shouldStop()) {
            usleep(100'000);
        }

        client.stop();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}