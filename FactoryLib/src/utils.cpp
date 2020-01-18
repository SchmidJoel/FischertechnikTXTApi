#include "utils.hpp"

void sleep(uint32_t seconds){
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void msleep(uint32_t millis){
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void usleep(uint32_t micros){
    std::this_thread::sleep_for(std::chrono::microseconds(micros));
}

Color convertToColor(uint16_t value){
    if(value > 1600){
        return Color::BLUE;
    }
    else if(value > 1200){
        return Color::RED;
    }
    return Color::WHITE;
}