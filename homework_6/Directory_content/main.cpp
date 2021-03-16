#include <iostream>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <utility>

namespace fs = std::filesystem;

std::string cut_path(const std::string &str, size_t width, bool add_dots = true) {
    if (str.length() > width) {
        if (add_dots) {
            return str.substr(0, width - 3) + "...";
        } else {
            return str.substr(0, width);
        }
    }

    return str;
}


int main(int argc, char **agrv) {
    fs::path user_path;
    std::cin >> user_path;
    if (!std::filesystem::exists(user_path) || !std::filesystem::is_directory(user_path)) {
        throw std::exception("Directory doesn't exist.");
    }
    const std::vector<size_t> widths = {40, 20, 10};

    std::cout << "Contents of \"" << fs::absolute(user_path).generic_string() << "\":\n";
    std::cout << std::left << std::setw(widths[0]) << "Path"
              << ' ' << std::left << std::setw(widths[1]) << "Name"
              << ' ' << std::left << std::setw(widths[2]) << "Extension"
              << ' ' << std::left << "Last modified\n";
    if (std::filesystem::exists(user_path) && std::filesystem::is_directory(user_path)) {
        for (auto const &cur_file : std::filesystem::recursive_directory_iterator(user_path)) {
            if (std::filesystem::is_regular_file(cur_file.status()) || std::filesystem::is_symlink(cur_file.status())) {
                const auto &path = cur_file.path();
                try {

                    std::time_t t = (fs::last_write_time(path).time_since_epoch().count() -
                                     __std_fs_file_time_epoch_adjustment) / 10000000;

                    std::cout << std::left << std::setw(widths[0]) << cut_path(
                            fs::relative(path, user_path).generic_string(), widths[0])
                              << ' ' << std::left << std::setw(widths[1]) << cut_path(path.stem().generic_string(),
                                                                                      widths[1])
                              << ' ' << std::left << std::setw(widths[2]) << cut_path(path.extension().generic_string(),
                                                                                      widths[2])
                              << ' ' << std::left << std::put_time(localtime(&t), "%d.%m.%Y %T") << '\n';
                } catch (std::exception &msg) {
                    std::cout << std::left << std::setw(widths[0]) << "???"
                              << ' ' << std::left << std::setw(widths[1]) << "???"
                              << ' ' << std::left << std::setw(widths[2]) << "???"
                              << ' ' << std::left << "???\n";

                    std::cerr << msg.what() << '\n';
                }
            }
        }
    }

    system("pause");

    return EXIT_SUCCESS;
}