#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include <random>
#include <fstream>
#include <filesystem>

unsigned int RSHash(const std::string &str)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = hash * a + str[i];
        a    = a * b;
    }

    return hash;
}

unsigned int JSHash(const std::string &str)
{
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash ^= ((hash << 5) + str[i] + (hash >> 2));
    }

    return hash;
}

unsigned int PJWHash(const std::string &str)
{
    const auto BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    const auto ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
    const auto OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
    const unsigned int HighBits          =
            (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    unsigned int hash = 0;
    unsigned int test = 0;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = (hash << OneEighth) + str[i];

        if ((test = hash & HighBits) != 0)
        {
            hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return hash;
}

unsigned int ELFHash(const std::string &str)
{
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = (hash << 4) + str[i];

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

unsigned int BKDRHash(const std::string &str)
{
    unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = (hash * seed) + str[i];
    }

    return hash;
}

unsigned int SDBMHash(const std::string &str)
{
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }

    return hash;
}

unsigned int DJBHash(const std::string &str)
{
    unsigned int hash = 5381;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = ((hash << 5) + hash) + str[i];
    }

    return hash;
}

unsigned int DEKHash(const std::string &str)
{
    unsigned int hash = str.size();
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
    }

    return hash;
}

unsigned int APHash(const std::string &str)
{
    unsigned int hash = 0xAAAAAAAA;
    unsigned int i    = 0;

    for (i = 0; i < str.size(); ++i)
    {
        hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ str[i] * (hash >> 3)) :
                (~((hash << 11) + (str[i] ^ (hash >> 5))));
    }

    return hash;
}

std::unordered_set < std::string > GenerateNRandomStrings(size_t N,
                                              const std::string & alphabet,
                                              size_t string_length)
{
    std::unordered_set < std::string > s {};
    static std::uniform_int_distribution<int> distribution(0, alphabet.size()-1);
    static std::random_device random_device;
    static std::mt19937 engine {random_device()};
    std::string name(string_length, '_');
    while (s.size() < N)
    {
        std::generate(std::begin(name), std::end(name), [&](){return alphabet[distribution(engine)];});
        s.insert(name);
    }
    return s;
}

template <typename F>
auto TestHashFunction(F hash_f, const std::string & hash_name, const std::unordered_set < std::string > & random_strings,
                      const size_t save_interval = 500000)
{
    std::set < std::size_t > hash_set {};
    size_t num_collisions = 0U;
    auto str_it = random_strings.begin();
    std::vector < std::pair < size_t, size_t > > collision_stat {};
    for (auto i = 0U; i < random_strings.size(); ++i)
    {
        auto [iter, flag] = hash_set.insert(hash_f(*str_it));
        if (!flag)
        {
            ++num_collisions;
        }
        if (i % save_interval == 0)
        {
            collision_stat.emplace_back(i, num_collisions);
            std::cout << '#';
        }
        std::advance(str_it, 1);
    }
    return std::make_pair(collision_stat, hash_name);
}

void to_csv(const std::vector < std::pair < std::vector < std::pair < size_t, size_t > >, std::string > > & stat,
            const std::filesystem::path & path){
    std::fstream f;
    f.open(path, std::ios::out);
    f << "Number_of_instances";
    for (const auto & x: stat[0].first)
    {
        f << ',' << x.first;
    }
    f << '\n';
    for (const auto & stat_i: stat)
    {
        f << stat_i.second;
        for (const auto stat_i_j: stat_i.first)
        {
            f << ',' << stat_i_j.second;
        }
        f << '\n';
    }
}

int main(){
    /*
     * Функция GenerateNRandomStrings генерирует num_instances слуяайных неповторяющихся строк
     * длины string_length. Эта функция возвращает unordered_set, так как некоторые хэш-функции
     * оказались чувствительными к порядку подачи строк (как в случае с set).
     * Результаты в /statistics/plot.pdf
     */
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    const size_t num_instances = 10000000;
    const size_t string_length = 10;
    using crds_t = std::pair <size_t, size_t >;
    using plot_data_t = std::pair < std::vector < crds_t >, std::string >;
    std::unordered_set < std::string > random_strings = GenerateNRandomStrings(num_instances,
                                                                               alphabet,
                                                                               string_length);
    std::vector < plot_data_t > stat;
    stat.push_back(TestHashFunction(RSHash, "RSHash", random_strings));
    stat.push_back(TestHashFunction(JSHash, "JSHash", random_strings));
    stat.push_back(TestHashFunction(PJWHash, "PJWHash", random_strings));
    stat.push_back(TestHashFunction(ELFHash, "ELFHash", random_strings));
    stat.push_back(TestHashFunction(BKDRHash, "BKDRHash", random_strings));
    stat.push_back(TestHashFunction(SDBMHash, "SDBMHash", random_strings));
    stat.push_back(TestHashFunction(DJBHash, "DJBHash", random_strings));
    stat.push_back(TestHashFunction(DEKHash, "DEKHash", random_strings));
    stat.push_back(TestHashFunction(APHash, "APHash", random_strings));

    to_csv(stat, "../statistics/collision_stat.csv");
    return 0;
}