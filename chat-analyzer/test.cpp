#include <ctime>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <string>
#include <thread>
#include "functions.h"
#pragma warning (disable: 4996)

std::stringstream main_run(int thread_count, int n, const std::string filename)
{
    std::stringstream ss;
    ss << "Top " + std::to_string(n) + " words...\n\n";
    
    std::vector<std::pair<std::string, std::string>> data = parse_json(filename);
    ss << frequent_words(data, thread_count, n).str();

    return ss;
}

std::stringstream time_test(int thread_count, int n, std::vector<std::pair<std::string, std::string>>& data)
{
    std::stringstream ss;

    ss << "Test with " + std::to_string(thread_count) + " thread is running...\n";
    unsigned int start_time = clock();
    frequent_words(data, thread_count, n).str();
    unsigned int working_time = clock() - start_time;
    ss << "Working time: " << working_time << ".\n";
    
    return ss;
}

std::stringstream time_tests(std::string filename)
{
    std::vector<std::pair<std::string, std::string>> data = parse_json(filename);
    std::stringstream ss;
    ss << "Time tests...\n\n";

    //1 thread
    ss << time_test(1, 3, data).str();
    
    //2 thread
    ss << time_test(2, 3, data).str();
    
    //3 thread
    ss << time_test(3, 3, data).str();
    
    //6 thread
    ss << time_test(6, 3, data).str();
    
    return ss;
}

int main()
{
    std::cout << main_run(2, 3, "chat.json").str();
    std::cout << "\n\n";
    std::cout << time_tests("chat.json").str();

    return 0;
}