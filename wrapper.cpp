#include "wrapper.h"

void write_2_arduino(serialib* serial, const char* message){
    char myString[MAX_MESSAGE_LEN] = {0};
    strcpy(myString, message);
    myString[strlen(myString)] = '\0';

    std::cout << "Writing to arduino: " << myString << std::endl;
    int t = serial->writeString(myString);

    std::cout << "Result : " << t << std::endl;
}

serialib init_serial(){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    std::cout << "Opening " << SERIAL_PORT << " at " << BAUDS << " bauds" << errorOpening << std::endl;
    if (errorOpening!=1) exit(errorOpening);
    return serial;
}