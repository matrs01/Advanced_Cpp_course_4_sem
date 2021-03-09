#include <iostream>
#include <regex>
#include <string>

int main() {
//    date and time format: YYYY.MM.DD and HH:MM:SS
    std::regex date_pattern(
            R"(([1-9][0-9]{3}|0[1-9][0-9]{2}|00[1-9][0-9]|000[1-9]).(0[0-9]|1[012]).([012][0-9]|3[01]))");
    std::regex time_pattern(R"(([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9]))");
    std::string input_str;
    std::getline(std::cin, input_str);
    std::sregex_iterator date_begin(input_str.cbegin(), input_str.cend(), date_pattern);
    std::sregex_iterator time_begin(input_str.cbegin(), input_str.cend(), time_pattern);
    std::sregex_iterator end;

    std::cout << "dates: ";
    std::for_each(date_begin, end, [](const std::smatch &m) {
        std::cout << m[0] << ' ';
    });
    std::cout << std::endl << "times: ";
    std::for_each(time_begin, end, [](const std::smatch &m) {
        std::cout << m[0] << ' ';
    });

    system("pause");
    return EXIT_SUCCESS;
}

