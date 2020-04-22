#ifndef TXT_LOWLEVEL_API
#define TXT_LOWLEVEL_API

#include "KeLibTxtDl.h"
#include "FtShmem.h"
#include "utils.h"
#include <stdexcept>
#include <fstream>
#include <chrono>

#define OUTPUT_MAX_LEVEL 512

enum DigitalState
{
    HIGH,
    LOW
};

class IOPin
{
protected:
    uint8_t pin;
    FISH_X1_TRANSFER *pTArea;
    IOPin(FISH_X1_TRANSFER *, uint8_t pin, bool eight);
public:
    uint8_t getPin();
};


/* Output: O1-O8
*/
class Output : public IOPin
{
public:
    Output(FISH_X1_TRANSFER *, uint8_t pin);
    void on();
    void off();
    void setLevel(uint16_t);
};

/* DigitalInput for e.g. Button, Fototransistor or reedcontact: I1-I8
*/
class DigitalInput : public IOPin
{
public:
    DigitalInput(FISH_X1_TRANSFER *, uint8_t pin);
    bool value();
    void waitFor(DigitalState);
};

/*
*/
class Counter : public IOPin
{
public:
    Counter(FISH_X1_TRANSFER *, uint8_t pin);
    bool value();
    uint8_t getPin();
    void waitSteps(uint16_t);
};

/*AnalogInput for NTC-Resistor, Photo-Resistor: I1-I8
*/
class AnalogInput : public IOPin
{
public:
    AnalogInput(FISH_X1_TRANSFER *, uint8_t pin);
    uint16_t value();
};

class NTC : public AnalogInput
{
public:
    NTC(FISH_X1_TRANSFER *, uint8_t pin);
    double getTemperature();
};

/*Ultrasonic: I1-I8
*/
class Ultrasonic : public IOPin
{
public:
    Ultrasonic(FISH_X1_TRANSFER *, uint8_t pin);
    uint16_t value();
};

/*For measure the Voltage (same as the ColorSensor): I1-I8
*/
class Voltage : public IOPin
{
public:
    Voltage(FISH_X1_TRANSFER *, uint8_t pin);
    uint16_t value();
};

/*ColorSensor: I1-I8
*/
class ColorSensor : public Voltage
{
public:
    ColorSensor(FISH_X1_TRANSFER *, uint8_t pin);
    Color color();
};

/*TrackSensor: I1-I8*/
class TrackSensor
{
private:
    FISH_X1_TRANSFER *pTArea;
    uint8_t left;
    uint8_t right;
public:
    TrackSensor(FISH_X1_TRANSFER *, uint8_t left, uint8_t right);
    bool valueLeft();
    bool valueRight();
    uint8_t getPinLeft();
    uint8_t getPinRight();
};

/*Motor: M1-M4*/
class Motor : public IOPin
{
public:
    Motor(FISH_X1_TRANSFER *, uint8_t pin);
    void left(uint16_t level);
    void right(uint16_t level);
    void stop();
};

/*EncoderMotor: M1-M4, C1-C4
*/
class EncoderMotor : public Motor
{
public:
    EncoderMotor(FISH_X1_TRANSFER *, uint8_t pin);
    void distanceLeft(uint16_t steps, uint16_t level);
    void distanceRight(uint16_t steps, uint16_t level);
    void synchronizeTo(EncoderMotor &other);
    void stopSynchronization();
    void waitToEnd();
    uint16_t counter();
    void reset();
};

class TXT
{
private:
    FISH_X1_TRANSFER *pTArea;
    bool _extension;
    TXT(FISH_X1_TRANSFER *, bool);

public:
    TXT();
    ~TXT();
    bool isExtension();
    FISH_X1_TRANSFER *getTransferArea();
    Output output(uint8_t pin);
    Motor motor(uint8_t pin);
    EncoderMotor encoderMotor(uint8_t pin);
    AnalogInput analogInput(uint8_t pin);
    NTC ntc(uint8_t pin);
    DigitalInput digitalInput(uint8_t pin);
    Counter counter(uint8_t pin);
    Ultrasonic ultrasonic(uint8_t pin);
    Voltage voltage(uint8_t pin);
    ColorSensor colorSensor(uint8_t pin);
    TrackSensor trackSensor(uint8_t left, uint8_t right);
    TXT extension();
    void playSound(uint8_t index, uint8_t repeats);
    void playSoundAndWait(uint8_t index, uint8_t repeats);
    uint16_t getTXTVoltage();
};

#endif