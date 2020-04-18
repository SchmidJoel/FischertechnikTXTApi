#include "TXT_highlevel_API.h"

/*Axis with encodermotor, referenceswitch (opener) and maxpos*/
AxisEM::AxisEM(TXT &txt, uint8_t motorpin, uint8_t refpin, ButtonMode refButtonMode, uint16_t max) : 
        em(txt.encoderMotor(motorpin)),
        ref(txt.digitalInput(refpin)),
        maxPos(max),
        mode(refButtonMode)
{
    speed = 512;
    state = AxisState::UNREFERENCED;
}

/*reference drive until ref is pressed*/
void AxisEM::reference()
{
    em.reset();
    em.left(speed);
    while ((ButtonMode::CLOSER == mode && !ref.value()) || (ButtonMode::OPENER == mode && ref.value()))
    {
        em.left(speed);
        sleep(5ms);
    }
    em.stop();
    pos = 0;
    em.reset();
    state = AxisState::READY;
}

std::thread AxisEM::referenceAsync()
{
    return std::thread(&AxisEM::reference, this);
}

/*stop the axis*/
void AxisEM::stop()
{
    em.stop();
}

/*get current position*/
uint16_t AxisEM::getPos()
{
    return pos + em.counter();
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisEM::moveAbsolut(uint16_t destination)
{
    if (destination > maxPos || destination < 0 || state != AxisState::READY)
    {
        return false;
    }
    dest = destination;
    drive();
    return true;
}

std::thread AxisEM::moveAbsolutAsync(uint16_t pos)
{
    return std::thread(&AxisEM::moveAbsolut, this, pos);
}

void AxisEM::setSpeed(uint16_t speed_)
{
    if (speed_ <= 512 && speed_ >= 0)
    {
        speed = speed_;
    }
    if (state == AxisState::LEFT)
    {
        em.left(speed);
    }
    else if (state == AxisState::RIGHT)
    {
        em.right(speed);
    }
}

void AxisEM::drive()
{
    if (pos > dest)
    {
        state = AxisState::LEFT;
        em.distanceLeft(pos - dest, speed);
    }
    else if (pos < dest)
    {
        state = AxisState::RIGHT;
        em.distanceRight(dest - pos, speed);
    }
    em.waitToEnd();
    pos = dest;
    state = AxisState::READY;
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisEM::moveRelative(int16_t distance)
{
    if (pos + distance > maxPos || pos + distance < 0 || state != AxisState::READY)
    {
        return false;
    }
    dest = pos + distance;
    drive();
    return true;
}

std::thread AxisEM::moveRelativeAsync(int16_t pos)
{
    return std::thread(&AxisEM::moveRelative, this, pos);
}

AxisState AxisEM::getState()
{
    return state;
}

ButtonMode AxisEM::getRefButtonMode()
{
    return mode;
}

void AxisEM::setRefButtonMode(ButtonMode _mode)
{
    mode = _mode;
}

/*Axis with normal motor and stepper, referenceswitch (opener), stepperpin and maxpos*/
AxisXS::AxisXS(TXT &txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin, ButtonMode refButtonMode, uint16_t max) : 
        m(txt.encoderMotor(motorpin)),
        ref(txt.digitalInput(refpin)),
        counter(txt.counter(countpin)),
        maxPos(max),
        mode(refButtonMode)
{
    state = AxisState::UNREFERENCED;
    speed = 512;
}

/*get current position*/
uint16_t AxisXS::getPos()
{
    return pos;
}

/*referencedrive until ref is pressed*/
void AxisXS::reference()
{
    while ((ButtonMode::CLOSER == mode && !ref.value()) || (ButtonMode::OPENER == mode && ref.value()))
    {
        m.left(speed);
        sleep(10ms);
    }
    m.stop();
    pos = 0;
    state = AxisState::READY;
}

std::thread AxisXS::referenceAsync()
{
    return std::thread(&AxisXS::reference, this);
}

/*stop the axis*/
void AxisXS::stop()
{
    m.stop();
}

void AxisXS::drive()
{
    if (pos > dest)
    {
        state = AxisState::LEFT;
        m.left(speed);
    }
    else if (pos < dest)
    {
        state = AxisState::RIGHT;
        m.right(speed);
    }
    while (pos != dest)
    {
        auto val = counter.value();
        while (val == counter.value())
            ;
        if (state == AxisState::LEFT)
        {
            pos--;
        }
        else if (state == AxisState::RIGHT)
        {
            pos++;
        }
    }
    m.stop();
    state = AxisState::READY;
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisXS::moveAbsolut(uint16_t destination)
{
    if (destination > maxPos || destination < 0 || state != AxisState::READY)
    {
        return false;
    }
    dest = destination;
    drive();
    return true;
}

std::thread AxisXS::moveAbsolutAsync(uint16_t pos)
{
    return std::thread(&AxisXS::moveAbsolut, this, pos);
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisXS::moveRelative(int16_t distance)
{
    if (pos + distance > maxPos || pos + distance < 0 || state != AxisState::READY)
    {
        return false;
    }
    dest = pos + distance;
    drive();
    return true;
}

std::thread AxisXS::moveRelativeAsync(int16_t pos)
{
    return std::thread(&AxisXS::moveRelative, this, pos);
}

void AxisXS::setSpeed(uint16_t speed_)
{
    if (speed_ <= 512 && speed_ >= 0)
    {
        speed = speed_;
    }
}

AxisState AxisXS::getState()
{
    return state;
}

ButtonMode AxisXS::getRefButtonMode()
{
    return mode;
}

void AxisXS::setRefButtonMode(ButtonMode _mode)
{
    mode = _mode;
}

NRefAxis::NRefAxis(TXT &txt, uint8_t motorpin, std::vector<uint8_t> pos) : m(txt.motor(motorpin)),
                                                                           speed(512)
{
    for (int i : pos)
    {
        auto ref = txt.digitalInput(i);
        postitions.push_back(std::pair<DigitalInput, ButtonMode>(ref, ButtonMode::CLOSER));
    }
    state = AxisState::UNREFERENCED;
}

void NRefAxis::reference()
{
    auto ref = postitions[0].first;
    ButtonMode mode = postitions[0].second;
    while ((ButtonMode::CLOSER == mode && !ref.value()) || (ButtonMode::OPENER == mode && ref.value()))
    {
        m.left(speed);
        sleep(10ms);
    }
    m.stop();
    state = AxisState::READY;
    lastpos = 0;
}

std::thread NRefAxis::referenceAsync()
{
    return std::thread(&NRefAxis::reference, this);
}

bool NRefAxis::isPos(uint8_t pos)
{
    if (pos >= postitions.size())
    {
        return false;
    }
    auto btn = postitions[pos].first;
    ButtonMode mode = postitions[pos].second;
    return (ButtonMode::CLOSER == mode && !btn.value()) || (ButtonMode::OPENER == mode && btn.value());
}

void NRefAxis::pos(uint8_t pos)
{
    if (pos >= postitions.size() || state != AxisState::READY)
    {
        return;
    }
    auto btn = postitions[pos].first;
    auto mode = postitions[pos].second;
    while ((ButtonMode::CLOSER == mode && !btn.value()) || (ButtonMode::OPENER == mode && btn.value()))
    {
        if (pos > lastpos)
        {
            m.right(speed);
            state = AxisState::RIGHT;
        }
        else
        {
            m.left(speed);
            state = AxisState::LEFT;
        }
        sleep(1ms);
    }
    m.stop();
    lastpos = pos;
    state = AxisState::READY;
}

std::thread NRefAxis::posAsync(uint8_t pos)
{
    return std::thread(&NRefAxis::pos, this, pos);
}

void NRefAxis::setSpeed(uint16_t speed_)
{
    if (speed_ <= 512 && speed_ >= 0)
    {
        speed = speed_;
    }
}

void NRefAxis::setButtonMode(uint8_t pos, ButtonMode mode)
{
    if (pos >= postitions.size())
    {
        return;
    }
    postitions[pos].second = mode;
}

/*axis with two endbutton, based on NRefAxis*/
TwoRefAxis::TwoRefAxis(TXT &txt, uint8_t motorpin, uint8_t refpin1, uint8_t refpin2, ButtonMode mode1, ButtonMode mode2) : 
    NRefAxis(txt, motorpin, std::vector<uint8_t>{refpin1, refpin2})
{
    setButtonMode(0, mode1);
    setButtonMode(1, mode2);
    state = AxisState::UNREFERENCED;
    lastpos = 0;
}

/*move to pos 1*/
void TwoRefAxis::pos1()
{
    auto btn = postitions[0].first;
    auto mode = postitions[0].second;
    while ((ButtonMode::CLOSER == mode && !btn.value()) || (ButtonMode::OPENER == mode && btn.value()))
    {
        m.left(speed);
        state = AxisState::LEFT;
        sleep(1ms);
    }
    m.stop();
}

/*move to pos 2*/
void TwoRefAxis::pos2()
{
    auto btn = postitions[1].first;
    auto mode = postitions[1].second;
    while ((ButtonMode::CLOSER == mode && !btn.value()) || (ButtonMode::OPENER == mode && btn.value()))
    {
        m.right(speed);
        state = AxisState::RIGHT;
        sleep(1ms);
    }
    m.stop();
}

std::thread TwoRefAxis::pos1Async()
{
    return std::thread(&TwoRefAxis::pos1, this);
}

std::thread TwoRefAxis::pos2Async()
{
    return std::thread(&TwoRefAxis::pos2, this);
}

/*is Axis at pos1*/
bool TwoRefAxis::isPos1()
{
    return isPos(0);
}

/*is axis at pos2*/
bool TwoRefAxis::isPos2()
{
    return isPos(1);
}