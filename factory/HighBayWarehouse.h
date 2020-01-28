#ifndef HIGHBAYWAREHOUSE
#define HIGHBAYWAREHOUSE
#include "TXT_highlevel_API.h"

#define REL_MOVE 50  //move to pull/put workpiece

class HighbayWarehouse {
private:
    AxisEM xaxis;
    AxisEM yaxis;
    TwoRefAxis zaxis;

public:
    HighbayWarehouse(TXT &txt);
    void reference();
    void drive(uint8_t x, uint8_t y);
    void pull();
    void put();
};

#endif