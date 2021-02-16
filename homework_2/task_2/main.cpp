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
double TimeIt(const T & v, F f, size_t num_iterations){
    double av_time = 0;
    for (int i = 0U; i < num_iterations; ++i) {
        av_time += TimeIteration(v, f);
    }
    return av_time / num_iterations;
}

int main() {
    /*
     *  Создаем экземпляры классов std::vector, std::list, std::forward_list,
     *  std::deque и std::array и заполняем их одинаковыми наборами рандомных
     *  чисел. Функция TimeIt замеряет среднее время num_iterations замеров
     *  работы функции f, которую она принимает как аргумент, и мы будем подавать
     *  туда функции сортировки. TimeIt вызывает функцию TimeIteration для замера
     *  времени одной итерации, эта функция копирует контейнер перед применением
     *  функции-фргумента f, чтобы не портить данные в контейнере.
     *  По результатам замера лучшее время показали std::list и std::forward_list,
     *  после них идет std::array (но это немного нечестно, так как он полностью
     *  на стэке), затем идет std::vector и с большим отставанием (пости на порядок)
     *  идет std::deque (см. таблицу в statistics/table.pdf)
     */
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

    std::generate_n(vec.begin(), num_elements, [&engine, &distribution](){return distribution(engine);});
    std::copy(vec.begin(), vec.end(), list.begin());
    std::copy(vec.begin(), vec.end(), f_list.begin());
    std::copy(vec.begin(), vec.end(), deque.begin());
    std::copy(vec.begin(), vec.end(), arr.begin());

    size_t t_vector = TimeIt(vec,
                              SortWithStdFunction<std::vector< int >>,
                              1000);
    size_t t_array = TimeIt(arr,
                              SortWithStdFunction<std::array<int, num_elements>>,
                              1000);
    size_t t_list = TimeIt(list,
                              SortWithInbuiltFunction<std::list< int >>,
                              1000);
    size_t t_f_list = TimeIt(f_list,
                              SortWithInbuiltFunction<std::forward_list< int >>,
                             1000);
    size_t t_deque = TimeIt(deque,
                              SortWithStdFunction<std::deque< int >>,
                              1000);
    std::fstream f;
    f.open("../statistics/table.csv", std::ofstream::out);
    f << "Container" << ',' << "Sorting time, nano sec" << '\n' << "vector" << ',' <<
    t_vector << '\n' << "array" << ',' << t_array << '\n' << "list" << ',' <<
    t_list << '\n' << "forward list" << ',' << t_f_list << '\n' << "deque" << ',' << t_deque << '\n';

    return 0;
}
