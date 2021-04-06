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
void pop_op_seminar (T & container, std::size_t num_iter_per_thread)
{
    while (!execution_start.load())
    {
        std::this_thread::yield();
    }

    int temp;
    for (auto i = 0U; i < num_iter_per_thread; ++i)
    {
        container.pop(temp);
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

template <typename T, typename F>
auto test (T & container, std::size_t num_iter_per_thread, F & pop_function, F & push_function)
{
    auto num_threads = std::thread::hardware_concurrency();

    std::vector < std::future < void > > pushers (num_threads/2);
    std::vector < std::future < void > > popers (num_threads/2 - 1);
    for (auto & future: pushers)
    {
        future = std::async(std::launch::async, push_function, std::ref(container), num_iter_per_thread);
    }
    for (auto & future: popers)
    {
        future = std::async(std::launch::async, pop_function, std::ref(container), num_iter_per_thread);
    }
    Timer time;
    execution_start.store(true);
    pop_function(container, num_iter_per_thread);

    for (auto & future: pushers)
    {
        future.wait();
    }
    for (auto & future: popers)
    {
        future.wait();
    }
    time.StopTimer();
    return time.GetTime();
}

template<typename T, typename F1, typename F2>
auto time_it (T & container, F1 f, F2 pop_f, F2 push_f, std::size_t num_iter, std::size_t num_inserts)
{
    Timer time;
    for (auto i = 0U; i < num_iter; ++i)
    {
        f(container, num_inserts, pop_f, push_f);
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
    for (auto i = 0U; i < 10*inserts; ++i)
    {
        st_1.push(128);
        st_2.push(128);
        q_1.push(128);
        q_2.push(128);
    }

    std::cout << "boost_stack_time = " << time_it(st_1, test <boost::lockfree::stack < int >,
                                                     void (*)(boost::lockfree::stack<int> &, unsigned int)>,
                                                     pop_op_boost < boost::lockfree::stack < int > >,
                                                     push_op_boost < boost::lockfree::stack < int > >,
                                                     num_iter, inserts) << std::endl;
    std::cout << "seminar_stack_time = " << time_it(st_2, test <Threadsafe_Stack < int >,
                                                     void (*)(Threadsafe_Stack < int > &, unsigned int)>,
                                                     pop_op_seminar < Threadsafe_Stack < int > >,
                                                     push_op_seminar < Threadsafe_Stack < int > >,
                                                     num_iter, inserts) << std::endl;
    std::cout << "boost_queue_time = " << time_it(q_1, test <boost::lockfree::queue < int >,
                                                     void (*)(boost::lockfree::queue<int> &, unsigned int)>,
                                                     pop_op_boost < boost::lockfree::queue < int > >,
                                                     push_op_boost < boost::lockfree::queue < int > >,
                                                     num_iter, inserts) << std::endl;
    std::cout << "seminar_queue_time = " << time_it(q_2, test <Threadsafe_Queue < int >,
                                                           void (*)(Threadsafe_Queue < int > &, unsigned int)>,
                                                     pop_op_seminar < Threadsafe_Queue < int > >,
                                                     push_op_seminar < Threadsafe_Queue < int > >,
                                                     num_iter, inserts) << std::endl;

    return EXIT_SUCCESS;
}