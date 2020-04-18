#include "debug.h"

std::string txtStateObject(TXT &txt) {
    Json::Value object;
    FISH_X1_TRANSFER *pTArea = txt.getTransferArea();

    Json::Value input;
    std::stringstream ss;
    for (int i = 0; i < 8; i++) {
        ss << "i" << i;
        input[ss.str()] = pTArea->ftX1in.uni[i];
        ss.str("");
    }
    object["input"] = input;

    Json::Value output;
    for (int i = 0; i < 8; i++) {
        ss << "o" << i;
        output[ss.str()] = pTArea->ftX1out.duty[i];
        ss.str("");
    }
    object["output"] = output;

    object["voltage"] = txt.getTXTVoltage();

    object["extension"] = txt.isExtension();

    return jsonToString(object);
}
