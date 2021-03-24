#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <cmath>

template < typename Iterator, typename UnaryFunction >
void parallel_for_each(Iterator first, Iterator last, UnaryFunction f, size_t max_size)
{
    const std::size_t length = std::distance(first, last);

    if (length <= max_size)
    {
        std::for_each(first, last, f);
    }
    else
    {
        Iterator middle = first;
        std::advance(middle, length / 2);

        std::future < void > first_half_result =
                std::async(parallel_for_each < Iterator, UnaryFunction >, first, middle, f, max_size);

        parallel_for_each(middle, last, f, max_size);

        first_half_result.wait();
    }
}

int main() {
    const size_t length = 100000000;
    std::vector < int > v(length);
    std::iota(v.begin(), v.end(), 1);
    auto f = [](auto & x) {x = std::sin(std::sin(std::tan(1.5 - std::sin(x)))); };

    std::cout << "Sequential for_each working:\n";
    std::for_each(std::begin(v), std::end(v), f);

    const size_t num_threads = std::thread::hardware_concurrency();
    std::cout << "Parallel for_each working:\n";
    parallel_for_each(std::begin(v), std::end(v), f, (num_threads) ? length/num_threads : length);

    system("pause");
    return EXIT_SUCCESS;
}

