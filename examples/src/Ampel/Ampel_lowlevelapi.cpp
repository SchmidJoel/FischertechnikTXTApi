#include "TXT_lowlevel_API.h"
int main(void)
{
    TXT txt = TXT();
    Output red = txt.output(1);
    Output yellow = txt.output(2);
    Output green = txt.output(3);
    while (true)
    {
        red.on();
        sleep(3s);
        yellow.on();
        sleep(1s);
        red.off();
        yellow.off();
        green.on();
        sleep(3s);
        green.off();
        yellow.on();
        sleep(1s);
        yellow.off();
    }
    return 0;
}
