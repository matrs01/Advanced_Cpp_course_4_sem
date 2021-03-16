#include <iostream>
#include <filesystem>
#include <fstream>

#include "json.hpp"

using nlohmann::json;


struct Person {
    std::string surname{};
    std::string name{};
    int age{};
    int height{};
    std::string phone_number{};
    std::string address{};
};

std::istream &operator>>(std::istream &in, Person &p) {
    in >> p.name
       >> p.surname
       >> p.age
       >> p.phone_number
       >> p.height;

    return in;
}


int main(int argc, char **agrv) {
    const std::filesystem::path save_path = "../people";
    std::filesystem::create_directory(save_path);

    std::ifstream input_file("../person_info.txt", std::ios::in);

    Person person;
    while (input_file >> person) {
        json j;

        j["name"] = person.name;
        j["surname"] = person.surname;
        j["age"] = person.age;
        j["height"] = person.height;
        j["phone_number"] = person.phone_number;

        std::string save_name = person.surname + "_" + person.name;
        std::ofstream output_file("../people/" + save_name + ".json", std::ios::out);
        output_file << j.dump(2);
    }

    return EXIT_SUCCESS;
}

