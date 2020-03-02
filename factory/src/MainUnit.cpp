#include "VacuumRobot.h"
#include "HighBayWarehouse.h"

TXT txt;

VacuumRobot robot = VacuumRobot(txt);
HighbayWarehouse warehouse = HighbayWarehouse(txt);

Motor belt = txt.motor(1);
DigitalInput light_sensor_vacuum_robot = txt.digitalInput(1);
DigitalInput light_sensor_warehouse = txt.digitalInput(4);
TrackSensor belt_track_sensor = txt.trackSensor(2, 3);

DigitalInput white_available = txt.digitalInput(12);
DigitalInput red_available = txt.digitalInput(13);
DigitalInput blue_available = txt.digitalInput(14);

void Run();

int main()
{
    robot.reference();
    warehouse.reference();

    while (true)
    {
        Run();
    }

    return 0;
}

void Run()
{
    robot.drive(WHITE_PICK_UP_X, PICK_UP_Y, WHITE_PICK_UP_Z);
    sleep(50ms);
    robot.suck();
    robot.drive(WHITE_PICK_UP_X, PICK_UP_Y - 200, WHITE_PICK_UP_Z);
    robot.drive(WHITE_PICK_UP_X, 0, 0);
    robot.drive(WAREHOUSE_X, 0, 0);
    robot.drive(WAREHOUSE_X, 0, WAREHOUSE_Z);
    robot.drive(WAREHOUSE_X, WAREHOUSE_Y, WAREHOUSE_Z);

    sleep(100ms);
    robot.release();
    sleep(100ms);
    robot.drive(WAREHOUSE_X, 0, 0);

    belt.left(OUTPUT_MAX_LEVEL);
    light_sensor_warehouse.waitFor(DigitalState::LOW);
    belt.stop();

    warehouse.drive(3, 3);
    warehouse.pull();
    warehouse.drive(0, 0);
    warehouse.put();

    warehouse.reference();

    warehouse.drive(0, 0);
    warehouse.pull();
    warehouse.drive(3, 3);
    warehouse.put();

    belt.right(OUTPUT_MAX_LEVEL);
    light_sensor_vacuum_robot.waitFor(DigitalState::LOW);
    belt.stop();

    robot.drive(WAREHOUSE_X, WAREHOUSE_Y, WAREHOUSE_Z);
    robot.suck();
    sleep(100ms);
    robot.drive(WAREHOUSE_X, 0, 0);
    robot.drive(PROCESS_STATION_X, PROCESS_STATION_Y, PROCESS_STATION_Z);
    sleep(100ms);
    robot.release();
    robot.drive(PROCESS_STATION_X, PROCESS_STATION_Y - 100, PROCESS_STATION_Z);
    robot.reference();
}