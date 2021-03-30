#include <iostream>
#include <vector>
#include <future>

#include <boost/lockfree/stack.hpp>
#include <boost/lockfree/queue.hpp>

#include "threadsafe_queue.h"
#include "threadsafe_stack.h"
#include "../../homework_1/Timer.h"

std::atomic < bool > execution_start = false;

template <typename T>
void pop_op_boost (T & container, std::size_t num_iter_per_thread)
{
    while (!execution_start.load())
    {
        std::this_thread::yield();
    }
    std::size_t counter = 0;
    int temp;
    while (counter < num_iter_per_thread)
    {
        counter += container.pop(temp);
    }
}


template <typename T>
void push_op_boost (T & container, std::size_t num_iter_per_thread)
{
    while (!execution_start.load())
    {
        std::this_thread::yield();
    }
    for (auto i = 0U; i < num_iter_per_thread; ++i)
    {
        container.bounded_push(123);
    }
}

template <typename T>
auto test_boost (T & container, std::size_t num_iter_per_thread)
{
    auto num_threads = std::thread::hardware_concurrency();
//    std::atomic < bool > flag = false;
    std::vector < std::future < void > > pushers (num_threads/2);
    std::vector < std::future < void > > popers (num_threads/2 - 1);
    for (auto & future: pushers)
    {
        future = std::async(std::launch::async, push_op_boost < T >, std::ref(container), num_iter_per_thread);
    }
    for (auto & future: popers)
    {
        future = std::async(std::launch::async, pop_op_boost < T >, std::ref(container), num_iter_per_thread);
    }
    Timer time;
    execution_start.store(true);
    pop_op_boost(container, num_iter_per_thread);
    while (!container.empty()){
        std::this_thread::yield();
    }
    time.StopTimer();
    return time.GetTime();
}

template <typename T>
void pop_op_seminar (T & container, std::size_t num_iter_per_thread)
{
    while (!execution_start.load())
    {
        std::this_thread::yield();
    }
    std::size_t counter = 0;
    int temp;
    for (auto i = 0U; i < num_iter_per_thread; ++i)
    {
        container.wait_and_pop(temp);
    }
}

template <typename T>
void push_op_seminar (T & container, std::size_t num_iter_per_thread)
{
    while (!execution_start.load())
    {
        std::this_thread::yield();
    }
    for (auto i = 0U; i < num_iter_per_thread; ++i)
    {
        container.push(123);
    }
}

template <typename T>
auto test_seminar (T & container, std::size_t num_iter_per_thread)
{
    auto num_threads = std::thread::hardware_concurrency();
//    std::atomic < bool > flag = false;
    std::vector < std::future < void > > pushers (num_threads/2);
    std::vector < std::future < void > > popers (num_threads/2 - 1);
    for (auto & future: pushers)
    {
        future = std::async(std::launch::async, push_op_seminar < T >, std::ref(container), num_iter_per_thread);
    }
    for (auto & future: popers)
    {
        future = std::async(std::launch::async, pop_op_seminar < T >, std::ref(container), num_iter_per_thread);
    }
    Timer time;
    execution_start.store(true);
    pop_op_seminar(container, num_iter_per_thread);
    while (!container.empty()){
        std::this_thread::yield();
    }
    time.StopTimer();
    return time.GetTime();
}

template<typename T, typename F>
auto time_it (T & container, F f, std::size_t num_iter, std::size_t num_inserts)
{
    Timer time;
    for (auto i = 0U; i < num_iter; ++i)
    {
        f(container, num_inserts);
    }
    time.StopTimer();
    return time.GetTime();
}

int main(int argc, char** argv)
{
    const std::size_t size = 1000000;
    const std::size_t num_iter = 1000;
    const std::size_t inserts = 100;

    boost::lockfree::stack < int > st_1 (size);
    boost::lockfree::queue < int > q_1 (size);
    Threadsafe_Stack < int > st_2;
    Threadsafe_Queue < int > q_2;
//    std::cout << test_boost(st_1, 100);
//    std::cout << test_seminar(st_2, 100);
    std::cout << "boost_stack_time = " << time_it(st_1, test_boost <boost::lockfree::stack < int >>,
                                                  num_iter, inserts) << std::endl;
    std::cout << "seminar_stack_time = " << time_it(st_2, test_seminar < Threadsafe_Stack < int > >,
                                                  num_iter, inserts) << std::endl;

    std::cout << "boost_queue_time = " << time_it(q_1, test_boost < boost::lockfree::queue < int > >,
                                                  num_iter, inserts) << std::endl;
    std::cout << "seminar_queue_time = " << time_it(q_2, test_seminar < Threadsafe_Queue < int > >,
                                                  num_iter, inserts) << std::endl;

    return EXIT_SUCCESS;
}