#include "TXT_lowlevel_API.h"
#include "TXT_highlevel_API.h"
#include <iostream>
#include <fstream>

void digitalInputExample(TXT &txt);
void synchronizedEncoderMotors(TXT &txt);
void ExtensionExample(TXT &txt);
void AxisEM_example(TXT &txt);
void NRefAxisExample(TXT &txt);
void TwoRefAxisExample(TXT &txt);


int main(void)
{
	TXT txt = TXT();
	//digitalInputExample(txt);
	//synchronizedEncoderMotors(txt);
	//ExtensionExample(txt);
	AxisEM_example(txt);
	//NRefAxisExample(txt);
	//TwoRefAxisExample(txt);

	return 0;
}

void digitalInputExample(TXT &txt){
	DigitalInput taster1 = DigitalInput(txt.getTransferArea(),1);
	//alternativ
	DigitalInput taster2 = txt.digitalInput(1);
	if(taster1.value()){
		//Wenn Taster 1 gedrueckt (Taster muss als Schliesser angeschlossen sein)
	}
}

void synchronizedEncoderMotors(TXT &txt){
	EncoderMotor em1 = txt.encoderMotor(1);
	EncoderMotor em2 = txt.encoderMotor(2);
	em1.synchronizeTo(em2);
	em1.distanceLeft(400, 512);
	em2.left(512);
	em1.waitToEnd();
}

void ExtensionExample(TXT &txt){
	//alternative 1
	TXT extension = txt.extension();
	Output lampe = extension.output(1);
	//alternative 2
	Output lampe2 = txt.output(10);
	lampe.on();
	lampe2.on();
}

void AxisEM_example(TXT &txt){
	//Encodermotor an M1, Referenztaster an I2
	AxisEM xaxis = AxisEM{txt, 1, 2}; 
	//state: UNREFERENCED
	xaxis.reference();
	//state: READY
	xaxis.moveAbsolut(50); //state: RIGHT
	//state: READY
	std::thread t = xaxis.moveRelativeAsync(-20);
	//state: left
	t.join();
	//state: READY
}

void NRefAxisExample(TXT &txt){
	NRefAxis ax = NRefAxis{txt, 1, std::vector<uint8_t>{1, 2, 3}};
	//state: UNREFERENCED
	ax.reference();
	//state: READY
	ax.pos(1); //state: RIGHT
	//state: READY
	std::thread t = ax.posAsync(0);
	//state: left
	t.join();
	//state: READY
}

void TwoRefAxisExample(TXT &txt){
	//Motor an M1, Positionstaster an I2 und I3
	TwoRefAxis ax = TwoRefAxis{txt, 1, 1, 2};
	//state: UNREFERENCED
	ax.pos2(); //state: RIGHT
	//state: READY
	std::thread t = ax.pos1Async();
	//state: left
	t.join();
	//state: READY
}

void UtilsExample(TXT &txt){
	sleep(1s); //Wartezeit im Sekundenbereich
	sleep(1ms);//Wartezeit im Millisekundenbereich
	sleep(1us);//Wartezeit im Mikrosekundenbereich
	Voltage v = txt.voltage(1);
	Color c = convertToColor(v.value());
	AnalogInput ai = txt.analogInput(2);
	double temp = convertToTemperature(ai.value());
}
