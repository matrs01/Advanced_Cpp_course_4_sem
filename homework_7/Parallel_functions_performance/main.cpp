#include <iostream>
#include <vector>
#include <execution>
#include <cmath>
#include <fstream>
#include <future>

#include "../../homework_1/Timer.h"


auto measure_for_each(size_t size){
    std::vector < double > v(size);
    double temp = 0.0;
    std::fill_n(v.begin(), size, 1.0);

    Timer t;
    t.StopTimer();

//    auto f = [](auto x){ std::sin(std::sin(std::tan(1.5 - std::sin(x))));};
    auto f = [&temp](auto x)
    {
        auto result = 0.0;
        for (std::size_t i = 0U; i < 10U; ++i)
        {
            result = std::sin(x + result);
        }
        temp = result;
    };
    t.SetTimer();
    std::for_each(std::execution::seq, v.begin(), v.end(), f);
    t.StopTimer();
    auto seq_time = t.GetTime();
    t.Clear();

    t.SetTimer();
    std::for_each(std::execution::par, v.begin(), v.end(), f);
    t.StopTimer();
    auto par_time = t.GetTime();
    t.Clear();

    return std::make_pair(seq_time, par_time);
}

auto measure_partial_sum(size_t size){
    std::vector < double > v(size);
    std::vector < double > temp(size);
    std::fill_n(v.begin(), size, 10.0);
    auto f = [](const auto lhs, const auto rhs)
    {
        auto result = 0.0;
        for (auto i = 0U; i < 100U; ++i)
        {
            result = std::sin(lhs + rhs + result);
        }
        return result;
    };

    Timer t;
    std::partial_sum(v.begin(), v.end(), temp.begin(), f);
    t.StopTimer();
    auto seq_time = t.GetTime();
    t.Clear();

    t.SetTimer();
    std::inclusive_scan(std::execution::par, v.begin(), v.end(), temp.begin(), f);
    t.StopTimer();
    auto par_time = t.GetTime();
    t.Clear();

    return std::make_pair(seq_time, par_time);
}

auto measure_inner_product(size_t size){
    std::vector < int > v1(size);
    std::iota(v1.begin(), v1.end(), 1);
    std::vector < int > v2(size);
    std::iota(v2.rbegin(), v2.rend(), 1);
    auto f = [](const auto lhs, const auto rhs)
    {
        auto result = 0.0;
        for (auto i = 0U; i < 100U; ++i)
        {
            result = std::sin(lhs + rhs + result);
        }
        return result;
    };

    Timer t;
    t.StopTimer();

    t.SetTimer();
    auto ipr1 = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0, f, f);
    t.StopTimer();
    auto seq_time = t.GetTime();
    t.Clear();

    t.SetTimer();
    auto ipr2 = std::transform_reduce(std::execution::par, v1.begin(), v1.end(), v2.begin(), 0, f, f);
    t.StopTimer();
    auto par_time = t.GetTime();
    t.Clear();

    return std::make_pair(seq_time, par_time);
}

template<typename F>
auto average_time(F f, size_t size, size_t repeats){
    size_t seq_time_mean = 0;
    size_t par_time_mean = 0;
    for (size_t i = 0; i<repeats; ++i){
        auto [seq_time, par_time] = f(size);
        seq_time_mean += seq_time;
        par_time_mean += par_time;
    }
    seq_time_mean /= repeats;
    par_time_mean /= repeats;

    return std::make_pair(seq_time_mean, par_time_mean);
}


int main(int argc, char ** argv)
{
    std::ofstream file("../statistics/data.csv", std::ios::out);
    file << "size,for_each (seq),for_each (par),partial_sum,inclusive_scan,inner_product,transform_reduce\n";
    for (size_t size = 1000; size<100000000; size*=10){
        size_t repeats = 10;
        auto for_each_time = average_time(measure_for_each, size, repeats);
        auto part_sum_time = average_time(measure_partial_sum, size, repeats);
        auto inn_prod_time = average_time(measure_inner_product, size, repeats);

        file << size << ','
             << for_each_time.first << ',' << for_each_time.second << ','
             << part_sum_time.first << ',' << part_sum_time.second << ','
             << inn_prod_time.first << ',' << inn_prod_time.second << '\n';
    }

    return EXIT_SUCCESS;
}