#include "TXT_highlevel_API.h"
#include "TxtMqttFactoryClient.h"
#include <thread>
#include "debug.h"
#include "config.h"

#define DEBUG_SORTINGLINE false

enum SortingLineState
{
    WAITING,
    WORKING
};

TXT txt;
TxtMqttFactoryClient* mqttClient;

DigitalInput light_sensor_start = txt.digitalInput(1);
DigitalInput light_sensor_end = txt.digitalInput(3);
Motor belt = txt.motor(1);
Counter counter = txt.counter(1);
ColorSensor color_sensor = txt.colorSensor(2);
Output comp = txt.output(8);
Output white = txt.output(5);
Output red = txt.output(6);
Output blue = txt.output(7);

// Monitor
NTC motorTemperture = txt.ntc(4);
Voltage motorVoltage = txt.voltage(5);

SortingLineState colorDetectionUnit = SortingLineState::WAITING;
SortingLineState sortingUnit = SortingLineState::WAITING;

void SortWorkpiece(Color color);
void ColorDetection();

int main(void)
{
    readConfig();
    mqttClient = new TxtMqttFactoryClient("SortingLine", ip_adress, "", "");
    mqttClient->connect(1000);

    std::thread debug;
    if (DEBUG_SORTINGLINE) {
        debug = std::thread([]() {
            while (true)
            {
                mqttClient->publishMessageAsync(TOPIC_DEBUG_SORTINGLINE, txtStateObject(txt));
                sleep(250ms);
            }
        });
        debug.detach();
    }

    // start monitor thread
    std::thread monitor = std::thread([] {
        while (true)
        {
            mqttClient->publishMessageAsync(TOPIC_MONITOR_SL_M1_VOLTAGE, std::to_string(motorVoltage.value()));
            mqttClient->publishMessageAsync(TOPIC_MONITOR_SL_M1_TEMPERATURE, std::to_string(motorTemperture.getTemperature()));
            sleep(500ms);
        }
    });
    monitor.detach();

    std::thread detection = std::thread(ColorDetection);
    mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_LAST_COLOR, "");
    mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_RAW_LAST_COLOR, "");    

    while (true)
    {
        if (colorDetectionUnit == SortingLineState::WORKING || sortingUnit == SortingLineState::WORKING)
        {
            belt.right(512);
        }
        else
        {
            belt.stop();
            mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "bereit", 0, true);
        }
        sleep(50ms);
    }
    delete mqttClient;
    return 0;
}

void ColorDetection()
{
    while (true)
    {
        light_sensor_start.waitFor(DigitalState::LOW);
        colorDetectionUnit = SortingLineState::WORKING;
        mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "Farbe erkennen", DFLT_QUALITY_OF_SERVICE, true);

        int min = color_sensor.value();
        while (light_sensor_end.value())
        {
            sleep(10ms);
            if (color_sensor.value() < min)
            {
                min = color_sensor.value();
            }
        }

        mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_LAST_COLOR, std::to_string(convertToColor(min, blue_lower, red_lower)));
        mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_RAW_LAST_COLOR, std::to_string(min));

        std::thread sort = std::thread(SortWorkpiece, convertToColor(min, blue_lower, red_lower));
        sort.detach();
        colorDetectionUnit = SortingLineState::WAITING;
    }
}

void SortWorkpiece(Color color)
{
    sortingUnit = SortingLineState::WORKING;
    mqttClient->publishMessageAsync(TOPIC_INPUT_SORTINGLINE_STATE, "Werkstück sortieren", DFLT_QUALITY_OF_SERVICE, true);

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