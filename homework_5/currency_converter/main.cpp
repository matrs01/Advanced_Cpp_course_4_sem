#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>

#include <Windows.h>

std::string show_currency_symbol(const std::string &locale_name) {
    return std::use_facet<std::moneypunct<char, false> >(
            std::locale(locale_name)).curr_symbol();
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "Converter " << show_currency_symbol("eu_DE.utf8") << " -> " <<
              show_currency_symbol("ru_RU.utf8") << std::endl;
    const double eur_to_rub = 88.57;
    long double value;
    std::cout.imbue(std::locale("ru_RU.utf8"));
    std::cin.imbue(std::locale("eu_DE.utf8"));
    std::cin >> std::get_money(value, false);
    std::cout << std::showbase << std::put_money(value * eur_to_rub, false) << std::endl;
}

