#include "TXT_highlevel_API.h"
#include <queue>
#include <thread>

enum SortingLineState{
    WAITING,
    WORKING
};

TXT txt;

DigitalInput light_sensor_start = txt.digitalInput(1);
DigitalInput light_sensor_end = txt.digitalInput(3);
Motor belt = txt.motor(1);
Counter counter = txt.counter(1);
ColorSensor color_sensor = txt.colorSensor(2);
Output comp = txt.output(8);
Output white = txt.output(5);
Output red = txt.output(6);
Output blue = txt.output(7);
std::queue<Color> color_queue;

SortingLineState colorDetectionUnit = SortingLineState::WAITING;
SortingLineState sortingUnit = SortingLineState::WAITING;

void SortWorkpiece();

int main(void)
{
    std::thread(ColorDetection);
    std::thread(SortWorkpiece);

    while (true)
    {
        if(colorDetectionUnit == SortingLineState::WORKING || sortingUnit == SortingLineState::WORKING){
            belt.right(512);
        }
        else{
            belt.stop();
        }
    }

    return 0;
}

void ColorDetection(){
    while(true){
        light_sensor_start.waitFor(DigitalState::LOW);
        colorDetectionUnit = SortingLineState::WORKING;
        belt.right(512);

        int min = color_sensor.value();
        while (light_sensor_end.value())
        {
            msleep(10);
            if (color_sensor.value() < min)
            {
                min = color_sensor.value();
            }
        }
        color_queue.push(convertToColor(min));
        colorDetectionUnit = SortingLineState::WAITING;
    }    
}

void SortWorkpiece()
{
    while(true){
        while(color_queue.size());
        while(!light_sensor_end.value());
        sortingUnit = SortingLineState::WORKING;
        
        comp.on();
        switch (color_queue.front())
        {
        case Color::WHITE:
            counter.waitSteps(6);
            white.on();
            break;
        case Color::RED:
            counter.waitSteps(16);
            red.on();
            break;
        case Color::BLUE:
            counter.waitSteps(26);
            blue.on();
            break;
        }
        msleep(100);
        white.off();
        red.off();
        blue.off();
        comp.off();
        sortingUnit = SortingLineState::WAITING;
    }
}