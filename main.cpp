#include <iostream>
#include <time.h>
#include "serialib.h"
#include <limits>
#define SERIAL_PORT "/dev/ttyACM0"
#define MAX_MESSAGE_LEN 1048
#define BAUDS 115200 //vitesse des données (bit/sec)
#define TIME_OUT 500

using namespace std;

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
    int errorCode;
    char myString[MAX_MESSAGE_LEN] = {0};
    strcpy(myString, message);
    myString[strlen(myString)] = '\0';
    do{
        errorCode = serial->writeBytes(myString, strlen(myString));
    }
    while(errorCode != 1);
}

serialib init_serial(){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    if (errorOpening!=1) exit(errorOpening);
    return serial;
}

int main() {
    char  buffer[MAX_MESSAGE_LEN] = {0};
    int nbLines = 1;
    serialib serial = init_serial();
    while (1){//Afficher la position reele avec P, le reste ne renvoie rien
        cout << ">> ";
        fgets(buffer, MAX_MESSAGE_LEN, stdin);
        write_2_arduino(&serial, buffer);
        read_from_arduino(&serial, buffer, nbLines);
        cout << buffer << endl;
        fflush(stdin);
    }/*
    write_2_arduino(&serial, "S 0 0 0\n");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;
    write_2_arduino(&serial, "V 200\n");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;
    write_2_arduino(&serial, "G 200 0\n");
    read_from_arduino(&serial, buffer, nbLines);
    cout << buffer << endl;*/
    serial.closeDevice();
    return 0;
}
