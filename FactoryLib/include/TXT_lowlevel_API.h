#ifndef TXT_LOWLEVEL_API
#define TXT_LOWLEVEL_API

#include "KeLibTxtDl.h"
#include "FtShmem.h"
#include "utils.h"

#define OUTPUT_MAX_LEVEL 512

enum DigitalState{
    HIGH,
    LOW
};

/* Output: O1-O8
*/
class Output{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Output(FISH_X1_TRANSFER*,uint8_t pin);
    void on();
    void off();
    void setLevel(uint16_t);
    uint8_t getPin();    
};

/* DigitalInput for e.g. Button, Fototransistor or reedcontact: I1-I8
*/
class DigitalInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    DigitalInput(FISH_X1_TRANSFER*, uint8_t pin);
    bool value();
    void waitFor(DigitalState);
    uint8_t getPin();    
};

/*
*/
class Counter{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Counter(FISH_X1_TRANSFER*, uint8_t pin);
    bool value();
    uint8_t getPin();  
    void waitSteps(uint16_t);  
};

/*AnalogInput for NTC-Resistor, Photo-Resistor: I1-I8
*/
class AnalogInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    AnalogInput(FISH_X1_TRANSFER*,uint8_t pin);
    uint16_t value();
    uint8_t getPin();    
};

class NTC : public AnalogInput{
    public:
    NTC(FISH_X1_TRANSFER*,uint8_t pin);
    double getTemperature();
};

/*Ultrasonic: I1-I8
*/
class Ultrasonic{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Ultrasonic(FISH_X1_TRANSFER*,uint8_t pin);
    uint16_t value();
    uint8_t getPin();    
};

/*ColorSensor: I1-I8
*/
class ColorSensor{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    ColorSensor(FISH_X1_TRANSFER*,uint8_t pin);
    uint16_t value();
    Color color();
    uint8_t getPin();
};

/*For measure the Voltage (same as the ColorSensor): I1-I8
*/
class Voltage{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Voltage(FISH_X1_TRANSFER*,uint8_t pin);
    uint16_t value();
    uint8_t getPin();
};

/*TrackSensor: I1-I8*/
class TrackSensor{
    private:
    uint8_t left;
    uint8_t right;
    FISH_X1_TRANSFER* pTArea;
    public:
    TrackSensor(FISH_X1_TRANSFER*,uint8_t left,uint8_t right);
    bool valueLeft();
    bool valueRight();
    uint8_t getPinLeft();
    uint8_t getPinRight();
};

/*Motor: M1-M4*/
class Motor{
    protected:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Motor(FISH_X1_TRANSFER* ,uint8_t pin);
    void left(uint16_t level);
    void right(uint16_t level);
    void stop();
    uint8_t getPin(); 
};

/*EncoderMotor: M1-M4, C1-C4
*/
class EncoderMotor : public Motor{
    public:
    EncoderMotor(FISH_X1_TRANSFER* ,uint8_t pin);
    void distanceLeft(uint16_t steps,uint16_t level);
    void distanceRight(uint16_t steps,uint16_t level);
    void synchronizeTo(EncoderMotor& other);
    void stopSynchronization();
    void waitToEnd();
    uint16_t counter();
    void reset();
};


class TXT{
private:
    FISH_X1_TRANSFER *pTArea;
    bool _extension;
    
public:
    TXT();
    TXT(FISH_X1_TRANSFER*,bool);
    ~TXT();
    bool isExtension();
    FISH_X1_TRANSFER* getArea();
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
    TrackSensor trackSensor(uint8_t left,uint8_t right);
    TXT extension();
    void playSound(uint8_t index,uint8_t repeats);
    void playSoundAndWait(uint8_t index,uint8_t repeats);
    uint16_t getTXTVoltage();
};


#endif