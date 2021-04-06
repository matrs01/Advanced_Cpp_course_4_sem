#include <algorithm>
#include <atomic>
#include <future>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <mutex>


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
            first++;
            second++;
        }
        return true;
    }
    void operator()(Iterator first, Iterator last, const std::string & element,
                    std::vector < int > & container, size_t index, std::mutex & mutex)
    {
        for (const std::size_t size = element.size()
                ; (first != last); ++first, ++index)
        {
            if (compare(first, element.cbegin(), size))
            {
                std::scoped_lock lock(mutex);
                container.push_back(index);
            }
        }
    }
};

template < typename Iterator>
void parallel_find(Iterator first, Iterator last, std::string & element, std::vector < int > & container)
{
    const std::size_t length = std::distance(first, last);
    const int size = element.size();

    std::mutex mutex;

    if (!length)
        return;

    const std::size_t num_threads = std::thread::hardware_concurrency();

    const std::size_t block_size = length / num_threads;

    std::vector < std::future < void > > threads(num_threads - 1);

    {
        Iterator block_start = first;
        Iterator block_end = first;

        std::size_t index = 0;

        for (std::size_t i = 0; i < (num_threads - 1); ++i)
        {
            std::advance(block_end, block_size + std::min(size, std::distance(block_end, last)) - size + 1);
            index = std::distance(first, block_start);

            threads[i] = std::async(std::launch::async, Searcher < Iterator> (),
                                     block_start, block_end, std::ref(element), std::ref(container),
                                     index, std::ref(mutex));

            block_start = block_end;
        }
        index = std::distance(first, block_start);
        if (std::distance(block_start, last) >= size)
        {
            Searcher < Iterator> ()(block_start, last, element, container, index, mutex);
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
    parallel_find(data.cbegin(), data.cend(), line, indexes);
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

