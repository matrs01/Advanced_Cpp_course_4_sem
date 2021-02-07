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
    ~Timer(){
        auto end = clock_time::now();

        std::cout<<"nanoseconds: "<<std::chrono::duration_cast<
                std::chrono::nanoseconds> (time_period + (end-m_begin)*(1-stopped)).count() << std::endl;
    }
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

private:
    time_point_t m_begin;
    bool stopped = false;
    time_duration time_period {0};
};

int main() {
    std::vector v(10000000U, 0U);
    for (auto i = 0U; i < std::size(v); ++i) {
        v[i] = 10000000U - i;
    }

    {
        Timer t;
        std::sort(v.begin(), v.end());

        t.StopTimer();
        for (auto i = 0U; i < std::size(v); ++i) {
            v[i] = 10000000U - i;
        }
        t.SetTimer();
        t.SetTimer();
        std::sort(v.begin(), v.end());
        t.StopTimer();
        t.StopTimer();
        for (auto i = 0U; i < std::size(v); ++i) {
            v[i] = 10000000U - i;
        }
        t.SetTimer();
        std::sort(v.begin(), v.end());
//        t.StopTimer();
    }

    return 0;
}

