#include "unistd.h"
#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"
#include <iostream>
#include <fstream>


int main(void) {
	TXT txt = TXT();

	/*
	NTC ntc = txt.ntc(1);
	std::ofstream output("out.txt");
	for(int i = 0; i < 20; i++){
		output << ntc.getTemperature() << std::endl;
		sleep(1);
	}
	output.close();
	*/


	//auto lampe = txt.output(3);
	//lampe.on();

/*
	AxisXS axis = AxisXS(txt, 1, 1, 2, 160);
	axis.reference();
	axis.moveAbsolut(80);
	for(int i = 0; i < 100; i++){
		axis.moveRelative(20);
		sleep(1);
	}

	TwoRefAxis axis = TwoRefAxis(txt, 1, 1, 3);
	axis.pos1();
	axis.pos2();
	axis.pos1();
	*/


	std::ofstream output("out.txt");

	AxisXS axis = AxisXS(txt, 1, 1, 2, 180);
	//AxisEM axis = AxisEM(txt, 1, 1, 1800);
	output << "start reference" << std::endl;
	axis.reference();
	output << "start drive" << std::endl;
	axis.moveRelative(150);

	for(int i = 0; i < 1000; i++){
		output << "current pos: " << axis.getPos() << std::endl;
		usleep(10000);
	}
	output.close();

	

/*
	EncoderMotor em = txt.encoderMotor(1);
	em.distanceRight(300, 512);
	em.waitToEnd();
	em.reset();
	em.distanceLeft(300, 512);
	em.waitToEnd();
	*/
	return 0;
}
