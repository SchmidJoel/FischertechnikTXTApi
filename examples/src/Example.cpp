#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"
#include <iostream>
#include <fstream>


int main(void) {
	TXT txt = TXT();
	EncoderMotor em1 = txt.encoderMotor(1);
	EncoderMotor em2 = txt.encoderMotor(2);

	em1.synchronizeTo(em2);
	em1.distanceLeft(400, 512);
	em2.left(512);

	em1.waitToEnd();
	return 0;
}
