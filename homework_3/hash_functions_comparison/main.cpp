#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include <random>
#include <fstream>
#include <filesystem>

unsigned int RSHash(const char* str, unsigned int length)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }

    return hash;
}

unsigned int JSHash(const char* str, unsigned int length)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }

    return hash;
}

unsigned int PJWHash(const char* str, unsigned int length)
{
    const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          =
            (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << OneEighth) + (*str);

        if ((test = hash & HighBits) != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

unsigned int ELFHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash << 4) + (*str);

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

unsigned int BKDRHash(const char* str, unsigned int length)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

unsigned int SDBMHash(const char* str, unsigned int length)
{
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned int DJBHash(const char* str, unsigned int length)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) + hash) + (*str);
    }

    return hash;
}
unsigned int DEKHash(const char* str, unsigned int length)
{
    unsigned int hash = length;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
    }

    return hash;
}

unsigned int APHash(const char* str, unsigned int length)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;

    for (i = 0; i < length; ++str, ++i)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*str) * (hash >> 3)) :
                (~((hash << 11) + ((*str) ^ (hash >> 5))));
    }

    return hash;
}

std::set < std::string > GenerateNRandomStrings(size_t N,
                                              const std::vector < std::string > & alphabet,
                                              size_t length)
{
    std::set < std::string > s {};
    std::uniform_int_distribution<int> distribution(0, alphabet.size()-1);
    std::random_device random_device;
    std::mt19937 engine {random_device()};
    std::string name {};
    while (s.size() < N)
    {
        for (size_t i{}; i < length; ++i)
        {
            name += alphabet[distribution(engine)];
        }
        s.insert(name);
        name = "";
    }
    return s;
}

template <class F>
auto TestHashFunction(F f, const std::string & hash_name)
{
    const size_t hash_size = 1024;
    std::vector < std::pair < double, int > > collisions_stat {};
    std::vector < int >  occupancy_stat (hash_size, 0);
    std::set < std::string > strings {};
    std::string temp_str;
//    std::vector < int > hash_vec (hash_size, 0);
    size_t num_of_strings = 100;
    size_t num_collisions = 0;

    std::vector < std::string > alphabet {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                                          "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "};


    for (; num_of_strings < 2*hash_size; num_of_strings += 100)
    {
        strings = GenerateNRandomStrings(num_of_strings, alphabet, 6);
        auto strings_iter = strings.begin();
        for (size_t i{}; i < 100; ++i)
        {
            temp_str = *(strings_iter++);
            occupancy_stat[f(temp_str.c_str(), temp_str.size()) % hash_size] += 1;
        }
        num_collisions = 0;
        for (const auto & x: occupancy_stat)
        {
            num_collisions += ((x > 0) ? x-1 : 0);
        }
        collisions_stat.push_back({(static_cast <double > (num_of_strings)) / hash_size, num_collisions});
    }
    return std::make_tuple(collisions_stat, occupancy_stat, hash_name);
}

void to_csv(const std::vector < std::tuple < std::vector < std::pair < double, int > >, std::vector < int >, std::string > > & stats,
            const std::string & path)
{
    std::fstream f, g;
    f.open(path + "collisions_stat.csv", std::ios::out);
    g.open(path + "occupancy_stat.csv", std::ios::out);
    for (size_t i{}; i < (std::get< 0 > (stats[0])).size();  ++i)
    {
        f << ',' << (std::get< 0 > (stats[0]))[i].first;
    }
    f << '\n';
    for (size_t i{}; i < stats.size(); ++i)
    {
        f << std::get< 2 > (stats[i]);
        for (size_t j{}; j < (std::get< 0 > (stats[0])).size();  ++j)
        {
            f << ',' << (std::get< 0 > (stats[i]))[j].second;
        }
        f << '\n';
    }

    for (size_t i{}; i < stats.size(); ++i)
    {
        g << std::get< 2 > (stats[i]);
        for (size_t j{}; j < (std::get< 1 > (stats[0])).size(); ++j)
        {
            g << ',' << (std::get< 1 > (stats[i]))[j];
        }
        g << '\n';
    }

}

int main()
{
    std::vector < std::tuple < std::vector < std::pair < double, int > >, std::vector < int >, std::string > > stats;
    stats.push_back(TestHashFunction(RSHash, "RSHash"));
    stats.push_back(TestHashFunction(JSHash, "JSHash"));
    stats.push_back(TestHashFunction(PJWHash, "PJWHash"));
    stats.push_back(TestHashFunction(ELFHash, "ELFHash"));
    stats.push_back(TestHashFunction(BKDRHash, "BKDRHash"));
    stats.push_back(TestHashFunction(SDBMHash, "SDBMHash"));
    stats.push_back(TestHashFunction(DJBHash, "DJBHash"));
    stats.push_back(TestHashFunction(DEKHash, "DEKHash"));
    stats.push_back(TestHashFunction(APHash, "APHash"));

    to_csv(stats, "../statistics/");

    return 0;
}