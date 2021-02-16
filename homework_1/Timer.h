#ifndef ADVANCED_CPP_COURSE_4_SEM_TIMER_H
#define ADVANCED_CPP_COURSE_4_SEM_TIMER_H
#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

class Timer{
public:
    using clock_time = std::chrono::steady_clock;
    using time_point_t = clock_time::time_point;
    using time_duration = std::chrono::nanoseconds;
    Timer() : m_begin(clock_time::now()) {};

//    virtual ~Timer() = 0;
    void StopTimer() {
        auto end = clock_time::now();
        if (!stopped){
            time_period += end - m_begin;
            stopped = true;
        }
    }
    void SetTimer() {
        if (stopped){
            m_begin = clock_time::now();
            stopped = false;
        }
    }
    int GetTime() {
        auto end = clock_time::now();
        return std::chrono::duration_cast<
                std::chrono::nanoseconds> (time_period + (end-m_begin)*(1-stopped)).count();
    }

private:
    time_point_t m_begin;
    bool stopped = false;
    time_duration time_period {0};
};


#endif //ADVANCED_CPP_COURSE_4_SEM_TIMER_H
