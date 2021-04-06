#include <iostream>
#include <random>
#include <thread>
#include <future>
#include <cstdint>
#include "../../homework_1/Timer.h"



void sequential_algorithm (std::uint64_t N, std::atomic < int > & counter) {
    std::uniform_real_distribution < double > distribution(0.0, 2.0);
    std::random_device rd;  // так как треды создаются неодновременно, эта величина у них будет разная
    std::mt19937 engine(rd());
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
}

auto parallel_algorithm (std::uint64_t N, std::atomic < int > & counter) {
    const std::uint64_t num_threads = std::thread::hardware_concurrency();
    const std::uint64_t block_size = N / num_threads;
    std::vector < std::future < void > > futures(num_threads - 1);

    for (auto i = 0U; i < num_threads-1; i++)
    {
        futures[i] = std::async(std::launch::async, sequential_algorithm, block_size, std::ref(counter));
    }

    auto last_N = N - block_size*(num_threads-1);
    sequential_algorithm(last_N, std::ref(counter));

    for (auto i = 0U; i < num_threads-1; i++)
    {
        futures[i].get();
    }

    return 4.0 * counter.load()/ N;
}

int main() {
    std::atomic < int > counter = 0;
    std::cout << "Parallel algorithm working:\n";
    Timer time;
    auto pi = parallel_algorithm(100000000, counter);
    time.StopTimer();
    std::cout << pi << std::endl;
    auto parallel_algorithm_time = time.GetTime();
    std::cout << "parallel_algorithm_time = " << parallel_algorithm_time;
    return EXIT_SUCCESS;
}