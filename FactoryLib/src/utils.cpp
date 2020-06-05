#include "utils.h"

Json::StreamWriterBuilder writer;

void sleep(std::chrono::microseconds micros)
{
    std::this_thread::sleep_for(micros);
}

Color convertToColor(uint16_t value, int blue_lower, int red_lower)
{
    if (value >= blue_lower)
    {
        return Color::BLUE;
    }
    else if (value >= red_lower)
    {
        return Color::RED;
    }
    return Color::WHITE;
}

double convertToTemperature(uint16_t value) {
    return log(value) * log(value) * 1.3932 + log(value) * -43.942 + 271.87;
}

std::string jsonToString(Json::Value obj) {
    writer["indentation"] = "";
    return Json::writeString(writer, obj);
}

std::string jsonToStyledString(Json::Value obj) {
    writer["indentation"] = "\t";
    return Json::writeString(writer, obj);
}