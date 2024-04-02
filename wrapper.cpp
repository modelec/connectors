#include "wrapper.h"

void write_2_arduino(serialib* serial, const char* message){
    std::cout << "Writing to arduino: " << message << std::endl;
    char myString[MAX_MESSAGE_LEN] = {0};
    strcpy(myString, message);
    myString[strlen(myString)] = '\0';
    serial->writeString(myString);
}

serialib init_serial(){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    std::cout << "Opening " << SERIAL_PORT << " at " << BAUDS << " bauds" << errorOpening << std::endl;
    if (errorOpening!=1) exit(errorOpening);
    return serial;
}