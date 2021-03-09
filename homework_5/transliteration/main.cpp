#include <bitset>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>

#include <boost/locale.hpp>

#include <Windows.h>

std::string convert_locale_to_utf(const std::string &string) {
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::to_utf<char>(string, locale);
}

std::string convert_utf_to_locale(const std::string &string) {
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
            generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::from_utf<char>(string, locale);
}

int main() {
//    system("chcp 1251");
    const std::unordered_map<char32_t, std::u32string> trans{
            {U'\U00000430', U"a"},
            {U'\U00000431', U"b"},
            {U'\U00000432', U"v"},
            {U'\U00000433', U"g"},
            {U'\U00000434', U"d"},
            {U'\U00000435', U"e"},
            {U'\U00000451', U"yo"},
            {U'\U00000436', U"zh"},
            {U'\U00000437', U"z"},
            {U'\U00000438', U"i"},
            {U'\U00000439', U"j"},
            {U'\U0000043a', U"k"},
            {U'\U0000043b', U"l"},
            {U'\U0000043c', U"m"},
            {U'\U0000043d', U"n"},
            {U'\U0000043e', U"o"},
            {U'\U0000043f', U"p"},
            {U'\U00000440', U"r"},
            {U'\U00000441', U"s"},
            {U'\U00000442', U"t"},
            {U'\U00000443', U"u"},
            {U'\U00000444', U"f"},
            {U'\U00000445', U"h"},
            {U'\U00000446', U"c"},
            {U'\U00000447', U"ch"},
            {U'\U00000448', U"sh"},
            {U'\U00000449', U"shch"},
            {U'\U0000044a', U"\""},
            {U'\U0000044b', U"y"},
            {U'\U0000044c', U"\'"},
            {U'\U0000044d', U"e"},
            {U'\U0000044e', U"yu"},
            {U'\U0000044f', U"ya"},
            {U'\U00000410', U"A"},
            {U'\U00000411', U"B"},
            {U'\U00000412', U"V"},
            {U'\U00000413', U"G"},
            {U'\U00000414', U"D"},
            {U'\U00000415', U"E"},
            {U'\U00000401', U"Yo"},
            {U'\U00000416', U"Zh"},
            {U'\U00000417', U"Z"},
            {U'\U00000418', U"I"},
            {U'\U00000419', U"J"},
            {U'\U0000041a', U"K"},
            {U'\U0000041b', U"L"},
            {U'\U0000041c', U"M"},
            {U'\U0000041d', U"N"},
            {U'\U0000041e', U"O"},
            {U'\U0000041f', U"P"},
            {U'\U00000420', U"R"},
            {U'\U00000421', U"S"},
            {U'\U00000422', U"T"},
            {U'\U00000423', U"U"},
            {U'\U00000424', U"F"},
            {U'\U00000425', U"H"},
            {U'\U00000426', U"C"},
            {U'\U00000427', U"Ch"},
            {U'\U00000428', U"Sh"},
            {U'\U00000429', U"Shch"},
            {U'\U0000042a', U"\""},
            {U'\U0000042b', U"Y"},
            {U'\U0000042c', U"\'"},
            {U'\U0000042d', U"E"},
            {U'\U0000042e', U"Yu"},
            {U'\U0000042f', U"Ya"},
    };
//// for console input:
//    SetConsoleCP      (1251);
//    SetConsoleOutputCP(1251);
//    std::string string;
//    std::getline(std::cin, string);
//    std::string u8string = convert_locale_to_utf(string);
//    std::ifstream file;

//// for file input:
    std::ifstream file;
    file.open("../input.txt");
    std::stringstream strStream;
    strStream << file.rdbuf();
    std::string u8string = strStream.str();
////---------
    std::u32string u32string =
            boost::locale::conv::utf_to_utf<char32_t, char>(u8string);
    std::u32string new_u32string{};
    for (auto &c: u32string) {
        auto pointer = trans.find(c);
        if (pointer != trans.end()) {
            new_u32string.append(pointer->second);
        } else {
            new_u32string += c;
        }
    }
    u8string = boost::locale::conv::utf_to_utf<char, char32_t>(new_u32string);
    std::string string_1 = convert_utf_to_locale(u8string);
    std::cout << string_1 << '\n';

    system("pause");
    return EXIT_SUCCESS;
}
