#include "TXT_lowlevel_API.h"

// needed for some debugging stuff of the ft-Libraries ("KeLibTxtDl.h", "FtShmem.h")
unsigned int DebugFlags;
FILE *DebugFile;

#define ERR_8_PIN "pin must be between 1 and 8 for master and 9 and 16 for extension"
#define ERR_4_PIN "pin must be between 1 and 4 for master and 5 and 8 for extension"
#define ERR_SAME_TXT "both pins must be on the same TXT"

IOPin::IOPin(FISH_X1_TRANSFER *pTArea, uint8_t pin, bool eight) {
    if ((pin > 16 || pin < 1) && eight)
    {
        throw std::invalid_argument(ERR_8_PIN);
    }
    else if ((pin > 8 || pin < 1) && !eight){
        throw std::invalid_argument(ERR_4_PIN);
    }

    if((pin > 8 && eight) || (!eight && pin > 4)){
        (*this).pTArea = pTArea+1;
    }
    else{
        (*this).pTArea = pTArea;
    }   
    (*this).pin = eight ? (pin-1)%8 : (pin-1)%4;
}

uint8_t IOPin::getPin(){
    return pin + 1;
}

TXT::TXT()
{
    if (StartTxtDownloadProg() == KELIB_ERROR_NONE)
    {
        pTArea = GetKeLibTransferAreaMainAddress();
    }
    else
    {
        throw std::runtime_error("KELIB_ERROR");
    }
}

TXT::TXT(FISH_X1_TRANSFER *pTArea, bool extension) : pTArea(pTArea), _extension(extension) {}

TXT::~TXT()
{
    StopTxtDownloadProg();
}

DigitalInput TXT::digitalInput(uint8_t pin)
{
    return DigitalInput{pTArea, pin};
}

Counter TXT::counter(uint8_t pin)
{
    return Counter{pTArea, pin};
}

AnalogInput TXT::analogInput(uint8_t pin)
{
    return AnalogInput{pTArea, pin};
}

NTC TXT::ntc(uint8_t pin)
{
    return NTC{pTArea, pin};
}

Ultrasonic TXT::ultrasonic(uint8_t pin)
{
    return Ultrasonic{pTArea, pin};
}

Voltage TXT::voltage(uint8_t pin)
{
    return Voltage{pTArea, pin};
}

ColorSensor TXT::colorSensor(uint8_t pin)
{
    return ColorSensor{pTArea, pin};
}

Output TXT::output(uint8_t pin)
{
    return Output{pTArea, pin};
}

TrackSensor TXT::trackSensor(uint8_t left, uint8_t right)
{
    return TrackSensor{pTArea, left, right};
}

Motor TXT::motor(uint8_t pin)
{
    return Motor{pTArea, pin};
}

EncoderMotor TXT::encoderMotor(uint8_t pin)
{
    return EncoderMotor{pTArea, pin};
}

FISH_X1_TRANSFER *TXT::getTransferArea()
{
    return pTArea;
}

void TXT::playSound(uint8_t index, uint8_t repeats)
{
    pTArea->sTxtOutputs.u16SoundCmdId = 0;
    pTArea->sTxtOutputs.u16SoundIndex = index;
    pTArea->sTxtOutputs.u16SoundRepeat = repeats;
    pTArea->sTxtOutputs.u16SoundCmdId++;
}

void TXT::playSoundAndWait(uint8_t index, uint8_t repeats)
{
    pTArea->sTxtOutputs.u16SoundCmdId = 0;
    pTArea->sTxtOutputs.u16SoundIndex = index;
    pTArea->sTxtOutputs.u16SoundRepeat = repeats;
    pTArea->sTxtOutputs.u16SoundCmdId++;
    while (pTArea->sTxtInputs.u16SoundCmdId == 0)
    {
        sleep(1ms);
    }
}

bool TXT::isExtension() {
    return _extension;
}

TXT TXT::extension()
{
    if (_extension)
    {
        throw std::runtime_error("a extension cannot have another extension");
    }
    return TXT{pTArea + 1, true};
}

uint16_t TXT::getTXTVoltage()
{
    return pTArea->sTxtInputs.u16TxtPower;
}

//Output
Output::Output(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, true){
}

void Output::on()
{
    pTArea->ftX1out.duty[pin] = 512;
}

void Output::off()
{
    pTArea->ftX1out.duty[pin] = 0;
}

void Output::setLevel(uint16_t level)
{
    pTArea->ftX1out.duty[pin] = level;
}

//DigitalInput
DigitalInput::DigitalInput(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, true)
{   
    pin = (*this).pin;
    (*this).pTArea->ftX1config.uni[pin].mode = MODE_R; //  resistor
    (*this).pTArea->ftX1config.uni[pin].digital = 1;   //  digital Input
    (*this).pTArea->ftX1state.config_id++;
}

bool DigitalInput::value()
{
    return pTArea->ftX1in.uni[pin];
}

void DigitalInput::waitFor(DigitalState state)
{
    while ((value() && state == DigitalState::LOW) || (!value() && state == DigitalState::HIGH))
    {
        sleep(10ms);
    }
}

//Counter C1-C4
Counter::Counter(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, false) {}

bool Counter::value()
{
    return pTArea->ftX1in.cnt_in[pin];
}

void Counter::waitSteps(uint16_t steps)
{
    while (steps)
    {
        auto val = pTArea->ftX1in.cnt_in[pin];
        while (val == pTArea->ftX1in.cnt_in[pin])
        {
            sleep(100us);
        }
        steps--;
    }
    pTArea->ftX1out.cnt_reset_cmd_id[pin]++;
    pTArea->ftX1in.motor_ex_reached[pin] = 0;
    sleep(1ms);
}

//AnalogInput (Widerstandsmessung)
AnalogInput::AnalogInput(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, true)
{
    pin = (*this).pin;
    (*this).pTArea->ftX1config.uni[pin].mode = MODE_R; //  resistor
    (*this).pTArea->ftX1config.uni[pin].digital = 0;   //  analog Input
    (*this).pTArea->ftX1state.config_id++;
}

uint16_t AnalogInput::value()
{
    return pTArea->ftX1in.uni[pin];
}

//NTC
NTC::NTC(FISH_X1_TRANSFER *pTArea, uint8_t pin) : AnalogInput(pTArea, pin) {}

double NTC::getTemperature()
{
    return convertToTemperature(value());
}

//Farbsensor
ColorSensor::ColorSensor(FISH_X1_TRANSFER *pTArea, uint8_t pin) : Voltage(pTArea, pin){}

Color ColorSensor::color()
{
    return convertToColor(value());
}

//Spannungsmessung
Voltage::Voltage(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, true)
{
    pin = (*this).pin;
    (*this).pTArea->ftX1config.uni[pin].mode = MODE_U; //	Spannung
    (*this).pTArea->ftX1config.uni[pin].digital = 0;   //  analog Input
    (*this).pTArea->ftX1state.config_id++;
}

uint16_t Voltage::value()
{
    return pTArea->ftX1in.uni[pin];
}

//Ultraschallsensor für Abstand
Ultrasonic::Ultrasonic(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, true)
{
    pin = (*this).pin;
    (*this).pTArea->ftX1config.uni[pin].mode = MODE_ULTRASONIC;
    (*this).pTArea->ftX1state.config_id++;
}

uint16_t Ultrasonic::value()
{
    return pTArea->ftX1in.uni[pin];
}

//Spurensensor
TrackSensor::TrackSensor(FISH_X1_TRANSFER *pTArea, uint8_t left, uint8_t right)
{
    left--;
    right--;
    if(left/7 != right/7){
        throw std::invalid_argument(ERR_SAME_TXT);
    }
    if (left > 15  || right > 15)
    {
        throw std::invalid_argument(ERR_8_PIN);
    }

    if(left > 7){
        (*this).pTArea = pTArea+1;

    }
    else{
        (*this).pTArea = pTArea;
    }      
    left = left%7;
    right = right%7;
    (*this).left = left;
    (*this).right = right;
    (*this).pTArea->ftX1config.uni[left].mode = MODE_U;
    (*this).pTArea->ftX1config.uni[left].digital = 1;
    (*this).pTArea->ftX1config.uni[right].mode = MODE_U;
    (*this).pTArea->ftX1config.uni[right].digital = 1;
    (*this).pTArea->ftX1state.config_id++;
}

bool TrackSensor::valueLeft()
{
    return pTArea->ftX1in.uni[left];
}

bool TrackSensor::valueRight()
{
    return pTArea->ftX1in.uni[right];
}

uint8_t TrackSensor::getPinLeft()
{
    return left + 1;
}

uint8_t TrackSensor::getPinRight()
{
    return right + 1;
}

//Motor
Motor::Motor(FISH_X1_TRANSFER *pTArea, uint8_t pin) : IOPin(pTArea, pin, false) {}

void Motor::left(uint16_t level)
{
    pTArea->ftX1out.cnt_reset_cmd_id[pin] = 0;
    pTArea->ftX1out.motor_ex_cmd_id[pin] = 0;
    pTArea->ftX1out.duty[pin * 2] = level;
    pTArea->ftX1out.duty[pin * 2 + 1] = 0;
}

void Motor::right(uint16_t level)
{
    pTArea->ftX1out.duty[pin * 2] = 0;
    pTArea->ftX1out.duty[pin * 2 + 1] = level;
}

void Motor::stop()
{
    pTArea->ftX1out.duty[pin * 2] = 0;
    pTArea->ftX1out.duty[pin * 2 + 1] = 0;
}

//EncoderMotor
EncoderMotor::EncoderMotor(FISH_X1_TRANSFER *pTArea, uint8_t pin) : Motor(pTArea, pin) {}

void EncoderMotor::distanceLeft(uint16_t steps, uint16_t level)
{
    reset();
    pTArea->ftX1out.distance[pin] = steps;  // Distance to drive
    pTArea->ftX1out.motor_ex_cmd_id[pin]++; // Set new Distance Value
    left(level);
}

void EncoderMotor::distanceRight(uint16_t steps, uint16_t level)
{
    reset();
    pTArea->ftX1out.distance[pin] = steps;  // Distance to drive
    pTArea->ftX1out.motor_ex_cmd_id[pin]++; // Set new Distance Value
    right(level);
}

void EncoderMotor::synchronizeTo(EncoderMotor &other)
{
    pTArea->ftX1out.master[pin] = other.getPin();
}

void EncoderMotor::stopSynchronization()
{
    pTArea->ftX1out.master[pin] = 0;
}

void EncoderMotor::waitToEnd()
{
    while (!pTArea->ftX1in.motor_ex_reached[pin])
    {
        sleep(10ms);
    }
    stop();
}

uint16_t EncoderMotor::counter()
{
    return pTArea->ftX1in.counter[pin];
}

void EncoderMotor::reset()
{
    //stoppen und warten bis Motor steht
    stop();
    sleep(10ms);

    //resetten
    pTArea->ftX1out.cnt_reset_cmd_id[pin]++;
    pTArea->ftX1out.distance[pin] = 0;
    pTArea->ftX1in.motor_ex_reached[pin] = 0;
    pTArea->ftX1out.motor_ex_cmd_id[pin]++;

    sleep(10ms);
}
