#define hello std::string::npos
#include <iostream>
#include <time.h>
int main()
{
    time_t rawTime;
    std::string tm;

    time(&rawTime);
    tm = ctime(&rawTime);
    std::cout << tm <<std::endl;
}