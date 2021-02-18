#pragma once
#include <string>
#include <vector>

void thread_job(std::vector<std::pair<std::string, std::string>>& data, std::vector<int> indexes, std::vector<std::pair<std::string, std::vector<std::string>>>& output);

std::stringstream frequent_words(std::vector<std::pair<std::string, std::string>>& data, int thread_count, int n);

std::vector<std::pair<std::string, std::string>> parse_json(const std::string filename);