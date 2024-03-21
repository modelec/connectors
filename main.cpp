#include "main.h"


void read_from_arduino(char * message_output){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    if (errorOpening!=1) exit(errorOpening);
    serial.readString(message_output, '\n', MAX_MESSAGE_LEN, TIME_OUT);
    serial.closeDevice();
}

void write_2_arduino(char* message){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    if (errorOpening!=1) exit(errorOpening);
    serial.writeString(message);
    serial.closeDevice();
}

int main() {
    char  buffer[MAX_MESSAGE_LEN];
    cout << "Message to send: ";
    cin >> buffer;
    write_2_arduino(buffer);
    read_from_arduino(buffer);
    cout << buffer;
    return 0;
}
