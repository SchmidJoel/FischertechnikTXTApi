#ifndef HIGHBAYWAREHOUSE
#define HIGHBAYWAREHOUSE
#include "TXT_highlevel_API.h"
#include "WarehouseStorage.h"

#define REL_MOVE 60 //move to pull/put workpiece

enum HighBayState
{
    H_UNREFERENCED,
    H_REFERENCING,
    H_STORE_WORKPIECE,
    H_PROVIDE_WORKPIECE,
    H_READY
};

class HighbayWarehouse
{
private:
    AxisEM xaxis;
    AxisEM yaxis;
    TwoRefAxis zaxis;
public:
    HighbayWarehouse(TXT &txt);
    void reference();
    std::thread referenceAsync();
    void drive(uint8_t x, uint8_t y);
    void pull(bool = false);
    void put(bool = false);
    HighBayState state = HighBayState::H_UNREFERENCED;
    FileStorage storage;
};

#endif