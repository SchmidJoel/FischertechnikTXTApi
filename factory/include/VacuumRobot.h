#ifndef VACUUMROBOT
#define VACUUMROBOT
#include "TXT_highlevel_API.h"

#define BLUE_PICK_UP_X 316
#define RED_PICK_UP_X 387
#define WHITE_PICK_UP_X 466
#define BLUE_PICK_UP_Z 588
#define RED_PICK_UP_Z 434
#define WHITE_PICK_UP_Z 380
#define PICK_UP_Y 820

#define WAREHOUSE_X 1401
#define WAREHOUSE_Y 140
#define WAREHOUSE_Z 195

#define PROCESS_STATION_X 930
#define PROCESS_STATION_Y 550
#define PROCESS_STATION_Z 880

class VacuumRobot
{
private:
    Output compressor;
    Output ventil;

public:
    VacuumRobot(TXT &txt);
    AxisEM xaxis;
    AxisEM yaxis;
    AxisEM zaxis;
    void reference();
    std::thread referenceAsync();
    void drive(uint16_t x, uint16_t y, uint16_t z);
    void suck();
    void release();
};

#endif