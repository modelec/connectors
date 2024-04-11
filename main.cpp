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

    while (!client.shouldStop()) {
        usleep(100'000);
    }

    return 0;
}