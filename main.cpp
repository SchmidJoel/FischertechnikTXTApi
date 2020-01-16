#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"
#include <iostream>
#include <fstream>
#include <chrono>
/*
todo:
c-Taster abfragen
utils.h mit farbwertconverter und zeit
achsen async, encoderpos
n-Axis

*/

int main(void) {
	TXT txt = TXT();

    TwoRefAxis ofen = TwoRefAxis{txt, 6, 10, 9};
    TwoRefAxis vacuum = TwoRefAxis{txt, 6, 5, 11};
    TwoRefAxis dreh = TwoRefAxis{txt, 1, 1, 3};
    ofen.pos1();
    vacuum.pos1();
    dreh.pos1();

    Output runter = txt.output(14);
    Output comp = txt.output(8);
    comp.on();
    runter.on();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    
	return 0;
}

void vgr(TXT &txt){
    AxisEM x = AxisEM {txt, 5, 9};
    AxisEM y = AxisEM{txt, 6, 10};
    AxisEM z = AxisEM {txt, 7, 11};
    
    y.reference();  
    z.reference();  
    x.reference();

    std::thread athread = x.moveAbsolutAsync(920);
    std::thread bthread = z.moveAbsolutAsync(880);
    athread.join();
    bthread.join();

    std::thread cthread = x.moveRelativeAsync(480);
    std::thread dthread = z.moveRelativeAsync(-610);

    cthread.join();
    dthread.join();

}


void hgr(TXT &txt){
    AxisEM x = AxisEM {txt, 2, 5};
    AxisEM y = AxisEM{txt, 4, 8};
    TwoRefAxis z = TwoRefAxis{txt, 3, 6, 7};

    z.pos1();
    x.reference();
    y.reference();
    
    std::thread aathread = x.moveAbsolutAsync(11);
    std::thread bbthread = y.moveAbsolutAsync(740);
    aathread.join();
    bbthread.join();

    std::thread athread = x.moveAbsolutAsync(760);
    std::thread bthread = y.moveAbsolutAsync(85);

    athread.join();
    bthread.join();
    z.pos2();
    y.moveRelative(-50);
    z.pos1();

    std::thread ethread = x.moveAbsolutAsync(1380);
    std::thread fthread = y.moveAbsolutAsync(450);
    ethread.join();
    fthread.join();

    std::thread cthread = x.moveAbsolutAsync(1990);
    std::thread dthread = y.moveAbsolutAsync(850-50);

    cthread.join();
    dthread.join();

    z.pos2();
    y.moveRelative(50);
    z.pos1();
}

void sortingline(TXT &txt){
    DigitalInput l1 = txt.digitalInput(1);
    Motor fliessband = txt.motor(1);
    ColorSensor color = txt.colorSensor(2);
    DigitalInput l2 = txt.digitalInput(3);
    Output comp = txt.output(8);
    Output white = txt.output(5);
    Output red = txt.output(6);
    Output blue = txt.output(7);


    while(l1.value());
    fliessband.right(512);
    comp.on();

    int min = color.value();
    while(l2.value()){
        if(color.value() < min){
            min = color.value();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    int dist = 0;
    if(min > 1600){
        dist = 26;
    }
    else if(min > 1200){
        dist = 16;
    }
    else{
        dist = 6;
    }

    
       
    while(dist){
        auto val = txt.getArea()->ftX1in.cnt_in[0];
        while(val == txt.getArea()->ftX1in.cnt_in[0]);
        dist--;
        
    }
    fliessband.stop();
    white.on();
    red.on();
    blue.on();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}