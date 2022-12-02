//
// Created by Bruker on 02.12.2022.
//

#ifndef LONK_02_SYSTEM_TIMER_HPP
#define LONK_02_SYSTEM_TIMER_HPP

#include <iostream>
#include <chrono>
#include <ctime>
#include <cmath>

// src - https://gist.github.com/mcleary/b0bf4fa88830ff7c882d
class timer
{
public:
    void start();

    void stop();

    double elapsedMilliseconds();

    double elapsedSeconds();

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};


#endif //LONK_02_SYSTEM_TIMER_HPP
