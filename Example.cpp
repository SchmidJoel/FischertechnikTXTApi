#include "FTLib.hpp"


int main(void) {
	TXT txt = TXT();
	DigitalInput Taster = txt.digitalInput(0);
	Output Licht = txt.output(0);
	Motor m3 = txt.motor(2);
	while(true){
		if(Taster.value()){
			m3.left(512);
		}
		else{
			m3.stop();
		}
	}
	return 0;
}
