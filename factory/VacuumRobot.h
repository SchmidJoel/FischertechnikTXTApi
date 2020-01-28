#ifndef VACUUMROBOT
#define VACUUMROBOT
#include "TXT_highlevel_API.h"

#define BLUE_PICK_UP_X 316
#define RED_PICK_UP_X 387
#define WHITE_PICK_UP_X 466
#define BLUE_PICK_UP_Z 588
#define RED_PICK_UP_Z 434
#define WHITE_PICK_UP_Z 380
#define PICK_UP_Y 800

#define WAREHOUSE_X 1400
#define WAREHOUSE_Y 110
#define WAREHOUSE_Z 170

#define PROCESS_STATION_X 920
#define PROCESS_STATION_Y 500
#define PROCESS_STATION_Z 880


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