#include "FTLib.hpp"
#include "unistd.h"


int main(void) {
	TXT txt = TXT();
	DigitalInput Taster = txt.digitalInput(0);
	Motor m2 = txt.motor(1);
	EncoderMotor em1 = txt.encoderMotor(0);
	EncoderMotor em2 = txt.encoderMotor(1);
	em1.distanceLeft(1000,512);
	em2.distanceLeft(1000,512);
	em2.synchronizeTo(em1);
	sleep(10);
	return 0;
}
