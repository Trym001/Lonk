//
// Created by Bruker on 02.12.2022.
//

#include "controller/system_timer.hpp"


void system_timer::start()
{
    m_StartTime = std::chrono::system_clock::now();
    m_bRunning = true;
}

void system_timer::stop()
{
    m_EndTime = std::chrono::system_clock::now();
    m_bRunning = false;
}

double system_timer::elapsedMilliseconds()
{
    std::chrono::time_point<std::chrono::system_clock> endTime;

    if(m_bRunning)
    {
        endTime = std::chrono::system_clock::now();
    }
    else
    {
        endTime = m_EndTime;
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_StartTime).count();
}

//trur ikkje elapsedSeconds e bra funksjons. Funksjonen unna 100% avhengig av funksjonen øve. Hør me guttan.

double system_timer::elapsedSeconds()
{
    return elapsedMilliseconds() / 1000.0;
}