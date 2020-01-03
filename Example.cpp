#include "unistd.h"
#include "TXT_lowlevel_API.hpp"
#include "TXT_highlevel_API.hpp"
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


	auto lampe = txt.output(3);
	//lampe.on();

/*
	AxisXS axis = AxisXS(txt, 1, 1, 2, 160);
	axis.reference();
	axis.moveAbsolut(80);
	for(int i = 0; i < 100; i++){
		axis.moveRelative(20);
		sleep(1);
	}
*/
	TwoRefAxis axis = TwoRefAxis(txt, 1, 1, 3);
	axis.pos1();
	axis.pos2();
	axis.pos1();

	std::ofstream output("out.txt");
	output << lampe.getPin();
	output.close();
	

	return 0;
}
