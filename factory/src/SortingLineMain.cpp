#include "TXT_highlevel_API.h"
#include "TxtMqttFactoryClient.h"
#include <thread>

enum SortingLineState
{
    WAITING,
    WORKING
};

TXT txt;
TxtMqttFactoryClient mqttClient("SortingLine", "192.168.178.66", "", "");

DigitalInput light_sensor_start = txt.digitalInput(1);
DigitalInput light_sensor_end = txt.digitalInput(3);
Motor belt = txt.motor(1);
Counter counter = txt.counter(1);
ColorSensor color_sensor = txt.colorSensor(2);
Output comp = txt.output(8);
Output white = txt.output(5);
Output red = txt.output(6);
Output blue = txt.output(7);

SortingLineState colorDetectionUnit = SortingLineState::WAITING;
SortingLineState sortingUnit = SortingLineState::WAITING;

void SortWorkpiece(Color color);
void ColorDetection();

int main(void)
{
    mqttClient.connect(1000);

    std::thread detection = std::thread(ColorDetection);

    while (true)
    {
        if (colorDetectionUnit == SortingLineState::WORKING || sortingUnit == SortingLineState::WORKING)
        {
            belt.right(512);
        }
        else
        {
            belt.stop();
            mqttClient.publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "bereit");
        }
        sleep(50ms);
    }

    return 0;
}

void ColorDetection()
{
    while (true)
    {
        light_sensor_start.waitFor(DigitalState::LOW);
        colorDetectionUnit = SortingLineState::WORKING;
        mqttClient.publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "Farbe erkennen");

        int min = color_sensor.value();
        while (light_sensor_end.value())
        {
            sleep(10ms);
            if (color_sensor.value() < min)
            {
                min = color_sensor.value();
            }
        }

        mqttClient.publishMessageAsync(TOPIC_INPUT_SORTINGLINE_LAST_COLOR, std::to_string(convertToColor(min)));
        mqttClient.publishMessageAsync(TOPIC_INPUT_SORTINGLINE_RAW_LAST_COLOR, std::to_string(min));

        std::thread sort = std::thread(SortWorkpiece, convertToColor(min));
        sort.detach();
        colorDetectionUnit = SortingLineState::WAITING;
    }
}

void SortWorkpiece(Color color)
{
    sortingUnit = SortingLineState::WORKING;
    mqttClient.publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "Werkstück sortieren");

    comp.on();
    switch (color)
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
    sleep(100ms);
    white.off();
    red.off();
    blue.off();
    comp.off();
    sortingUnit = SortingLineState::WAITING;
}