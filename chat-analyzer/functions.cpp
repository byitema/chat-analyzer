#include <algorithm>
#include <ctime>
#include <exception>
#include <fstream>
#include <iostream>
#include <json.hpp>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include "functions.h"
#pragma warning (disable: 4996)

using json = nlohmann::json;
using namespace std;

void thread_job(vector<pair<string, string>>& data, vector<int> indexes, vector<pair<string, vector<string>>> &output)
{
    for (const auto index: indexes)
    {
        /*
        cout << this_thread::get_id() << "\n";
         */
        
        //divide by words
        output[index].first = data[index].first;
        
        char* str = new char[data[index].second.length() + 1];
        strcpy(str, data[index].second.c_str());
        
        char* p = strtok(str, " \'\"(),.!?;-\n\t");
        while (p != 0)
        {
            if (strlen(p) > 2)
            {
                output[index].second.push_back(string(p));
            }
            p = strtok(NULL, " \'\"(),.!?;-\n\t");
        }
        
        //counting frequency
        map<string, int> frequency;
        for (const auto& word: output[index].second)
        {
            frequency[word]++;
        }
        
        //sort output by frequency
        sort(output[index].second.begin(), output[index].second.end(),
            [&frequency](const string& a, const string& b) -> bool
            {
                return frequency[a] > frequency[b];
            });
        
        //delete repeats
        output[index].second.erase(unique(output[index].second.begin(), output[index].second.end()), output[index].second.end());
    }
}

stringstream frequent_words(vector<pair<string, string>>& data, int thread_count, int n)
{
    vector<pair<string, vector<string>>> output(data.size());
    
    //count slices and start threads
    vector<thread> threads;
    for (int i = 0; i < thread_count; i++) {
        vector<int> indexes;
        for (int j = 0; j < data.size(); j++)
        {
            if ((j % thread_count) == i)
            {
                indexes.push_back(j);
            }
        }
        threads.push_back(thread(thread_job, ref(data), indexes, ref(output)));
    }
    
    for (auto& t: threads) {
        t.join();
    }
    
    //output
    stringstream ss;
    for (const auto &pr: output)
    {
        size_t size = pr.second.size();
        ss << pr.first << ": ";
        for (int i = 0; i < n; i++)
        {
            if (i < size)
            {
                ss << pr.second[i] << " | ";
            }
            else
            {
                break;
            }
        }
        ss << "\n";
    }
    
    return ss;
}

vector<pair<string, string>> parse_json(const string filename)
{
    ifstream fin(filename);
    json chat_json = json::parse(fin);
    
    map<string, string> person_messages;
    string tmp_name;
    
    //parse json to the map (name, all messages string)
    const auto& messages = chat_json["messages"];
    for (const auto& message : messages)
    {
        try
        {
            tmp_name = message.at("from").get<string>();
            person_messages[tmp_name] += " ";
            person_messages[tmp_name] += message.at("text").get<string>();
        }
        catch (exception& ex)
        {
            cerr << ex.what() << "\n";
        }
    }
    
    //text to lower case
    vector<pair<std::string, string>> data(person_messages.size());
    int index = 0;
    for (auto it = person_messages.begin(); it != person_messages.end(); ++it)
    {
        for_each((it->second).begin(), (it->second).end(), [](char& c) {
            c = std::tolower(c);
            });
        data[index] = make_pair(it->first, it->second);
        index++;
    }
    
    return data;
}