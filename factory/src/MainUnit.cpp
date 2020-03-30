#include "VacuumRobot.h"
#include "HighBayWarehouse.h"
#include "TxtMqttFactoryClient.h"

enum BeltState
{
    WAREHOUSE,
    VACUUM_ROBOT,
    AT_WAREHOUSE,
    AT_VACUUM_ROBOT
};

TXT txt;
TxtMqttFactoryClient mqttClient("MainUnit", "192.168.178.66", "", "");

VacuumRobot robot = VacuumRobot(txt);
HighbayWarehouse warehouse = HighbayWarehouse(txt);

Motor belt = txt.motor(1);
DigitalInput light_sensor_vacuum_robot = txt.digitalInput(1);
DigitalInput light_sensor_warehouse = txt.digitalInput(4);
TrackSensor belt_track_sensor = txt.trackSensor(2, 3);

DigitalInput white_available = txt.digitalInput(12);
DigitalInput red_available = txt.digitalInput(13);
DigitalInput blue_available = txt.digitalInput(14);

BeltState beltstate = BeltState::WAREHOUSE;
//todo soll/ist state für belt

void driveToWarehouse(Color);
void driveToProcessing();
void storeWorkpieceHighBay(uint8_t, uint8_t, int);
void getWorkpieceHighBay(uint8_t, uint8_t);
void checkAvailableWorkpieces();
void getEmptyBox(Color);
void storeBox(WarehouseContent);
void getFullBox();

int main()
{
    mqttClient.connect(1000);

    std::thread thread_vacuum = robot.referenceAsync();
    std::thread thread_warehouse = warehouse.referenceAsync();

    mqttClient.publishMessageAsync(TOPIC_INPUT_STOCK, warehouse.storage.getAsJson());
    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "referenzieren");
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "referenzieren");

    thread_vacuum.join();
    thread_warehouse.join();
    warehouse.state = HighBayState::H_READY;
    robot.state = VacuumRobotState::V_READY;

    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "bereit");
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "bereit");

    std::thread run = std::thread(checkAvailableWorkpieces);
    run.detach();

    while (true)
    {
        if (beltstate == BeltState::WAREHOUSE && !light_sensor_vacuum_robot.value())
        {
            belt.left(450);
            light_sensor_warehouse.waitFor(DigitalState::LOW);
            belt.stop();
            beltstate = BeltState::AT_WAREHOUSE;
        }
        else if (beltstate == BeltState::VACUUM_ROBOT && !light_sensor_warehouse.value())
        {
            belt.right(450);
            light_sensor_vacuum_robot.waitFor(DigitalState::LOW);
            belt.stop();
            beltstate = BeltState::AT_VACUUM_ROBOT;
        }
        sleep(10ms);
    }

    return 0;
}

void checkAvailableWorkpieces()
{
    while (true)
    {
        if (!white_available.value())
        {
            std::thread wait = std::thread([] {
                while (warehouse.state != HighBayState::H_READY)
                {
                    sleep(10ms);
                }
                getEmptyBox(Color::WHITE);
            });
            driveToWarehouse(Color::WHITE);
            wait.join();
            storeBox(WarehouseContent::WHITE);
        }
        else if (!red_available.value())
        {
            std::thread wait = std::thread([] {
                while (warehouse.state != HighBayState::H_READY)
                {
                    sleep(10ms);
                }
                getEmptyBox(Color::RED);
            });
            driveToWarehouse(Color::RED);
            wait.join();
            storeBox(WarehouseContent::RED);
        }
        else if (!blue_available.value())
        {
            std::thread wait = std::thread([] {
                while (warehouse.state != HighBayState::H_READY)
                {
                    sleep(10ms);
                }
                getEmptyBox(Color::BLUE);
            });
            driveToWarehouse(Color::BLUE);
            wait.join();
            storeBox(WarehouseContent::BLUE);
        }
        else
        {
            getFullBox();
            while (beltstate != BeltState::AT_VACUUM_ROBOT)
            {
                sleep(10ms);
            }
            std::thread processing = std::thread(driveToProcessing);
            while (beltstate != BeltState::AT_WAREHOUSE)
            {
                sleep(10ms);
            }
            storeBox(WarehouseContent::EMPTY_BOX);
            processing.join();
        }
    }
}

void driveToWarehouse(Color color)
{
    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "holen");
    robot.yaxis.moveAbsolut(0);
    if (color == Color::WHITE)
    {
        robot.drive(WHITE_PICK_UP_X, PICK_UP_Y, WHITE_PICK_UP_Z);
    }
    else if (color == Color::RED)
    {
        robot.drive(RED_PICK_UP_X, PICK_UP_Y, RED_PICK_UP_Z);
    }
    else if (color == Color::BLUE)
    {
        robot.drive(BLUE_PICK_UP_X, PICK_UP_Y, BLUE_PICK_UP_Z);
    }
    robot.suck();
    robot.yaxis.moveAbsolut(0);
    robot.drive(WAREHOUSE_X, 0, 0);
    robot.zaxis.moveAbsolut(WAREHOUSE_Z);
    while (beltstate != BeltState::AT_VACUUM_ROBOT)
    {
        sleep(10ms);
    }
    robot.yaxis.moveAbsolut(WAREHOUSE_Y);
    robot.release();
    robot.yaxis.moveAbsolut(0);
    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "bereit");
}

void driveToProcessing()
{
    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "abliefern");
    robot.yaxis.moveAbsolut(0);
    robot.drive(WAREHOUSE_X, WAREHOUSE_Y, WAREHOUSE_Z);
    robot.suck();
    robot.yaxis.moveAbsolut(0);
    beltstate = BeltState::WAREHOUSE;
    robot.zaxis.moveAbsolut(0);
    robot.xaxis.moveAbsolut(PROCESS_STATION_X);
    robot.drive(PROCESS_STATION_X, PROCESS_STATION_Y, PROCESS_STATION_Z);
    robot.release();
    std::thread xaxis = robot.yaxis.moveAbsolutAsync(0);
    robot.zaxis.moveAbsolut(0);
    xaxis.join();
    mqttClient.publishMessageAsync(TOPIC_INPUT_VACUUMROBOT_STATE, "bereit");
}

void storeWorkpieceHighBay(uint8_t x, uint8_t y, WarehouseContent content)
{
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "einlagern");
    warehouse.state = HighBayState::H_STORE_WORKIECE;
    beltstate = BeltState::WAREHOUSE;
    warehouse.drive(3, 3);
    while (beltstate != BeltState::AT_WAREHOUSE)
    {
        sleep(10ms);
    }
    warehouse.pull();
    warehouse.drive(x, y);
    warehouse.put();
    warehouse.storage.setWorkpieceAt(y * 3 + x, content);

    mqttClient.publishMessageAsync(TOPIC_INPUT_STOCK, warehouse.storage.getAsJson());

    warehouse.state = HighBayState::H_READY;
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "bereit");
}

void getEmptyBox(Color color)
{
    int pos = warehouse.storage.getPositionOf(WarehouseContent::EMPTY_BOX);
    if (pos != -1)
    {
        int x = pos % 3;
        int y = pos / 3;
        getWorkpieceHighBay(x, y);
    }
}

void storeBox(WarehouseContent content)
{
    int pos = warehouse.storage.getPositionOf(WarehouseContent::NO_BOX);
    if (pos != -1)
    {
        int x = pos % 3;
        int y = pos / 3;
        storeWorkpieceHighBay(x, y, content);
    }
}

void getWorkpieceHighBay(uint8_t x, uint8_t y)
{
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "auslagern");
    warehouse.state = HighBayState::H_PROVIDE_WORKPIECE;
    warehouse.drive(x, y);
    warehouse.pull();
    warehouse.storage.setWorkpieceAt(y * 3 + x, WarehouseContent::NO_BOX);

    mqttClient.publishMessageAsync(TOPIC_INPUT_STOCK, warehouse.storage.getAsJson());

    warehouse.drive(3, 3);
    warehouse.put(true);
    warehouse.state = HighBayState::H_READY;
    beltstate = BeltState::VACUUM_ROBOT;
    mqttClient.publishMessageAsync(TOPIC_INPUT_WAREHOUSE_STATE, "bereit");
}

void getFullBox()
{
    int x = 0;
    int y = 0;
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        if ((int)warehouse.storage.getWorkpieceAt(i) > 0)
        {
            x = i % 3;
            y = i / 3;
            break;
        }
    }
    getWorkpieceHighBay(x, y);
}
