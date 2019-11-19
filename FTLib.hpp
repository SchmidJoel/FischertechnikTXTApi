#ifndef FTLib
#define FTLib

#include "KeLibTxtDl.h"
#include "FtShmem.h"

/*  This is a lowlevel API for Programming the fischertechnik TXT controller
    To compile programms you need the arm cross-compiler:
    https://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/arm-linux-gnueabihf/gcc-linaro-7.2.1-2017.11-i686-mingw32_arm-linux-gnueabihf.tar.xz
*/



/* Output: O1-O8
*/
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

/* DigitalInput for e.g. Taster, Fototransistor or reedcontact: I1-I8
*/
class DigitalInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    DigitalInput(FISH_X1_TRANSFER*, uint8_t);
    bool value();
    uint8_t getPin();    
};

/*AnalogInput for NTC-Resistor: I1-I8
*/
class AnalogInput{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    AnalogInput(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    uint8_t getPin();    
};

/*Ultrasonic: I1-I8
*/
class Ultrasonic{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Ultrasonic(FISH_X1_TRANSFER*,uint8_t);
    uint16_t value();
    uint8_t getPin();    
};

/*Colors for the ColorSensor
*/
enum Color{
    BLUE,
    WHITE,
    RED
};

/*ColorSensor: I1-I8
*/
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

/*For measure the Voltage (same as the ColorSensor): I1-I8
*/
class Voltage{
    private:
    uint8_t pin;
    FISH_X1_TRANSFER* pTArea;
    public:
    Voltage(FISH_X1_TRANSFER*,uint8_t);
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
    TrackSensor(FISH_X1_TRANSFER*,uint8_t,uint8_t);
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
    Motor(FISH_X1_TRANSFER* ,uint8_t);
    void left(uint16_t);
    void right(uint16_t);
    void stop();
    uint8_t getPin(); 
};

/*EncoderMotor: M1-M4, C1-C4
*/
class EncoderMotor : public Motor{
    public:
    EncoderMotor(FISH_X1_TRANSFER* ,uint8_t);
    void distanceLeft(uint16_t,uint16_t);
    void distanceRight(uint16_t,uint16_t);
    void synchronizeTo(EncoderMotor&);
    void stopSynchronization();
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
    EncoderMotor encoderMotor(uint8_t);
    AnalogInput analogInput(uint8_t);
    DigitalInput digitalInput(uint8_t);
    Ultrasonic ultrasonic(uint8_t);
    Voltage voltage(uint8_t);
    ColorSensor colorSensor(uint8_t);
    TrackSensor trackSensor(uint8_t,uint8_t);
    TXT extension();
};


#endif