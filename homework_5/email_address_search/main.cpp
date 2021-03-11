#include <iostream>
#include <regex>
#include <string>

int main() {
//    std::regex pattern(R"(([\-\w]+)@([\w\-]+\.[[:alpha:]]+[_\-.?[:alpha:]+]*))");
    std::regex pattern(R"((([\-\w]+\.)*[\-\w])+@(([[:alpha:]]+\.)+[[:alpha:]]{2,6}))");
//    std::regex pattern(R"(([\-\w]+)@(([[:alpha:]]+\.)+))");


    std::string input_str;
    std::getline(std::cin, input_str);
    std::sregex_iterator begin(input_str.cbegin(), input_str.cend(), pattern);
    std::sregex_iterator end;

    std::for_each(begin, end, [](const std::smatch &m) {

        std::cout << "e-mail: " << m[0] << ", domain name: " << m[3] << '\n';
    });

    system("pause");
    return EXIT_SUCCESS;
}

