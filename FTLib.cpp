#include "FTLib.hpp"
#include <stdexcept>

unsigned int DebugFlags;
FILE *DebugFile;

//TXT
TXT::TXT(){
    if(StartTxtDownloadProg() == KELIB_ERROR_NONE){
        pTArea = GetKeLibTransferAreaMainAddress();
    }
    else{
        throw std::runtime_error("KELIB_ERROR");
    }
}

TXT::~TXT(){
    StopTxtDownloadProg();
}

DigitalInput TXT::digitalInput(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return DigitalInput{(pTArea+(pin>>3)),pin&7};
}

AnalogInput TXT::analogInput(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return AnalogInput{(pTArea+(pin>>3)),pin&7};
}

Ultrasonic TXT::ultrasonic(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return Ultrasonic{(pTArea+(pin>>3)),pin&7};
}

Voltage TXT::voltage(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return Voltage{(pTArea+(pin>>3)),pin&7};
}

ColorSensor TXT::colorSensor(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return ColorSensor{(pTArea+(pin>>3)),pin&7};
}

Output TXT::output(uint8_t pin){
    if(pin > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    return Output{(pTArea+(pin>>3)),pin&7};
}

TrackSensor TXT::trackSensor(uint8_t left, uint8_t right){
    if(left > 15 || right > 15){
        throw std::invalid_argument("pin must be between 0 and 7 for master and 8 and 15 for extension");
    }
    else if((left>>3) != (right>>3)){
        throw std::invalid_argument("both pins must be on the same TXT");
    }
    return TrackSensor{(pTArea+(left>>3)),left&7, right&7};
}

Motor TXT::motor(uint8_t pin){
    if(pin > 8){
        throw std::invalid_argument("pin must be between 0 and 3 for master and 4 and 8 for extension");
    }
    return Motor{(pTArea+(pin>>2)),pin&3};
}

EncoderMotor TXT::encoderMotor(uint8_t pin, uint8_t c_pin){
    if(pin > 8 || c_pin > 8){
        throw std::invalid_argument("pin must be between 0 and 3 for master and 4 and 8 for extension");
    }
    else if((pin>>2) != (c_pin>>2)){
        throw std::invalid_argument("both pins must be on the same TXT");
    }
    return EncoderMotor{(pTArea+(pin>>2)),pin&3, c_pin&3};
}

FISH_X1_TRANSFER* TXT::getArea(){
    return pTArea;
}


//Output
Output::Output(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){}

void Output::on(){
    pTArea->ftX1out.duty[pin] = 512;
}

void Output::off(){
    pTArea->ftX1out.duty[pin] = 0;
}

void Output::setLevel(uint16_t level){
    pTArea->ftX1out.duty[pin] = level;
}

uint8_t Output::getPin(){
    return pin;
}

//DigitalInput
DigitalInput::DigitalInput(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_R; 	    //  resistor
    pTArea->ftX1config.uni[pin].digital = 1;        //  digital Input
    pTArea->ftX1state.config_id ++; 
}

bool DigitalInput::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t DigitalInput::getPin(){
    return pin;
}

AnalogInput::AnalogInput(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_R; 	    //  resistor
    pTArea->ftX1config.uni[pin].digital = 0;        //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t AnalogInput::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t AnalogInput::getPin(){
    return pin;
}

//Colorsensor
ColorSensor::ColorSensor(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_U; 	//	Spannung
    pTArea->ftX1config.uni[pin].digital = 0;    //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t ColorSensor::value(){
    return pTArea->ftX1in.uni[pin];
}

Color ColorSensor::color(){
    if(value() < 500){
        return Color::WHITE;
    }
    else if(value() < 1300){
        return Color::RED;
    }
    else {
        return Color::BLUE;
    }
}

uint8_t ColorSensor::getPin(){
    return pin;
}

//Colorsensor
Voltage::Voltage(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_U; 	//	Spannung
    pTArea->ftX1config.uni[pin].digital = 0;    //  analog Input
    pTArea->ftX1state.config_id ++; 
}

uint16_t Voltage::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t Voltage::getPin(){
    return pin;
}

//Colorsensor
Ultrasonic::Ultrasonic(FISH_X1_TRANSFER* pTArea,uint8_t pin): pin(pin), pTArea(pTArea){
    pTArea->ftX1config.uni[pin].mode = MODE_ULTRASONIC; 
    pTArea->ftX1config.uni[pin].digital = 0;  
    pTArea->ftX1state.config_id ++; 
}

uint16_t Ultrasonic::value(){
    return pTArea->ftX1in.uni[pin];
}

uint8_t Ultrasonic::getPin(){
    return pin;
}

//Spurensensor
TrackSensor::TrackSensor(FISH_X1_TRANSFER* pTArea,uint8_t left, uint8_t right): left(left), right(right), pTArea(pTArea){
    pTArea->ftX1config.uni[left].mode = MODE_U; 
    pTArea->ftX1config.uni[left].digital = 1;  
    pTArea->ftX1config.uni[right].mode = MODE_U; 
    pTArea->ftX1config.uni[right].digital = 1;  
    pTArea->ftX1state.config_id ++; 
}

bool TrackSensor::valueLeft(){
    return pTArea->ftX1in.uni[left];
}

bool TrackSensor::valueRight(){
    return pTArea->ftX1in.uni[right];
}

uint8_t TrackSensor::getPinLeft(){
    return left;
}

uint8_t TrackSensor::getPinRight(){
    return left;
}


//Motor
Motor::Motor(FISH_X1_TRANSFER* pTArea, uint8_t pin): pin(pin), pTArea(pTArea){}


void Motor::left(uint16_t level){
    pTArea->ftX1out.duty[pin*2] = level;
    pTArea->ftX1out.duty[pin*2+1] = 0;
}

void Motor::right(uint16_t level){
    pTArea->ftX1out.duty[pin*2] = 0;
    pTArea->ftX1out.duty[pin*2+1] = level;
}

void Motor::stop(){
    pTArea->ftX1out.duty[pin*2] = 0;
    pTArea->ftX1out.duty[pin*2+1] = 0;
}

uint8_t Motor::getPin(){
    return pin;
}

//EncoderMotor
EncoderMotor::EncoderMotor(FISH_X1_TRANSFER* pTArea,uint8_t pin, uint8_t c_pin) : Motor(pTArea,pin), c_pin(c_pin) {}

void EncoderMotor::distanceLeft(uint16_t steps){
    pTArea->ftX1out.distance[pin] = steps;  // Distance to drive Motor 1 [0]
	pTArea->ftX1out.motor_ex_cmd_id[pin]++; // Set new Distance Value for Motor 1 [0]
}

