#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"
#include <iostream>
#include <fstream>

int main(void)
{
	TXT txt = TXT();
	//DigitalInput taster = txt.digitalInput(1);
	/*
	EncoderMotor em1 = txt.encoderMotor(1);
	EncoderMotor em2 = txt.encoderMotor(2);

	em1.synchronizeTo(em2);
	em1.distanceLeft(400, 512);
	em2.left(512);

	em1.waitToEnd();
	*/

	Motor motor = txt.motor(1);
	DigitalInput taster = DigitalInput(txt.getTransferArea(),1);
	
	motor.left(512);
	auto u = txt.voltage(1);

	

	std::ofstream out("output_vals.txt");

	for(int i = 0; i < 1000; i++){
		out << i << "  " << u.value() << std::endl;
		sleep(10ms);
	}



	out.close();
	sleep(10s);
	return 0;
}
