#include "TXT_highlevel_API.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <thread>


/*Axis with encodermotor, referenceswitch (opener) and maxpos*/
AxisEM::AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin, uint16_t max) : em(txt.encoderMotor(motorpin)), ref(txt.digitalInput(refpin)), maxPos(max) {
    speed = 512;
    state = AxisState::UNREFERENCED;
}

/*Axis with encodermotor and referenceswitch (opener) - maxpos = maxvalue(uint16_t)*/
AxisEM::AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin) : AxisEM(txt, motorpin, refpin, std::numeric_limits<uint16_t>::max()){}

/*reference drive until ref is pressed*/
void AxisEM::reference() {
	 while(!ref.value()){
        em.left(speed);
    }
    em.stop();
    pos = 0;
    em.reset();
    state = AxisState::READY;
}

/*stop the axis*/
void AxisEM::stop() {
    em.stop();
}

/*get current position*/
uint16_t AxisEM::getPos() {
    return pos + em.counter();
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisEM::moveAbsolut(uint16_t pos) {
    if(pos > maxPos || pos < 0 || state != AxisState::READY){
        return false;
    }
    dest = pos;
    drive();
    return true;
}

std::thread AxisEM::moveAbsolutAsync(uint16_t pos){
    return std::thread (&AxisEM::moveAbsolut, this, pos);
}

void AxisEM::setSpeed(uint16_t speed_){
    if(speed_ <= 512 && speed_ >= 0){
        speed = speed_;
    }
    if(state == AxisState::LEFT){
        em.left(speed);
    }
    else if(state == AxisState::RIGHT){
        em.right(speed);
    }
}

void AxisEM::drive(){
    if(pos > dest){
        state = AxisState::LEFT;
        em.distanceLeft(pos - dest, speed);
    }
    else if(pos < dest){
        state = AxisState::RIGHT;
        em.distanceRight(dest - pos, speed);
    }
    em.waitToEnd();
    pos = dest;
    state = AxisState::READY;
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisEM::moveRelative(int16_t distance) {
    if(pos + distance > maxPos || pos + distance < 0 || state != AxisState::READY){
        return false;
    }
    dest = pos + distance;
    drive();
    return true;
}

std::thread AxisEM::moveRelativeAsync(int16_t pos){
    return std::thread (&AxisEM::moveRelative, this, pos);
}

AxisState AxisEM::getState(){
    return state;
}



/*Axis with normal motor and stepper, referenceswitch (opener), stepperpin and maxpos*/
AxisXS::AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin, uint16_t max) : m(txt.encoderMotor(motorpin)), ref(txt.digitalInput(refpin)), counter(txt.digitalInput(countpin)), maxPos(max) {
    state = AxisState::UNREFERENCED;
    speed = 512;
}

/*Axis with normal motor and stepper, referenceswitch (opener), stepperpin - maxpos = maxvalue(uint16_t)*/
AxisXS::AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin) : AxisXS(txt, motorpin, countpin, refpin, std::numeric_limits<uint16_t>::max()){}

/*get current position*/
uint16_t AxisXS::getPos() {
    return pos;
}

/*referencedrive until ref is pressed*/
void AxisXS::reference() {
	while(!ref.value()){
        m.left(speed);
    }
    m.stop();
    pos = 0;
    state = AxisState::READY;
}

/*stop the axis*/
void AxisXS::stop(){
    m.stop();
}

void AxisXS::drive(){
    if(pos > dest){
        state = AxisState::LEFT;
        m.left(speed);
    }
    else if(pos < dest){
        state = AxisState::RIGHT;
        m.right(speed);
    }
    while(pos != dest){
        auto val = counter.value();
        while(val == counter.value());
        if(state == AxisState::LEFT){
            pos--;
        }
        else if(state == AxisState::RIGHT){
            pos++;
        }
    }
    m.stop();
    state = AxisState::READY;
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisXS::moveAbsolut(uint16_t destination) {
    if(pos > maxPos || pos < 0 || state != AxisState::READY){
        return false;
    }
    dest = pos;
    drive();
    return true;
}

std::thread AxisXS::moveAbsolutAsync(uint16_t pos){
    return std::thread (&AxisXS::moveAbsolut, this, pos);
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisXS::moveRelative(int16_t distance) {
    if(pos + distance > maxPos || pos + distance < 0 || state != AxisState::READY){
        return false;
    }
    dest = pos + distance;
    drive();
    return true;
}

std::thread AxisXS::moveRelativeAsync(int16_t pos){
    return std::thread (&AxisXS::moveRelative, this, pos);
}

void AxisXS::setSpeed(uint16_t speed_){
    if(speed_ <= 512 && speed_ >= 0){
        speed = speed_;
    }
}

AxisState AxisXS::getState(){
    return state;
}

/*axis with two endswitch*/
TwoRefAxis::TwoRefAxis(TXT& txt, uint8_t motorpin, uint8_t refpin1, uint8_t refpin2) : m(txt.motor(motorpin)), ref1(txt.digitalInput(refpin1)), ref2(txt.digitalInput(refpin2)) {}

/*move to pos 1*/
void TwoRefAxis::pos1(){
    while( !ref1.value()){
        m.left(512);
    }
    m.stop();
}

/*move to pos 2*/
void TwoRefAxis::pos2(){
    while( !ref2.value()){
        m.right(512);
    }
    m.stop();
}

/*is Axis at pos1*/
bool TwoRefAxis::isPos1(){
    return ref1.value();
}

/*is axis at pos2*/
bool TwoRefAxis::isPos2(){
    return ref2.value();
}


