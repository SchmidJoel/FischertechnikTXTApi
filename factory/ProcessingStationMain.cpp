#include "TXT_highlevel_API.h"
TXT txt;

TwoRefAxis oven = TwoRefAxis{txt, 5, 10, 9};
TwoRefAxis vacuum_roboter = TwoRefAxis{txt, 6, 5, 11};
NRefAxis table = NRefAxis{txt, 1, std::vector<uint8_t>{1, 2, 3}};

Motor belt = txt.motor(3);
Motor saw = txt.motor(2);

Output oven_gate = txt.output(15);
Output ventil_roboter = txt.output(14);
Output ventil_vacuum = txt.output(13);
Output comp = txt.output(8);
Output oven_light = txt.output(16);
Output table_ventil = txt.output(7);

DigitalInput oven_light_sensor = txt.digitalInput(14);
DigitalInput belt_light_sensor = txt.digitalInput(4);

void ProcessWorkpiece();

int main(void)
{
    std::thread thread1 = oven.pos1Async();
    std::thread thread2 = vacuum_roboter.pos1Async();
    std::thread thread3 = table.referenceAsync();

    thread1.join();
    thread2.join();
    thread3.join();

    while (true)
    {
        ProcessWorkpiece();
    }

    return 0;
}

void ProcessWorkpiece()
{
    while (oven_light_sensor.value());
    comp.on();
    sleep(2);
    std::thread thread2 = vacuum_roboter.pos2Async();
    oven_gate.on();
    oven.pos2();
    oven_gate.off();
    msleep(100);
    for (int i = 0; i < 14; i++)
    {
        oven_light.on();
        msleep(200);
        oven_light.off();
        msleep(200);
    }
    oven_gate.on();
    oven.pos1();

    thread2.join();
    ventil_roboter.on();

    msleep(500);
    ventil_vacuum.on();
    msleep(500);

    ventil_roboter.off();
    vacuum_roboter.pos1();
    ventil_roboter.on();

    msleep(500);
    ventil_vacuum.off();
    msleep(500);

    ventil_roboter.off();
    msleep(100);
    table.pos(1);
    saw.right(OUTPUT_MAX_LEVEL);
    sleep(3);
    saw.stop();
    table.pos(2);

    belt.right(OUTPUT_MAX_LEVEL);
    table_ventil.on();
    msleep(100);
    table_ventil.off();
    comp.off();

    while (belt_light_sensor.value());
    sleep(1);
    belt.stop();
}