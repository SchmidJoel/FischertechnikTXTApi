#include "HighBayWarehouse.h"

uint16_t HIGHBAY_X[4] = {760, 1380, 1980, 11};      //0-2: highbay, 3: delivery
uint16_t HIGHBAY_Y[4] = {85, 450, 850, 740};      //0-2: highbay, 3: delivery

HighbayWarehouse::HighbayWarehouse(TXT &txt) : 
        xaxis(AxisEM{txt, 2, 5}), 
        yaxis(AxisEM{txt, 4, 8}),
        zaxis(TwoRefAxis{txt, 3, 6, 7}) {}

void HighbayWarehouse::reference(){
    zaxis.pos1();
    std::thread xt = xaxis.referenceAsync();
    std::thread yt = yaxis.referenceAsync();
    xt.join();
    yt.join();
}

void HighbayWarehouse::drive(uint8_t x, uint8_t y){
    std::thread xt = xaxis.moveAbsolutAsync(HIGHBAY_X[x]);
    std::thread yt = yaxis.moveAbsolutAsync(HIGHBAY_Y[y]);
    xt.join();
    yt.join();
}

void HighbayWarehouse::pull(){
    zaxis.pos2();
    yaxis.moveRelative(-REL_MOVE);
    zaxis.pos1();
}

void HighbayWarehouse::put(){
    yaxis.moveRelative(-REL_MOVE);
    zaxis.pos2();
    yaxis.moveRelative(REL_MOVE);
    zaxis.pos1();
}