#include "wrapper.h"

void write_2_arduino(serialib* serial, const char* message){
    // std::cout << "Writing to arduino: " << myString << std::endl;
    int t = serial->writeString(message);

    std::cout << "Result : " << t << std::endl;
}

serialib init_serial(){
    serialib serial;
    char errorOpening = serial.openDevice(SERIAL_PORT, BAUDS);
    std::cout << "Opening " << SERIAL_PORT << " at " << BAUDS << " bauds" << errorOpening << std::endl;
    if (errorOpening!=1) exit(errorOpening);
    return serial;
}