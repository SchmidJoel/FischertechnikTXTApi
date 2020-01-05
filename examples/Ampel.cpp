#include "TXT_lowlevel_API.h"
#include "unistd.h"


int main(void) {
	TXT txt = TXT();
	Output red = txt.output(1);
    Output yellow = txt.output(2);
    Output green = txt.output(3);
    while(true){
        red.on();
        sleep(3);
        yellow.on();
        sleep(1);
        red.off();
        yellow.off();
        green.on();
        sleep(3);
        green.off();
        yellow.on();
        sleep(1);
        yellow.off();
    }
	return 0;
}
