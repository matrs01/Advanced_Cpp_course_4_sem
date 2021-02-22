#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <iomanip>

using namespace boost::multi_index;

struct PhoneBook
{
    std::string name;
    std::string surname;
    std::string phone_number;
};

using phonebook_multi_index = multi_index_container <
        PhoneBook, indexed_by <
                hashed_non_unique <
                        member < PhoneBook, std::string, &PhoneBook::surname > > ,
                random_access <> ,
                ordered_non_unique <
                        member < PhoneBook, std::string, &PhoneBook::surname > > > > ;

int main() {
    /*
     * Демонстрационный код
     */
    phonebook_multi_index phone_book;
    phone_book.insert({"Ivan", "Ivanov", "+7(916)123-45-56"});
    phone_book.insert({"Andrew", "Andreev", "+7(915)888-75-56"});
    phone_book.insert({"Nikolay", "Ivanov", "8(800)555-35-35"});
    phone_book.insert({"Michail", "Pavlov", "+7(915)764-23-21"});
    phone_book.insert({"Alexander", "Izyurov", "+7(916)534-99-01"});

//  Для городской типографии:
    std::cout << "For City Printing House:\n";
    auto & ordered_phone_book = phone_book.get < 0 >();

    auto begin = ordered_phone_book.begin();
    auto end = ordered_phone_book.end();

    for (; begin != end; begin++)
    {
        std::cout << std::left << std::setw(15) << begin->surname << ' '<<
        std::left << std::setw(15) << begin->name << ' '<<
        std::left << std::setw(15) << begin->phone_number << '\n';
    }
//  Для рекламного агентства:
    std::cout << "\nFor advertising agency:\n";
    auto & random_access_phone_book = phone_book.get < 1 >();
    std::cout << "index=1: " << std::left << std::setw(10) << random_access_phone_book[1].surname << ' '<<
                               std::left << std::setw(10) << random_access_phone_book[1].name << ' '<<
                               std::left << std::setw(10) << random_access_phone_book[1].phone_number << '\n';
    std::cout << "index=3: " << std::left << std::setw(10) << random_access_phone_book[3].surname << ' '<<
              std::left << std::setw(10) << random_access_phone_book[3].name << ' '<<
              std::left << std::setw(10) << random_access_phone_book[3].phone_number << '\n';
//  Для регулярного пользователя:
    std::cout << "\nFor regular user:\n";
    auto & hashed_phone_book = phone_book.get < 2 >();
    std::cout << "surname=Pavlov: " << std::right << std::setw(10) << hashed_phone_book.find("Pavlov")->surname << ' '<<
                                    std::right << std::setw(10) << hashed_phone_book.find("Pavlov")->name << ' '<<
                                    std::right << std::setw(10) << hashed_phone_book.find("Pavlov")->phone_number << '\n';
    std::cout << "surname=Izyurov: " << std::right << std::setw(10) << hashed_phone_book.find("Izyurov")->surname << ' '<<
              std::right << std::setw(10) << hashed_phone_book.find("Izyurov")->name << ' '<<
              std::right << std::setw(10) << hashed_phone_book.find("Izyurov")->phone_number << '\n';
    return 0;
}