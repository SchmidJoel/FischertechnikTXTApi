#ifndef UTILS
#define UTILS

#include <chrono>
#include <thread>
#include <string>
#include "math.h"
#include "json/json.h"

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

std::string jsonToString(Json::Value obj);

std::string jsonToStyledString(Json::Value obj);

#endif