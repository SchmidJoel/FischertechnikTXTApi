#ifndef UTILS
#define UTILS

#include <chrono>
#include <thread>

/*Colors for the ColorSensor
*/
enum Color{
    BLUE,
    WHITE,
    RED
};

void sleep(uint32_t);
void msleep(uint32_t);
void usleep(uint32_t);

Color convertToColor(uint16_t);

#endif