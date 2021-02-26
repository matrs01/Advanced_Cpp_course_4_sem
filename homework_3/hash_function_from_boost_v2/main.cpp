#include <string>
#include <unordered_set>
#include <set>
#include <random>
#include <fstream>
#include <iostream>

template < typename T >
void hash_combine(std::size_t & seed, const T & value) noexcept
{
    seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t & seed, const T & value) noexcept
{
    hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t & seed, const T & value, const Types & ... args) noexcept
{
    hash_combine(seed, value);
    hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
    std::size_t seed = 0;
    hash_value(seed, args...);
    return seed;
}

std::set < int > GenerateNRandomNumbers(size_t N, int min, int max)
{
//  Generating random numbers without repeating:
    std::set < int > s {};
    static std::uniform_int_distribution<int> distribution(min, max);
    static std::random_device random_device;
    static std::mt19937 engine {random_device()};
    while (s.size() < N) {
        s.insert(distribution(engine));
    }
    return s;
}

std::set < std::string > GenerateNRandomNames(size_t N,
                                              const std::string & alphabet,
                                              size_t name_length)
{
    std::set < std::string > s {};
    static std::uniform_int_distribution<int> distribution(0, alphabet.size()-1);
    static std::random_device random_device;
    static std::mt19937 engine {random_device()};
    std::string name(name_length, '_');
    while (s.size() < N)
    {
        std::generate(std::begin(name), std::end(name), [&](){return alphabet[distribution(engine)];});
        s.insert(name);
    }
    return s;
}

int main(){
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const size_t num_instances = 10000000;
    const size_t save_interval = 500000;
    const size_t name_length = 6;
    std::set < int > random_numbers = GenerateNRandomNumbers(num_instances,
                                                             std::numeric_limits <int>::min(),
                                                             std::numeric_limits <int>::max());
    std::set < std::string > random_names = GenerateNRandomNames(num_instances, alphabet, name_length);
    std::set < std::size_t > hash_set {};
    size_t num_collisions = 0U;
    auto name_it = random_names.begin();
    auto num_it = random_numbers.begin();
    std::fstream f;
    f.open("../statistics/collision_stat.csv", std::ios::out);
    f << "Number_of_instances,Number_of_collisions\n";
    for (auto i = 0U; i < random_names.size(); ++i)
    {
        auto [iter, flag] = hash_set.insert(hash_value(*name_it, *num_it));
        if (!flag)
        {
            ++num_collisions;
        }
        if (i % save_interval == 0)
        {
            f << i << ',' << num_collisions << '\n';
            std::cout << '#';
        }
        std::advance(name_it, 1);
        std::advance(num_it, 1);
    }
    return 0;
}
