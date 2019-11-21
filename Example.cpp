#include "FTLib.hpp"
#include "unistd.h"


int main(void) {
	TXT txt = TXT();
	EncoderMotor em1 = txt.encoderMotor(0);
	Output Lampe = txt.output(2);
	em1.distanceLeft(500,512);
	em1.waitToEnd();
	sleep(1);
	em1.distanceLeft(600,512);
	em1.waitToEnd();
	Lampe.on();
	sleep(10);
	return 0;
}
