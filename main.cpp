#include "MyTCPClient.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <csignal>
#include <Modelec/CLParser.h>

std::atomic<bool> shouldStop = false;

void signalHandler( int signum ) {
    shouldStop = true;
}


int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);

    CLParser clParser(argc, argv);

    int port = clParser.getOption<int>("port", 8080);

    auto host = clParser.getOption("host", "127.0.0.1");

    MyTCPClient client(host, port);

    try{
        client.start();

        while (!client.shouldStop() && !shouldStop) {
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