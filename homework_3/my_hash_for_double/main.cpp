#include <iostream>
#include <cstring>
#include <vector>
#include <random>
#include <fstream>
#include <limits>
#include <iomanip>

struct Hasher
{
    uint64_t operator()(const double & x) const noexcept {
        uint64_t hash;
        std::memcpy( &hash, &x, sizeof( double ) );
        return hash;
    }
};

int main()
{
    Hasher hasher;
    const int hash_size = 128;
    std::vector < int > hash_vec (hash_size, 0);

    std::uniform_real_distribution<> distribution(0., 0.1);
    std::random_device random_device;
    std::mt19937 engine {random_device()};

    double x = -10000000;
    int iter{};
    double percentile {};
    std::cout << "0.00%";
    while (x < 10000000)
    {
//      progress visualisation:
        if (iter%1000000==0){
            system("cls");
            percentile = ((x+10000000)/20000000*100);
            std::cout << std::setprecision(2) << percentile << "%\n[";
            for (size_t i{}; i <= static_cast < int > (percentile); i++)
            {
                std::cout << '#';
            }
            for (size_t i = (static_cast < int >(percentile) + 1); i <= 100; i++)
            {
                std::cout << '-';
            }
            std::cout << ']';
        }
        iter++;
//      ####
        hash_vec[hasher(x) % hash_size] += 1;
        x += distribution(random_device);
    }
    std::fstream f;
    f.open("../statistics/stats.csv", std::ios::out);
    for (const auto & item: hash_vec)
    {
        f << item << ',';
    }
    return 0;
}
