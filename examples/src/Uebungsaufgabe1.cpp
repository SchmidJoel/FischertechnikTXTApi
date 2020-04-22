#include "TXT_lowlevel_API.h"

int main(void)
{
    TXT txt = TXT();
    Output car_red = txt.output(1);
    Output car_yellow = txt.output(2);
    Output car_green = txt.output(3);
    Output person_red = txt.output(4);
    Output person_green = txt.output(5);
    DigitalInput request = txt.digitalInput(1);
    while (true)
    {
        person_red.on();
        car_red.on();
        sleep(3s);
        car_yellow.on();
        sleep(1s);
        car_red.off();
        car_yellow.off();
        car_green.on();
        request.waitFor(DigitalState::HIGH);
        sleep(1s);
        car_green.off();
        car_yellow.on();
        sleep(1s);
        car_yellow.off();
        car_red.on();
        sleep(1s);
        person_red.off();
        person_green.on();
        sleep(4s);
        person_green.off();
    }
    return 0;
}
