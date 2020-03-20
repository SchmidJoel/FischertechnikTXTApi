#include "TXT_lowlevel_API.h"
#include <chrono>
#include <thread>

void run_motor(Motor *motor, int sec)
{
    motor->left(512);

    std::this_thread::sleep_for(std::chrono::seconds(sec));

    motor->stop();
}

int main(void)
{
    TXT txt = TXT();
    Motor eins = txt.motor(1);
    Motor zwei = txt.motor(2);

    std::thread one(run_motor, &eins, 5);
    std::thread two(run_motor, &zwei, 3);

    one.join();
    two.join();

    return 0;
}