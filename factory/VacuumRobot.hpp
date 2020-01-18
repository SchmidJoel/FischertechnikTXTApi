#ifndef VACUUMROBOT
#define VACUUMROBOT
#include "TXT_highlevel_API.h"


class VacuumRobot {
private:
    AxisEM xaxis;
    AxisEM yaxis;
    AxisEM zaxis;
    Output compressor;
    Output ventil;

public:
    VacuumRobot(TXT &txt);
    void reference();
    void drive(uint16_t x, uint16_t y, uint16_t z);
    void suck();
    void release();

};

#endif