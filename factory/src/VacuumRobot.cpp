#include "VacuumRobot.h"

#define YAXIS_OFFSET 100

VacuumRobot::VacuumRobot(TXT &txt) : xaxis(AxisEM{txt, 5, 9}),
                                     yaxis(AxisEM{txt, 6, 10}),
                                     zaxis(AxisEM{txt, 7, 11}),
                                     compressor(txt.output(15)),
                                     ventil(txt.output(16)) {}

void VacuumRobot::reference()
{
    yaxis.reference();
    std::thread xt = xaxis.referenceAsync();
    std::thread zt = zaxis.referenceAsync();
    xt.join();
    zt.join();
}

std::thread VacuumRobot::referenceAsync()
{
    return std::thread(&VacuumRobot::reference, this);
}

void VacuumRobot::suck()
{
    compressor.on();
    sleep(500ms);
    ventil.on();
}

void VacuumRobot::release()
{
    ventil.off();
    compressor.off();
}

void VacuumRobot::drive(uint16_t x, uint16_t y, uint16_t z)
{
    std::thread xt = xaxis.moveAbsolutAsync(x);
    std::thread zt = zaxis.moveAbsolutAsync(z);
    xt.join();
    zt.join();
    yaxis.moveAbsolut(y);
}