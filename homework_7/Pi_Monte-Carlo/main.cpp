#include <iostream>
#include <random>
#include <thread>
#include <future>
#include <cstdint>
#include "../../homework_1/Timer.h"


auto sequential_algorithm (std::uint64_t N) {
    std::uniform_real_distribution < double > distribution(0.0, 2.0);
    std::random_device rd;  // так как треды создаются неодновременно, эта величина у них будет разная
    std::mt19937 engine(rd());
    std::uint64_t counter = 0;
    double dx, dy, x, y;
    for (auto i = 0U; i < N; ++i)
    {
        x = distribution(engine);
        y = distribution(engine);
        dx = x - 1.0;
        dy = y - 1.0;
        if (dx*dx + dy*dy <= 1.0)
        {
            counter++;
        }
    }
    return 4.0*counter / N;
}

auto parallel_algorithm (std::uint64_t N) {
    const std::uint64_t num_threads = std::thread::hardware_concurrency();
    const std::uint64_t block_size = N / num_threads;
    std::vector < std::future < double > > futures(num_threads - 1);

    for (auto i = 0U; i < num_threads-1; i++)
    {
        futures[i] = std::async(std::launch::async, sequential_algorithm, block_size);
    }
//    std::cout << "#######";
    auto last_N = N - block_size*(num_threads-1);
    auto last_res = last_N * sequential_algorithm(last_N);
    double thread_res = 0;
    for (auto i = 0U; i < num_threads-1; i++)
    {
        thread_res += futures[i].get();
    }
    last_res += block_size*thread_res;
    return 1.0 * last_res / N;
}

int main() {
    std::cout << "Sequential algorithm working:\n";
    Timer time;
    auto pi = sequential_algorithm(100000000);
    time.StopTimer();
    std::cout << pi << std::endl;
    auto sequential_algorithm_time = time.GetTime();
    std::cout << "Parallel algorithm working:\n";
    time.Clear();
    time.SetTimer();
    pi = parallel_algorithm(100000000);
    std::cout << pi << std::endl;
    time.StopTimer();
    auto parallel_algorithm_time = time.GetTime();
    std::cout << "sequential_algorithm_time/parallel_algorithm_time = " << 1.0*sequential_algorithm_time/parallel_algorithm_time;
    /*
     * Выигрыш в скорости получается примерно в 6.6 раза.
     */
    return EXIT_SUCCESS;
}