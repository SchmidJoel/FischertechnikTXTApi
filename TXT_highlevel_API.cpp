#include "TXT_highlevel_API.hpp"
#include <fstream>
#include <iostream>
#include <limits>

/*Axis with encodermotor, referenceswitch (opener) and maxpos*/
AxisEM::AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin, uint16_t max) : em(txt.encoderMotor(motorpin)), ref(txt.digitalInput(refpin)), maxPos(max) {}

/*Axis with encodermotor and referenceswitch (opener) - maxpos = maxvalue(uint16_t)*/
AxisEM::AxisEM(TXT& txt, uint8_t motorpin, uint8_t refpin) : AxisEM(txt, motorpin, refpin, std::numeric_limits<uint16_t>::max()){}

/*reference drive until ref is pressed*/
void AxisEM::reference() {
	 while(ref.value()){
        em.left(512);
    }
    em.stop();
    pos = 0;
    em.resetCounter();
}

/*stop the axis*/
void AxisEM::stop() {
    em.stop();
}

/*get current position*/
uint16_t AxisEM::getPos() {
    return pos;
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisEM::moveAbsolut(uint16_t destination) {
    return moveRelative(destination - pos);
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisEM::moveRelative(int16_t distance) {
    if(pos + distance > maxPos){
        return false;
    }
    if(distance > 0){
        em.distanceRight(distance,512);        
    }
    else{
        em.distanceLeft(abs(distance),512);
    }
    em.waitToEnd();
    pos += distance;
    return true;
}

/*Axis with normal motor and stepper, referenceswitch (opener), stepperpin and maxpos*/
AxisXS::AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin, uint16_t max) : m(txt.encoderMotor(motorpin)), ref(txt.digitalInput(refpin)), counter(txt.digitalInput(countpin)), maxPos(max) {}

/*Axis with normal motor and stepper, referenceswitch (opener), stepperpin - maxpos = maxvalue(uint16_t)*/
AxisXS::AxisXS(TXT& txt, uint8_t motorpin, uint8_t refpin, uint8_t countpin) : AxisXS(txt, motorpin, countpin, refpin, std::numeric_limits<uint16_t>::max()){}

/*get current position*/
uint16_t AxisXS::getPos() {
    return pos;
}

/*referencedrive until ref is pressed*/
void AxisXS::reference() {
	 while(ref.value()){
        m.left(512);
    }
    m.stop();
    pos = 0;
}

/*stop the axis*/
void AxisXS::stop(){
    m.stop();
}

/*move for a absolute value, return false if maxpos would exceed*/
bool AxisXS::moveAbsolut(uint16_t destination) {
    return moveRelative(destination - pos);
}

/*move for a relative value, return false if maxpos would exceed*/
bool AxisXS::moveRelative(int16_t distance) {
    if(pos + distance > maxPos){
        return false;
    }
    uint16_t dist = 0;
    if(distance > 0){
        m.right(512);
    }
    else{
        m.left(512);
    }
    while(dist < abs(distance)){
        auto val = counter.value();
        while(val == counter.value() || !ref.value());
        dist++;
        pos += distance > 0 ? 1 : -1;
        if(!ref.value()){
            reference();
        }
    }
    m.stop();
    return true;
}

/*axis with two endswitch*/
TwoRefAxis::TwoRefAxis(TXT& txt, uint8_t motorpin, uint8_t refpin1, uint8_t refpin2) : m(txt.motor(motorpin)), ref1(txt.digitalInput(refpin1)), ref2(txt.digitalInput(refpin2)) {}

/*move to pos 1*/
void TwoRefAxis::pos1(){
    while( ref1.value()){
        m.left(512);
    }
    m.stop();
}

/*move to pos 2*/
void TwoRefAxis::pos2(){
    while( ref2.value()){
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
