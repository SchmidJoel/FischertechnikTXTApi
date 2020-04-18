#include "debug.h"

Json::StreamWriterBuilder writer;

std::string txtStateObject(TXT &txt) {
    Json::Value object;
    FISH_X1_TRANSFER *pTArea = txt.getTransferArea();

    Json::Value input;
    for (int i = 0; i < 8; i++) {
        input["i" + i] = pTArea->ftX1in.uni[i];
    }
    object["input"] = input;

    Json::Value output;
    for (int i = 0; i < 8; i++) {
        input["o" + i] = pTArea->ftX1out.duty[i];
    }
    object["output"] = output;

    object["voltage"] = txt.getTXTVoltage();

    object["extension"] = txt.isExtension();

    return Json::writeString(writer, object);
}
