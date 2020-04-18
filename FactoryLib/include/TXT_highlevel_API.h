#ifndef TXT_HIGHLEVEL_API
#define TXT_HIGHLEVEL_API

#include "TXT_lowlevel_API.h"
#include <thread>
#include <vector>
#include <utility>
#include <iostream>
#include <limits>

enum AxisState
{
    UNREFERENCED,
    READY,
    LEFT,
    RIGHT
};

enum ButtonMode
{
    CLOSER,
    OPENER
};

class PosAxis
{
public:
    virtual uint16_t getPos() = 0;
    virtual void reference() = 0;
    virtual std::thread referenceAsync() = 0;
    virtual bool moveAbsolut(uint16_t destination) = 0;
    virtual std::thread moveAbsolutAsync(uint16_t destination) = 0;
    virtual bool moveRelative(int16_t distance) = 0;
    virtual std::thread moveRelativeAsync(int16_t distance) = 0;
    virtual void stop() = 0;
    virtual void setSpeed(uint16_t speed) = 0;
    virtual AxisState getState() = 0;
    virtual ButtonMode getRefButtonMode() = 0;
    virtual void setRefButtonMode(ButtonMode mode) = 0;
};

class AxisEM : public PosAxis
{
private:
    EncoderMotor em;
    DigitalInput ref;
    uint16_t pos;
    uint16_t dest;
    uint16_t maxPos;
    uint16_t speed;
    AxisState state;
    ButtonMode mode;
    void drive();

public:
    //AxisEM(TXT &txt, uint8_t motorpin, uint8_t refpin);
    
    AxisEM(TXT &txt, uint8_t motorpin, uint8_t refpin, ButtonMode = ButtonMode::CLOSER, uint16_t = std::numeric_limits<uint16_t>::max());
    //AxisEM(TXT &txt, uint8_t motorpin, uint8_t refpin, ButtonMode refButtonMode, uint16_t max);
    uint16_t getPos() override;
    void reference() override;
    std::thread referenceAsync() override;
    bool moveAbsolut(uint16_t destination) override;
    std::thread moveAbsolutAsync(uint16_t destination) override;
    bool moveRelative(int16_t distance) override;
    std::thread moveRelativeAsync(int16_t destination) override;
    void stop() override;
    void setSpeed(uint16_t speed) override;
    AxisState getState() override;
    ButtonMode getRefButtonMode() override;
    void setRefButtonMode(ButtonMode mode) override;
};

class AxisXS : public PosAxis
{
private:
    Motor m;
    DigitalInput ref;
    Counter counter;
    uint16_t pos;
    uint16_t dest;
    uint16_t maxPos;
    uint16_t speed;
    AxisState state;
    ButtonMode mode;
    void drive();

public:
    AxisXS(TXT &txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin, ButtonMode = ButtonMode::CLOSER, uint16_t = std::numeric_limits<uint16_t>::max());
    uint16_t getPos() override;
    void reference() override;
    std::thread referenceAsync() override;
    bool moveAbsolut(uint16_t destination) override;
    std::thread moveAbsolutAsync(uint16_t destination) override;
    bool moveRelative(int16_t distance) override;
    std::thread moveRelativeAsync(int16_t distance) override;
    void stop() override;
    void setSpeed(uint16_t speed) override;
    AxisState getState() override;
    ButtonMode getRefButtonMode() override;
    void setRefButtonMode(ButtonMode mode) override;
};

class NRefAxis
{
protected:
    Motor m;
    uint16_t speed;
    std::vector<std::pair<DigitalInput, ButtonMode>> postitions;
    AxisState state;
    uint8_t lastpos;

public:
    NRefAxis(TXT &txt, uint8_t motorpin, std::vector<uint8_t> pos);
    void reference();
    std::thread referenceAsync();
    bool isPos(uint8_t pos);
    void pos(uint8_t pos);
    std::thread posAsync(uint8_t pos);
    void setSpeed(uint16_t speed_);
    void setButtonMode(uint8_t, ButtonMode mode);
};

class TwoRefAxis : public NRefAxis
{
public:
    TwoRefAxis(TXT &txt, uint8_t motorpin, uint8_t refpin1, uint8_t refpin2, ButtonMode = ButtonMode::CLOSER, ButtonMode = ButtonMode::CLOSER);
    void pos1();
    void pos2();
    std::thread pos1Async();
    std::thread pos2Async();
    bool isPos1();
    bool isPos2();
};

#endif