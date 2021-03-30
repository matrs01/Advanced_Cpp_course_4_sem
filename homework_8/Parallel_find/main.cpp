#include <algorithm>
#include <atomic>
#include <future>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <fstream>
#include <string>


template < typename Iterator >
struct Searcher
{
    bool compare (Iterator first, Iterator second, size_t size)
    {
        for (auto i = 0U; i < size; ++i)
        {
            if (*first != *second)
            {
                return false;
            }
            std::advance(first, 1);
            std::advance(second, 1);
        }
        return true;
    }
    void operator()(Iterator first, Iterator last, std::string & element,
                    std::vector < int > & container, size_t index) noexcept
    {
        const std::size_t size = element.size();
        for (; (first != last); ++first)
        {
            if (compare(first, element.begin(), size))
            {
                container.push_back(index);
            }
            index++;
        }
    }
};

template < typename Iterator>
void parallel_find(Iterator first, Iterator last, std::string & element, std::vector < int > & container)
{
    const std::size_t length = std::distance(first, last);
    const int size = element.size();

    if (!length)
        return;

    const std::size_t min_per_thread = 25;
    const std::size_t max_threads =
            (length + min_per_thread - 1) / min_per_thread;

    const std::size_t hardware_threads =
            std::thread::hardware_concurrency();

    const std::size_t num_threads =
            std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const std::size_t block_size = length / num_threads;

    std::vector < std::future < void > > threads(num_threads - 1);

    {
        Iterator block_start = first;
        std::size_t index = 0;

        for (std::size_t i = 0; i < (num_threads - 1); ++i)
        {
            Iterator block_end = block_start;
            std::advance(block_end, block_size + std::min(size, std::distance(block_end, last)) - size + 1);
            index = std::distance(first, block_start);

            threads[i] = std::async(Searcher < Iterator> (),
                                     block_start, block_end, std::ref(element), std::ref(container), index);

            block_start = block_end;
        }
        index = std::distance(first, block_start);
        if (std::distance(block_start, last) >= size)
        {
            Searcher < Iterator> ()(block_start, last, element, container, index);
        }
    }
}


int main(int argc, char ** argv)
{
    std::fstream file ("../data.txt", std::ios::in);
    std::string data;
    std::getline(file, data);

    std::string line;
    std::getline(std::cin, line);

    std::vector < int > indexes;
    parallel_find(data.begin(), data.end(), line, indexes);
    std::sort(indexes.begin(), indexes.end());

    if (indexes.empty())
    {
        std::cout << "No matching";
    }
    else
    {
        for (auto x: indexes)
        {
            std::cout << x << ' ';
        }
    }

    return EXIT_SUCCESS;
}

