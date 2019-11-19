#ifndef FTLib
#define FTLib

#include <unistd.h>
#include <fstream>
#include <iostream>

#include "KeLibTxtDl.h"
#include "FtShmem.h"

#define MAX_LVL 512




class TXT{
    private:
    unsigned int DebugFlags;
    FILE *DebugFile;
    FISH_X1_TRANSFER *pTArea;
    bool isExtension;
    public:
    TXT();
    ~TXT();
    bool isExtension();
    FISH_X1_TRANSFER* getArea();
    Output Output(uint8_t);
    Motor Motor(uint8_t);
    EncoderMotor EncoderMotor(uint8_t,uint8_t);
    AnalogInput AnalogInput(uint8_t);
    DigitalInput DigitalInput(uint8_t);
    Ultrasonic Ultrasonic(uint8_t);
    Voltage Voltage(uint8_t);
    ColorSensor ColorSensor(uint8_t);
    TrackSensor TrackSensor(uint8_t,uint8_t);
    TXT Extension();
};

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
    Input(FISH_X1_TRANSFER*, uint8_t);
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
    ColorSensor(FISH_X1_TRANSFER*,uint8_t,uint8_t);
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
    void left(uint8_t);
    void right(uint8_t);
    void stop();
    uint8_t getPin(); 
};


class EncoderMotor : public Motor{
    private:
    uint8_t c_pin;
    public:
    EncoderMotor(uint8_t, uint8_t);
    void distanceLeft(uint16_t);
    void distanceRight(uint16_t);
    void syncTo(&EncoderMotor);
    uint8_t getC_Pin();
};


#endif