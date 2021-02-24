#pragma once
#include <string>
#include <vector>

using namespace std;

void thread_job(vector<pair<string, string>>& data, vector<int> indexes, vector<pair<string, vector<string>>>& output);

stringstream frequent_words(vector<pair<string, string>>& data, int thread_count, int n);

vector<pair<string, string>> parse_json(const string filename);