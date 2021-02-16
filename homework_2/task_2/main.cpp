#include <iostream>
#include <functional>
#include <vector>
#include <list>
#include <array>
#include <deque>
#include <forward_list>
#include <fstream>
#include <random>
#include "../../homework_1/Timer.h"


template <typename T>
void SortWithInbuiltFunction(T & v) {
    v.sort();
}

template <typename T>
void SortWithStdFunction(T & v) {
    std::sort(v.begin(), v.end());
}

template<typename T, typename F>
double TimeIteration(T v, void (*f)(F)) {
    Timer t{};
    f(v);
    return t.GetTime();
}

template <typename T, typename F>
double TimeIt(T & v, void (*f)(F), size_t num_iterations){
    double av_time = 0;
    for (int i = 0U; i < num_iterations; ++i) {
        av_time += TimeIteration(v, f);
    }
    return av_time / num_iterations;
}

int main() {
    constexpr size_t num_elements = 100000;
    std::vector < int > vec (num_elements, 0);
    std::list < int > list {};
    std::forward_list< int > f_list {};
    std::deque < int > deque {};
    std::array < int, num_elements > arr {};

    list.resize(num_elements);
    f_list.resize(num_elements);
    deque.resize(num_elements);

    std::uniform_int_distribution<int> distribution(0, 100000);
    std::random_device random_device;
    std::mt19937 engine {random_device()};
    auto generator = std::bind(distribution, engine);

    std::generate_n(vec.begin(), num_elements, generator);
    std::generate_n(list.begin(), num_elements, generator);
    std::generate_n(f_list.begin(), num_elements, generator);
    std::generate_n(deque.begin(), num_elements, generator);
    std::generate_n(arr.begin(), num_elements, generator);

    size_t t_vector = TimeIt(vec,
                              SortWithStdFunction<std::vector< int >>,
                              10000);
    size_t t_array = TimeIt(arr,
                              SortWithStdFunction<std::array<int, num_elements>>,
                              10000);
    size_t t_list = TimeIt(list,
                              SortWithInbuiltFunction<std::list< int >>,
                              10000);
    size_t t_f_list = TimeIt(f_list,
                              SortWithInbuiltFunction<std::forward_list< int >>,
                             10000);
    size_t t_deque = TimeIt(deque,
                              SortWithStdFunction<std::deque< int >>,
                              10000);
    std::fstream f;
    f.open("../statistics/table.csv", std::ofstream::out);
    f << "Container" << ',' << "Sorting time" << '\n' << "vector" << ',' <<
    t_vector << '\n' << "array" << ',' << t_array << '\n' << "list" << ',' <<
    t_list << '\n' << "forward list" << ',' << t_f_list << '\n' << "deque" << ',' << t_deque << '\n';

    return 0;
}
