#include "utils.h"
#include "math.h"

void sleep(std::chrono::microseconds micros)
{
    std::this_thread::sleep_for(micros);
}

Color convertToColor(uint16_t value)
{
    if (value < 500)
    {
        return Color::WHITE;
    }
    else if (value < 1300)
    {
        return Color::RED;
    }
    return Color::BLUE;
}

double convertToTemperature(uint16_t value){
    return log(value) * log(value) * 1.3932 + log(value) * -43.942 + 271.87;
}