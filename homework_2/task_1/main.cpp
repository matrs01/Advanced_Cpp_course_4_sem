#include <iostream>
#include <vector>
#include <fstream>
//#include "../../homework_1/Timer.h"

template <typename T>
void to_csv(T & v, const std::string & path) {
    std::fstream f;
    f.open(path, std::ofstream::out);
    for (auto i = 0U; i < v.size() - 1; ++i) {
        f << v[i] << ',';
    }
    f << v[v.size() - 1];
}

int main(){
//  Let's look at the memory allocation when adding one element at a time
    std::vector < int > v;
    std::vector < int > vector_capacity_1 (10000, 0);
    v.reserve(1);
    std::vector < int > vector_capacity_2 {};
    std::vector < double > mult_factor {};
    size_t prev_capacity = 0;

    std::cout << "Checking capacity increase via .puchback() method...\n";
    for (auto i = 0U; i < 10000; ++i) {
        v.push_back(0);
        vector_capacity_1[i] = v.capacity();
        if (v.capacity() > prev_capacity) {
            mult_factor.push_back(static_cast< double >(v.capacity()) / prev_capacity);
            prev_capacity = v.capacity();
            vector_capacity_2.push_back(prev_capacity);
        }
    }
    to_csv(vector_capacity_1, "../graphics/plot_1.csv");
//  we can see that capacity increases 1.5 times and only when capacity = size
//  Now we plot only points when capacity increases and try to allocate as
//  much memory as possible
    while (true) {
        try{
            v.push_back(0);
            if (v.capacity() > prev_capacity){
                mult_factor.push_back(static_cast< double >(v.capacity()) / prev_capacity);
                prev_capacity = v.capacity();
                vector_capacity_2.push_back(prev_capacity);
            }
        }
        catch (std::bad_alloc & msg){
            std::cout << "memory overflow: " << msg.what() << std::endl;
            std::cout << "max capacity: " << prev_capacity << std::endl;
            break;
        }
    }
    to_csv(vector_capacity_2, "../graphics/plot_2.csv");
    to_csv(mult_factor, "../graphics/plot_3.csv");
//  we can see that capacity increases 1.5 times till the memory overflow (bad_alloc)

//  consider .reserve() method
//  we will reserve more and more memory and check capacity
    std::vector < int > g {};
    std::vector < int > vector_capacity_3 {};
    size_t multiplier = 1;

    std::cout << "Checking capacity increase via .reserve() method...\n";

//  increasing reserved memory
    while (true) {
        try {
            g.reserve(multiplier);
            vector_capacity_3.push_back(g.capacity());
            multiplier *= 10;
        }
        catch (std::bad_alloc & msg){
            std::cout << "memory overflow: " << msg.what() << std::endl;
            std::cout << "max capacity: " << g.capacity() << std::endl;
            break;
        }
    }
//  decreasing reserved memory
    multiplier /= 10;
    while (true) {
        g.reserve(multiplier);
        vector_capacity_3.push_back(g.capacity());
        multiplier /= 10;
        if (multiplier <= 1){
            break;
        }
    }

    to_csv(vector_capacity_3, "../graphics/plot_4.csv");

//  when reserving more and more memory capacity is exactly the same as
//  reserve size, when decreasing reserve size capacity remains the same.

//  see results in graphics/plot.pdf
    return 0;
}
