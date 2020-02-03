#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"

#include <fstream>

int main(void) {
	TXT txt = TXT();

    //TwoRefAxis axis = TwoRefAxis{txt, 1, 1, 6};
    AxisEM axis = AxisEM{txt, 1, 1};
    axis.reference();
    axis.moveAbsolut(500);
    axis.reference();
    
    /*
    std::ofstream out = std::ofstream("settings.txt");
    
    out << txt.getArea()->ftX1out.cnt_reset_cmd_id[0] << std::endl;
    out << txt.getArea()->ftX1out.distance[0] << std::endl;
    out << txt.getArea()->ftX1in.motor_ex_reached[0] << std::endl;
    out << txt.getArea()->ftX1out.motor_ex_cmd_id[0] << std::endl;

    auto em = txt.encoderMotor(1);

    em.left(512);
    txt.digitalInput(1).waitFor(DigitalState::HIGH);
    em.stop();

    //sleep(1);

    em.distanceRight(500, 512);
    em.waitToEnd();    
    em.stop();


    //sleep(1);

    em.left(512);
    txt.digitalInput(1).waitFor(DigitalState::HIGH);
    em.stop();

    //sleep(1);
    out.close();
    */
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
    TwoRefAxis z = TwoRefAxis{txt, 3, 6, 7, ButtonMode::CLOSER, ButtonMode::CLOSER};

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

void workingStation(TXT &txt){
    TwoRefAxis ofen = TwoRefAxis{txt, 6, 10, 9};
    TwoRefAxis vacuum = TwoRefAxis{txt, 6, 5, 11};
    std::vector<uint8_t> pos;
    pos.push_back(1);
    pos.push_back(2);
    pos.push_back(3);
    NRefAxis dreh = NRefAxis{txt, 1, pos};
    ofen.pos1();
    vacuum.pos1();
    dreh.reference();

    Output runter = txt.output(14);
    Output comp = txt.output(8);
    comp.on();
    runter.on();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
}