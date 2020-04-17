#ifndef UTILS
#define UTILS

#include <chrono>
#include <thread>

using namespace std::chrono;

/*Colors for the ColorSensor
*/
enum Color
{
    WHITE,
    RED,
    BLUE
};

void sleep(std::chrono::microseconds);

Color convertToColor(uint16_t);

double convertToTemperature(uint16_t);

#endif