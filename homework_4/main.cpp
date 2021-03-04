#include <iostream>
#include <random>
#include <vector>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>
#include <fstream>

void print(const std::vector < int > & vec, std::fstream & file)
{
    std::copy(std::cbegin(vec),
              std::cend(vec),
              std::ostream_iterator<int>(file, " "));
    file << '\n';
}

int main(){
    /*
     * Вывод в данной программе происхдит в файл output.txt, так как в Clion большие
     * проблемы с выводом русскоязычных символов в консоль (я так и не смог починить).
     */
    std::fstream file;
    file.open("../output.txt", std::ios::out);
//    task 1:
    file << "Создаем последовательность p1 чисел от 1 до 10.\n";
    const size_t size_p1 = 10;
    std::vector < int > p1 (size_p1);
    std::iota(p1.begin(), p1.end(), 1);
    file << "p1: ";
    print(p1, file);

//    task 2:
    file << "Добавим в p1 несколько чисел из cin (чтобы остановить ввод, введите не int).\n";
    std::copy(
            std::istream_iterator<int>(std::cin),
            std::istream_iterator<int>(),
            std::back_inserter(p1));
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file << "p1: ";
    print(p1, file);

//    task 3:
    file << "Перемешаем p1 случайным образом.\n";
    std::random_device random_device{};
    std::mt19937 engine(random_device());
    std::shuffle(std::begin(p1), std::end(p1), engine);
    file << "p1: ";
    print(p1, file);

//    task 4:
    file << "Удалим дубликаты из p1.\n";
    std::sort(p1.begin(), p1.end());
    p1.erase(std::unique(std::begin(p1), std::end(p1)), std::end(p1));
    file << "p1: ";
    print(p1, file);

//    task 5:
    file << "Подсчитаем количество нечетных чисел в p1.\n";
    file << std::count_if(std::cbegin(p1), std::cend(p1), [](auto x) { return (std::abs(x) % 2 == 1); }) << '\n';

//    task 6:
    file << "Определим минимальный и максимальный элеиент в p1.\n";
    const auto[min, max] = std::minmax_element(std::cbegin(p1), std::cend(p1));
    file << "min value: " << *min << "; max value: " << *max << '\n';

//    task 7:
    file << "Найдем первое простое число в p1.\n";
    auto prime = std::find_if(std::cbegin(p1), std::cend(p1), [](auto x) {
        if (x == 2) {
            return true;
        } else if (x < 2 || x % 2 == 0) {
            return false;
        } else {
            for (size_t i{3}; i < static_cast<int>(std::sqrt(x)) + 1; i += 2) {
                if (x % i == 0) {
                    return false;
                }
            }
            return true;
        }
    });
    if (prime != std::end(p1)) {
        file << *prime << '\n';
    } else {
        file << "Prime number not found.\n";
    }

//    task 8:
    file << "Заменим все числа в p1 их квалратами.\n";
    std::transform(std::begin(p1), std::end(p1), std::begin(p1), [](auto x) { return x * x; });
    print(p1, file);

//    task 9:
    file << "Создадим последовательность p2 из N случайных чисел, где N - длина p1.\n";
    std::uniform_int_distribution<int> generator(-100, 100);
    std::vector<int> p2(p1.size());
    std::generate(std::begin(p2), std::end(p2), [&]() { return generator(engine); });
    file << "p2: ";
    print(p2, file);

//    task 10:
    file << "Вычислим сумму чисел в p2.\n";
    file << "p2 sum: " << std::accumulate(std::cbegin(p2), std::cend(p2), 0) << '\n';

//    task 11:
    file << "Заменим первые 3 числа в p2 единицей.\n";
    std::for_each_n(std::begin(p2), 3, [](auto & x){x = 1;});
    file << "p2: ";
    print(p2, file);

//    task 12:
    file << "Создадим последовательномть p3 как разность p1 и p2.\n";
    std::vector<int> p3;
    std::transform(std::cbegin(p1), std::cend(p1), std::cbegin(p2),
                   std::back_inserter(p3), std::minus<>());
    file << "p1: ";
    print(p1, file);
    file << "p2: ";
    print(p2, file);
    file << "p3: ";
    print(p3, file);

//    task 13:
    file << "Заменим каждый отрицательный элемент в p3 нулем.\n";
    std::replace_if(std::begin(p3), std::end(p3), [](auto x){return (x < 0);}, 0);
    file << "p3: ";
    print(p3, file);

//    task 14:
    file << "Удалим все нулевые элементы из p3.\n";
    p3.erase(std::remove(std::begin(p3), std::end(p3), 0), std::end(p3));
    file << "p3: ";
    print(p3, file);

//    task 15:
    file << "Изменим порядок следования элементов в p3 на обратный.\n";
    std::reverse(std::begin(p3), std::end(p3));
    file << "p3: ";
    print(p3, file);

//    task 16:
    file << "Определим быстро топ-3 наибольших элемента в p3.\n";
    std::nth_element(std::begin(p3), std::prev(std::end(p3), 3), std::end(p3));
    std::copy(std::prev(std::end(p3), 3), std::end(p3), std::ostream_iterator<int>(file, " "));
    file << '\n';
//    task 17:
    file << "Отсортируем полностью p1 и p2 по возрастанию.\n";
    std::sort(std::begin(p1), std::end(p1));
    std::sort(std::begin(p2), std::end(p2));
    file << "p1: ";
    print(p1, file);
    file << "p2: ";
    print(p2, file);

//    task 18:
    file << "Создадим последовательность p4 как слияние p1 и p2.\n";
    std::vector < int > p4;
    std::merge(std::cbegin(p1), std::cend(p1),
               std::cbegin(p2), std::cend(p2),
               std::back_inserter(p4));
    file << "p4: ";
    print(p4, file);

//    task 19:
    file << "Определим диапазон для упорядоченной вставки числа 1 в p4\n";
    auto [lower_bound, upper_bound] = std::equal_range(std::cbegin(p4), std::cend(p4), 1);
    file << "[lower_bound, upper_bound] = [" << std::distance(std::cbegin(p4), lower_bound) <<
    ", " << std::distance(std::cbegin(p4), upper_bound) << "]\n";

//    task 20:
    file << "Выведем все последовательности в cout.\n";
    file << "p1: ";
    print(p1, file);
    file << "p2: ";
    print(p2, file);
    file << "p4: ";
    print(p3, file);

//    task 21:
    file << "Определим количество сравнений в алгоритмах std::sort и std::nth_element.\n";
    std::vector < int > p5 (300);
    std::iota(std::begin(p5), std::end(p5), -150);
    std::shuffle(std::begin(p5), std::end(p5), engine);
    std::fstream f;
    f.open("../task_21.csv", std::ios::out);
    f << "std::sort,std::nth_element\n";
    size_t counter;
    std::vector < int > temp_vec;
    for (auto i = 1U; i <= 30; ++i)
    {
        temp_vec.resize(i*10);
        counter = 0;
        std::copy(std::cbegin(p5), std::next(std::cbegin(p5), i*10), std::begin(temp_vec));
        std::sort(std::begin(temp_vec), std::end(temp_vec), [&counter](auto x, auto y){
            counter++;
            return std::less<int>()(x, y);
        });
        f << counter << ',';
        counter = 0;
        std::copy(std::cbegin(p5), std::next(std::cbegin(p5), i*10), std::begin(temp_vec));
        std::nth_element(std::begin(temp_vec), std::next(std::begin(temp_vec), i*3),
                         std::end(temp_vec), [&counter](auto x, auto y){
            counter++;
            return std::less<int>()(x, y);
        });
        f << counter << '\n';
    }
    /*
     * График сравнения std::sort и std::nth_element можно посмотреть в plot.pdf.
     * График сильно скачет, так как мы не делали усреднения по нескольким разным
     * последовательностям, но качественно видим, что примерно до длины 30 количество
     * сравнений одинаково, а затем начинает отличаться. Это объясняется тем, что
     * до std::_ISORT_MAX = 32 элементов std::nth_element просто делает полную сортировку.
     */
//    std::cout << std::_ISORT_MAX;
    return 0;
}

