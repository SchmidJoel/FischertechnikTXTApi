#include "FTLib.hpp"


int main(void) {
	TXT txt = TXT();
	DigitalInput Taster = txt.digitalInput(0);
	Output Licht = txt.output(0);
	while(true){
		if(Taster.value()){
			Licht.on();
		}
		else{
			Licht.off();
		}
	}
	return 0;
}
