#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

std::string read_file(std::istream &in) {
    std::stringstream sstream;
    sstream << in.rdbuf();
    return sstream.str();
}

void save_file(std::ostream &out, const std::string &str) {
    std::stringstream sstream;
    sstream << str;
    out << sstream.rdbuf();
}

int main(int argc, char **agrv) {

    std::fstream file("../test.txt", std::ios::in);
    std::string file_in_string = read_file(file);
    file.close();

    enum {
        SECOND_SLASH,     // There was the second consecutive slash that means comment "//"
        OPEN_ASTERISK,    // There was "/*" construction so it is commented until */ or EOF
        OPEN_QUOTE        // There was an open quote meaning start of the string literal
    };


    std::vector<bool> flags(3, false);
    std::string::iterator string_it;

    for (auto it = file_in_string.begin(); it != file_in_string.end(); it++) {
        switch (*it) {
            case '/': {
                if (flags[OPEN_QUOTE] || flags[SECOND_SLASH] || flags[OPEN_ASTERISK]) {
                    break;
                }
                if (*(it - 1) == '/') {
                    flags[SECOND_SLASH] = true;
                    string_it = it - 1;
                    break;
                }
                break;
            }
            case '\n': {
                if (flags[SECOND_SLASH]) {
                    if (*(it - 1) == '\\') {
                        break;
                    }
                    it = file_in_string.erase(string_it, it);
                    flags[SECOND_SLASH] = false;
                    break;
                }
                break;
            }
            case '"': {
                if (flags[SECOND_SLASH] || flags[OPEN_ASTERISK]) {
                    break;
                }
                if (flags[OPEN_QUOTE]) {
                    if (*(it - 1) == '\\') {
                        if (*(it - 2) == '\\') {
                            flags[OPEN_QUOTE] = false;
                        }
                        break;
                    }
                    flags[OPEN_QUOTE] = false;
                    break;
                }
                flags[OPEN_QUOTE] = true;
                break;
            }
            case '*': {
                if (flags[SECOND_SLASH]) {
                    break;
                }
                if (!flags[OPEN_ASTERISK] && *(it - 1) == '/') {
                    flags[OPEN_ASTERISK] = true;
                    string_it = it - 1;
                    break;
                }
                if (flags[OPEN_ASTERISK]) {
                    if ((it + 1) != file_in_string.end() && *(it + 1) == '/') {
                        flags[OPEN_ASTERISK] = false;
                        it = file_in_string.erase(string_it, it + 2);
                        break;
                    }
                    break;
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    if (flags[OPEN_ASTERISK]) {
        file_in_string.erase(string_it, file_in_string.end());
    }

    file.open("../output.txt", std::ios::out);
    save_file(file, file_in_string);

    return EXIT_SUCCESS;
}