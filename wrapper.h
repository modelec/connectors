#ifndef CONNECTOR
#define CONNECTOR

#include <iostream>
#include "serialib.h"

#define SERIAL_PORT "/dev/ttyACM0"
#define MAX_MESSAGE_LEN  64
#define BAUDS 115200 //vitesse des données (bit/sec)
#define TIME_OUT 2000

using namespace std;

void write_2_arduino(serialib* serial, const char* message);

serialib init_serial();

#endif
