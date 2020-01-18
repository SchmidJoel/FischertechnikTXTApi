#include "VacuumRobot.hpp"

VacuumRobot::VacuumRobot(TXT &txt) : 
        xaxis(AxisEM{txt, 5, 9}),
        yaxis(AxisEM{txt, 6, 10}),
        zaxis(AxisEM{txt, 7, 11}),
        compressor(txt.output(1)),
        ventil(txt.output(2)) {}    //todo pins anpassen

void VacuumRobot::reference(){
    yaxis.reference();
    std::thread xt = xaxis.referenceAsync();
    std::thread zt = zaxis.referenceAsync();
    xt.join();
    zt.join();
}

void VacuumRobot::suck(){
    compressor.on();
    sleep(1);
    ventil.on();
}

void VacuumRobot::release(){
    compressor.off();
    ventil.off();
}

void VacuumRobot::drive(uint16_t x, uint16_t y, uint16_t z){
    std::thread xt = xaxis.moveAbsolutAsync(x);
    std::thread zt = zaxis.moveAbsolutAsync(z);
    xt.join();
    zt.join();
    yaxis.moveAbsolutAsync(y);
}