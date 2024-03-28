#include <iostream>
#include <time.h>
#include "serialib.h"
#define SERIAL_PORT "/dev/ttyACM0"
#define MAX_MESSAGE_LEN 64
#define BAUDS 115200 //vitesse des données (bit/sec)
#define TIME_OUT 2000

using namespace std;

void my_delay(int i)    /*Pause l'application pour i seconds*/
{
    clock_t start,end;
    start=clock();
    while(((end=clock())-start)<=i*CLOCKS_PER_SEC);
}

void read_from_arduino(serialib* serial, char * message_output, int nbLines2Rcv){
    strncpy(message_output, "", strlen(message_output));
    char buffer[MAX_MESSAGE_LEN+1] = {0};
    while(nbLines2Rcv-- != 0){
        serial->readString(buffer, '\n', MAX_MESSAGE_LEN, TIME_OUT);
        strcat(message_output, buffer);
        strncpy(buffer, "", strlen(buffer));
    }
}

void write_2_arduino(serialib* serial, char* message){
    char myString[MAX_MESSAGE_LEN] = {0};
    strcpy(myString, message);
    myString[strlen(myString)] = '\n';
    serial->writeString(myString);
}

serialib init_serial(){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    if (errorOpening!=1) exit(errorOpening);
    return serial;
}

int main() {
    char  buffer[MAX_MESSAGE_LEN] = {0};
    int nbLines = 2;
    serialib serial = init_serial();
    while (1){
        cout << ">> ";
        cin >> buffer;
        write_2_arduino(&serial, buffer);
        read_from_arduino(&serial, buffer, nbLines);
        cout << buffer << endl;
        //my_delay(nbLines*TIME_OUT);
    }/*
    write_2_arduino(&serial, "S 1500 500 0");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;
    write_2_arduino(&serial, "V 200");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;
    write_2_arduino(&serial, "G 500 1000");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;*/
    serial.closeDevice();
    return 0;
}
