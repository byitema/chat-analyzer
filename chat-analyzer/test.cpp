#include <ctime>
#include <iostream>
#include <json.hpp>
#include <sstream>
#include <string>
#include <thread>
#include "functions.h"
#pragma warning (disable: 4996)

using namespace std;

stringstream main_run(int thread_count, int n, const string filename)
{
    stringstream ss;
    ss << "Top " + to_string(n) + " words...\n\n";
    
    vector<pair<string, string>> data = parse_json(filename);
    ss << frequent_words(data, thread_count, n).str();

    return ss;
}

stringstream time_test(int thread_count, int n, vector<pair<string, string>>& data)
{
    stringstream ss;

    ss << "Test with " + to_string(thread_count) + " thread is running...\n";
    unsigned int start_time = clock();
    frequent_words(data, thread_count, n).str();
    unsigned int working_time = clock() - start_time;
    ss << "Working time: " << working_time << ".\n";
    
    return ss;
}

stringstream time_tests(string filename)
{
    vector<pair<string, string>> data = parse_json(filename);
    stringstream ss;
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

bool thread_job_test()
{
    vector<pair<string, string>> data;
    data.push_back(make_pair("Artem", " second, user"));
    data.push_back(make_pair("Danik", " hello; from.me! more"));
    vector<pair<string, vector<string>>> output(2);
    vector<int> indexes = { 0, 1 };

    vector<pair<string, vector<string>>> wanted_output(2);
    vector<string> v1 = {"second", "user"};
    vector<string> v2 = { "hello", "from", "more" };
    wanted_output[0] = make_pair("Artem", v1);
    wanted_output[1] = make_pair("Danik", v2);

    thread_job(data, indexes, output);

    if (wanted_output == output)
    {
        return true;
    }

    return false;
}

bool frequent_words_test()
{
    vector<pair<string, string>> data;
    data.push_back(make_pair("Artem", " second, user, second, user, user"));
    data.push_back(make_pair("Danik", " hello; from.me! more"));

    string s1 = "Artem: user | second | \nDanik: hello | from | more | \n";
    string s2 = frequent_words(data, 2, 3).str();

    if (s1 == s2)
    {
        return true;
    }

    return false;
}

bool parse_json_test()
{
    auto v1 = parse_json("parsetest.json");

    vector<pair<string, string>> v2;
    v2.push_back(make_pair("Artem", " second user"));
    v2.push_back(make_pair("Danik", " hello from me"));

    if (v1 == v2)
    {
        return true;
    }

    return false;
}

int main()
{
    cout << main_run(2, 3, "chat.json").str();
    cout << "\n\n";
    cout << time_tests("chat.json").str();

    cout << "\nparse_json test...\n";
    if (parse_json_test())
    {
        cout << "Correct!\n";
    }
    else
    {
        cout << "Wrong!\n";
    }

    cout << "\nthread_job test...\n";
    if (thread_job_test())
    {
        cout << "Correct!\n";
    }
    else
    {
        cout << "Wrong!\n";
    }

    cout << "\nfrequent_words test...\n";
    if (frequent_words_test())
    {
        cout << "Correct!\n";
    }
    else
    {
        cout << "Wrong!\n";
    }

    return 0;
}