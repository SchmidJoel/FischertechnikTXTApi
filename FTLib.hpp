#ifndef FTLib
#define FTLib

#include <unistd.h>
#include <fstream>
#include <iostream>

#include "KeLibTxtDl.h"
#include "FtShmem.h"

#define MAX_LVL 512






class Output{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Output(FISH_X1_TRANSFER*,uint8_t);
    void on();
    void off();
    void setLevel(uint16_t);
    uint8_t getPin();    
};

class DigitalInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    DigitalInput(FISH_X1_TRANSFER*, uint8_t);
    bool value();
    uint8_t getPin();    
};

class AnalogInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    AnalogInput(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    uint8_t getPin();    
};

class Ultrasonic{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Ultrasonic(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    uint8_t getPin();    
};

enum Color{
    BLUE,
    WHITE,
    RED
};

class ColorSensor{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    ColorSensor(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    Color color();
    uint8_t getPin();
};

class Voltage{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Voltage(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    uint8_t getPin();
};

class TrackSensor{
    private:
    uint8_t left;
    uint8_t right;
    FISH_X1_TRANSFER* pTArea;
    public:
    TrackSensor(FISH_X1_TRANSFER*,uint8_t,uint8_t);
    bool valueLeft();
    bool valueRight();
    uint8_t getPinLeft();
    uint8_t getPinRight();
};

class Motor{
    protected:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Motor(FISH_X1_TRANSFER* ,uint8_t);
    void left(uint16_t);
    void right(uint16_t);
    void stop();
    uint8_t getPin(); 
};


class EncoderMotor : public Motor{
    private:
    uint8_t c_pin;
    public:
    EncoderMotor(FISH_X1_TRANSFER* ,uint8_t, uint8_t);
    void distanceLeft(uint16_t);
    void distanceRight(uint16_t);
    void syncTo(EncoderMotor&);
    uint8_t getC_Pin();
};


class TXT{
    private:
    FISH_X1_TRANSFER *pTArea;
    bool _extension;
    public:
    TXT();
    ~TXT();
    bool isExtension();
    FISH_X1_TRANSFER* getArea();
    Output output(uint8_t);
    Motor motor(uint8_t);
    EncoderMotor encoderMotor(uint8_t,uint8_t);
    AnalogInput analogInput(uint8_t);
    DigitalInput digitalInput(uint8_t);
    Ultrasonic ultrasonic(uint8_t);
    Voltage voltage(uint8_t);
    ColorSensor colorSensor(uint8_t);
    TrackSensor trackSensor(uint8_t,uint8_t);
    TXT extension();
};


#endif