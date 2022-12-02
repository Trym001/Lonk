//
// Created by Bruker on 02.12.2022.
//

#include "../include/system_timer.hpp"

void timer::start()
{
    m_StartTime = std::chrono::system_clock::now();
    m_bRunning = true;
}

void timer::stop()
{
    m_EndTime = std::chrono::system_clock::now();
    m_bRunning = false;
}

double timer::elapsedMilliseconds()
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

double timer::elapsedSeconds()
{
    return elapsedMilliseconds() / 1000.0;
}