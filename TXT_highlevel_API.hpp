#ifndef TXT_HIGHLEVEL_API
#define TXT_HIGHLEVEL_API

#include "TXT_lowlevel_API.hpp"

enum AxisState{
    UNREFERENCED,
    READY,
    LEFT,
    RIGHT
};

class PosAxis{
public:
    virtual uint16_t getPos() = 0;
    virtual void reference() = 0;
    virtual bool moveAbsolut(uint16_t destination) = 0;
    virtual bool moveRelative(int16_t distance) = 0;
    virtual void stop() = 0;
    virtual void setSpeed(uint16_t speed) = 0;
    virtual AxisState getState() = 0;
};

class AxisEM : public PosAxis{
private:
    EncoderMotor em;
    DigitalInput ref;
    uint16_t pos;
    uint16_t dest;
    uint16_t maxPos;
    uint16_t speed;
    AxisState state;
    void drive();
public:
    AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin);
    AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin, uint16_t max);
    uint16_t getPos() override;
    void reference() override;
    bool moveAbsolut(uint16_t destination) override;
    bool moveRelative(int16_t distance) override;
    void stop() override;
    void setSpeed(uint16_t speed) override;
    AxisState getState() override;
};

class AxisXS : public PosAxis{
private:
    Motor m;
    DigitalInput ref;
    DigitalInput counter;
    uint16_t pos;
    uint16_t dest;
    uint16_t maxPos;
    uint16_t speed;
    AxisState state;
    void drive();
public:
    AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin);
    AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin, uint16_t max);
    uint16_t getPos() override;
    void reference() override;
    bool moveAbsolut(uint16_t destination) override;
    bool moveRelative(int16_t distance) override;
    void stop() override;
    void setSpeed(uint16_t speed) override;
    AxisState getState() override;
};


class TwoRefAxis{
private:
    DigitalInput ref1;
    DigitalInput ref2;
    Motor m;
public:
    TwoRefAxis(TXT& txt, uint8_t motorpin, uint8_t refpin1, uint8_t refpin2);
    void pos1();
    void pos2();
    bool isPos1();
    bool isPos2();
};





#endif