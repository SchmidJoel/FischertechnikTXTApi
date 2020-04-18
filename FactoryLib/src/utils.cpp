#include "utils.h"
#include "math.h"
#include <chrono>

void sleep(std::chrono::microseconds micros)
{
    std::this_thread::sleep_for(micros);
}

Color convertToColor(uint16_t value)
{
    if (value > 1600)
    {
        return Color::BLUE;
    }
    else if (value > 1200)
    {
        return Color::RED;
    }
    return Color::WHITE;
}

double convertToTemperature(uint16_t value){
    return log(value) * log(value) * 1.3932 + log(value) * -43.942 + 271.87;
}