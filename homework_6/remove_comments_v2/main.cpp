#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

void remove_comments (std::string & string)
{
    for (auto iterator = std::begin(string); iterator != std::end(string); ++iterator)
    {
        if (*iterator == '"')
        {
            iterator++;
            for (; *iterator != '"' || *std::prev(iterator) == '\\'; ++iterator);
        }
        if (*iterator == '/')
        {
            if (*std::next(iterator) == '/')
            {
                auto begin = iterator;
                for (; iterator != std::end(string) && *iterator != '\n'; ++iterator);
                iterator = string.erase(begin, iterator);
            }
            else if (*std::next(iterator) == '*')
            {
                auto begin = iterator;
                for (; iterator != std::end(string) &&
                                !(*iterator == '/' && *std::prev(iterator) == '*'); ++iterator);
                iterator = string.erase(begin, ++iterator);
            }
        }

        if (iterator == std::end(string))
        {
            break;
        }
    }
}

int main(int argc, char **agrv) {
    std::fstream file("../test.txt", std::ios::in);
    std::string file_in_string = read_file(file);
    file.close();

    remove_comments(file_in_string);

    file.open("../output.txt", std::ios::out);
    save_file(file, file_in_string);
    return EXIT_SUCCESS;
}