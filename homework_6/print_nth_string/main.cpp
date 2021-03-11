#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

void make_input_file(const std::filesystem::path &path, int size) {
    std::fstream file(path, std::ios::out);
    const size_t size_len = std::to_string(size).size();
    for (auto i = 0U; i < size; ++i) {
        file << "This is " << std::setw(size_len) << std::setfill('0') << i << " string in a file\n";
    }
}

int main() {
    const std::filesystem::path path = "../input.txt";
    make_input_file(path, 459);

    std::fstream file(path, std::ios::in | std::ios::binary);
    std::string line;
    std::getline(file, line);
    const size_t line_len = line.size() + 1;
    size_t num;
    while (std::cin) {
        std::cout << "Enter number of string:\n";
        std::cin >> num;
        file.seekg(num * line_len);
        std::getline(file, line);
        std::cout << line << std::endl << "--------\n";
    }

    return EXIT_SUCCESS;
}
