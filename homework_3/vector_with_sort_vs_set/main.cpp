#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <random>
#include "../../homework_1/Timer.h"
#include <array>
#include <fstream>
#include <filesystem>

template <class T>
double TimeSetInsert(const T & random_numbers, size_t number_iterations)
{
    auto t = Timer();
    t.StopTimer();
    std::set < int > s;
    for (auto i = 0U; i < number_iterations; ++i)
    {
        s.clear();
        t.SetTimer();
        s.insert(random_numbers.begin(), random_numbers.end());
        t.StopTimer();
    }
    return static_cast < double >(t.GetTime()) / number_iterations;
}

template <class T>
double TimeVectorInsertAndSort(const T & random_numbers, size_t number_iterations)
{
    auto t = Timer();
    t.StopTimer();
    std::vector < int > v(random_numbers.size(), 0);
    for (auto i = 0U; i < number_iterations; ++i)
    {
//        v.clear();
        t.SetTimer();
        std::copy(random_numbers.begin(), random_numbers.end(), v.begin());
        std::sort(v.begin(), v.end());
        t.StopTimer();
    }
    return static_cast < double >(t.GetTime()) / number_iterations;
}

template <size_t N, class T >
double TimeArrayInsertAndSort(const T & random_numbers, size_t number_iterations)
{
    auto t = Timer();
    t.StopTimer();
    std::array < int, N > arr {};
    for (auto i = 0U; i < number_iterations; ++i)
    {
//        arr.clear();
        t.SetTimer();
        std::copy(random_numbers.begin(), random_numbers.end(), arr.begin());
        std::sort(arr.begin(), arr.end());
        t.StopTimer();
    }
    return static_cast < double >(t.GetTime()) / number_iterations;
}

template<class F>
double MakeMeasurement(size_t N, size_t number_iterations, F f)
{
    std::vector < int > random_numbers (N, 0);
    std::iota (random_numbers.begin(), random_numbers.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_numbers.begin(), random_numbers.end(), g);
    return f(random_numbers, number_iterations);
}

template<size_t N>
double MakeMeasurement(size_t number_iterations)
{
    std::vector < int > random_numbers (N, 0);
    std::iota (random_numbers.begin(), random_numbers.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_numbers.begin(), random_numbers.end(), g);
    return TimeArrayInsertAndSort< N >(random_numbers, number_iterations);
}
template <class T, class P>
void to_csv(const T & statistics, const std::filesystem::path & PATH, const P & columns, const P & rows)
{
    std::fstream f;
    f.open(PATH, std::ios::out);
    f << ',' << columns[0] << ',' << columns[1] << ',' << columns[2] << ',' << columns[3] << '\n';
    for (size_t i{}; i < statistics.size(); ++i)
    {
        f << rows[i] << " (μs)";
        for (size_t k{}; k < statistics[0].size(); ++k)
        {
            f << ',' << statistics[i][k];
        }
        f << '\n';
    }
}

int main()
{
    std::vector < std::vector < double > > statistics (3, std::vector < double > (4, 0));
    std::vector < std::string > columns {
        "1e2", "1e3", "1e4", "1e5"
    };
    std::vector < std::string > rows {
            "array", "vector", "set"
    };
    constexpr std::array < int, 4> sizes {100, 1000, 10000, 100000};
    const int NUMBER_ITERATIONS = 1000;
//  time array:
    std::cout << "Timing array...\n";
    statistics[0][0] = MakeMeasurement < sizes[0] >(NUMBER_ITERATIONS);
    statistics[0][1] = MakeMeasurement < sizes[1] >(NUMBER_ITERATIONS);
    statistics[0][2] = MakeMeasurement < sizes[2] >(NUMBER_ITERATIONS);
    statistics[0][3] = MakeMeasurement < sizes[3] >(NUMBER_ITERATIONS);
//  time vector:
    std::cout << "Timing vector...\n";
    statistics[1][0] = MakeMeasurement(sizes[0], NUMBER_ITERATIONS, TimeVectorInsertAndSort <std::vector < int > >);
    statistics[1][1] = MakeMeasurement(sizes[1], NUMBER_ITERATIONS, TimeVectorInsertAndSort <std::vector < int > >);
    statistics[1][2] = MakeMeasurement(sizes[2], NUMBER_ITERATIONS, TimeVectorInsertAndSort <std::vector < int > >);
    statistics[1][3] = MakeMeasurement(sizes[3], NUMBER_ITERATIONS, TimeVectorInsertAndSort <std::vector < int > >);
//  time set:
    std::cout << "Timing set...\n";
    statistics[2][0] = MakeMeasurement(sizes[0], NUMBER_ITERATIONS, TimeSetInsert <std::vector < int > >);
    statistics[2][1] = MakeMeasurement(sizes[1], NUMBER_ITERATIONS, TimeSetInsert <std::vector < int > >);
    statistics[2][2] = MakeMeasurement(sizes[2], NUMBER_ITERATIONS, TimeSetInsert <std::vector < int > >);
    statistics[2][3] = MakeMeasurement(sizes[3], NUMBER_ITERATIONS, TimeSetInsert <std::vector < int > >);
    std::cout << "Finished!";
    to_csv(statistics, "../statistics/results.csv", columns, rows);
    return 0;
}