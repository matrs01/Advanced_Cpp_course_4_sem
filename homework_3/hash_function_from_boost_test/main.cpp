#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include <random>
#include <fstream>

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

class Customer
{
private:
    friend struct Customer_Hash;
    friend struct Customer_Equal;

public:
    explicit Customer(const std::string & name, const std::size_t mark) :
            m_name(name), m_mark(mark)
    {}

    ~Customer() noexcept = default;

public:
    friend std::ostream & operator << (std::ostream & stream, const Customer & customer)
    {
        return (stream << customer.m_name << "," << customer.m_mark);
    }

private:
    std::string m_name;
    std::size_t m_mark;
};

struct Customer_Hash
{
    std::size_t operator() (const Customer & customer) const noexcept
    {
        return hash_value(customer.m_name, customer.m_mark);
    }
};

struct Customer_Equal
{
    bool operator() (const Customer & lhs, const Customer & rhs) const noexcept
    {
        return (lhs.m_name == rhs.m_name);
    }
};


std::vector < int > GenerateNRandomNumbers(size_t N, int min, int max)
{
//  Generating random numbers without repeating:
    std::set < int > s {};
    std::vector < int > v {};
    std::uniform_int_distribution<int> distribution(min, max);
    std::random_device random_device;
    std::mt19937 engine {random_device()};
    int set_size = s.size(), x;
    while (set_size < N)
    {
        x = distribution(engine);
        s.insert(x);
        if (s.size() > set_size)
        {
            set_size = s.size();
            v.push_back(x);
        }
    }
    return v;
}

std::set < std::string > GenerateNRandomNames(size_t N,
                                               const std::vector < std::string > & alphabet,
                                               size_t name_length)
{
    std::set < std::string > s {};
    std::uniform_int_distribution<int> distribution(0, alphabet.size()-1);
    std::random_device random_device;
    std::mt19937 engine {random_device()};
    std::string name {};
    while (s.size() < N)
    {
        for (size_t i{}; i < name_length; ++i)
        {
            name += alphabet[distribution(engine)];
        }
        s.insert(name);
        name = "";
    }
    return s;
}

int main(int argc, char ** argv)
{
    /*
     * Чтобы построить график зависимости количества коллизий от заполненности хэш-таблицы,
     * будем генерировать случайные экзепляры класса Customer с помощью функций GenerateNRandomNames
     * и GenerateNRandomNumbers. GenerateNRandomNames возвращает set <string> с N неповторяющимися
     * рандомными именами длины name_length, GenerateNRandomNumbers возвращает vector <int> с N
     * неповторяющимися рандомными целыми числами в интервале от min до max.
     * Также строим гистограмму количества объектов в бакетах при полной нагрузке.
     * Все графики, как обычно, в statistics/plot.pdf
     */
    std::vector < int > marks {};
    std::set < std::string > names {};
    Customer_Hash hasher;
    const size_t hash_size = 1024;
    std::vector < int > hash_vec (hash_size, 0);
    size_t num_of_instances = 100;
    size_t num_collisions = 0;

    std::vector < std::string > alphabet {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                                   "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "};

    std::fstream f;
    f.open("../statistics/table.csv", std::ios::out);
    f << "number_of_elements,number_of_collisions" << "\n";
    while (num_of_instances < 2*hash_size)
    {
        marks = GenerateNRandomNumbers(num_of_instances, -10000, 10000);
        names = GenerateNRandomNames(num_of_instances, alphabet, 6);
        auto names_iter = names.begin();
        for (size_t i{}; i < 100; ++i)
        {
            Customer customer = Customer(*(names_iter++), marks[i]);
            hash_vec[hasher(customer) % hash_size] += 1;
        }
        num_collisions = 0;
        for (const auto & x: hash_vec)
        {
            num_collisions += ((x > 0) ? x-1 : 0);
        }
        f << static_cast < double >(num_of_instances) / hash_size << ',' << num_collisions << '\n';
        num_of_instances += 100;
    }
    std::fstream g;
    g.open("../statistics/hash_occupancy.csv", std::ios::out);
    for (const auto & x: hash_vec)
    {
        g << x << ',';
    }
    g << '\n';
    return 0;
}