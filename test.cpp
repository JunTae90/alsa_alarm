#include "alarm.hpp"
#include <thread>

float ttc = 2.5;
void alarmThread()
{   
    Alarm alarm = Alarm();
    while(1)
    {
        if (ttc < 2.6)
        {
            alarm.ring();
        }
    }
}

int main()
{
    thread t1(alarmThread);
    t1.join();
    return 0;
}