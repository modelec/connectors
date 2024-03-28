#ifndef CONNECTOR
#define CONNECTOR


#include <iostream>
#include "serialib.h"
#define SERIAL_PORT "/dev/ttyACM0"
#define MAX_MESSAGE_LEN  64
#define BAUDS 9600 //vitesse des données (bit/sec)
#define TIME_OUT 2000

using namespace std;

void read_from_arduino(char * message_output);

void write_2_arduino(char* message);

#endif
