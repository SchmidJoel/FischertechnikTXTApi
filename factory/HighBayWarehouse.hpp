#ifndef HIGHBAYWAREHOUSE
#define HIGHBAYWAREHOUSE
#include "TXT_highlevel_API.h"

#define REL_MOVE 50  //move to pull/put workpiece


uint16_t HIGHBAY_X[4] = {760, 1380, 1980, 11};      //0-2: highbay, 3: delivery
uint16_t HIGHBAY_Y[4] = {85, 450, 850, 740};      //0-2: highbay, 3: delivery

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