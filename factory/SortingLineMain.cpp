#include "TXT_highlevel_API.h"

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

void SortWorkpiece();

int main(void)
{
    while (true)
    {
        SortWorkpiece();
    }

    return 0;
}

void SortWorkpiece()
{
    while (light_sensor_start.value())
        ;
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

    comp.on();
    Color active_color = convertToColor(min);
    switch (active_color)
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

    msleep(10);
    comp.off();
    belt.stop();
}