#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <json.hpp>
#include <exception>
#include <thread>
#include <algorithm>
#include <ctime>
#pragma warning (disable: 4996)

using json = nlohmann::json;

void thread_job(std::vector<std::pair<std::string, std::string>>& data, std::vector<int> indexes, std::vector<std::pair<std::string, std::vector<std::string>>> &output)
{
	for (int index: indexes)
	{
		/*
		std::cout << std::this_thread::get_id() << "\n";
		 */
		
		//divide by words
		output[index].first = data[index].first;
		
		char* str = new char[data[index].second.length() + 1];
		std::strcpy(str, data[index].second.c_str());

		char* p = std::strtok(str, " \'\"(),.!?-\n\t");
		while (p != 0)
		{
			if (strlen(p) > 2)
			{
				output[index].second.push_back(std::string(p));
			}
			p = std::strtok(NULL, " \'\"(),.!?;-\n\t");
		}

		//counting frequency
		std::map<std::string, int> frequency;
		for (std::string word: output[index].second)
		{
			frequency[word]++;
		}

		//sort output by frequency
		std::sort(output[index].second.begin(), output[index].second.end(),
			[&frequency](const std::string& a, const std::string& b) -> bool
			{
				return frequency[a] > frequency[b];
			});

		//delete repeats
		output[index].second.erase(std::unique(output[index].second.begin(), output[index].second.end()), output[index].second.end());
	}
}

std::stringstream frequent_words(std::vector<std::pair<std::string, std::string>>& data, int thread_count, int n)
{
	std::vector<std::pair<std::string, std::vector<std::string>>> output(data.size());

	//count slices and start threads
	std::vector<std::thread> threads;
	for (int i = 0; i < thread_count; i++) {
		std::vector<int> indexes;
		for (int j = 0; j < data.size(); j++)
		{
			if ((j % thread_count) == i)
			{
				indexes.push_back(j);
			}
		}
		threads.push_back(std::thread(thread_job, std::ref(data), indexes, std::ref(output)));
	}
	
	for (std::thread& t: threads) {
		t.join();
	}

	//output
	std::stringstream ss;
	for (auto &pr: output)
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

std::vector<std::pair<std::string, std::string>> parse_json(const std::string filename)
{
	std::ifstream fin(filename);
	json chat_json = json::parse(fin);

	std::map<std::string, std::string> person_messages;
	std::string tmp_name;
	
	//parse json to the map (name, all messages string)
	auto& messages = chat_json["messages"];
	for (auto& message : messages)
	{
		try
		{
			tmp_name = message.at("from").get<std::string>();
			person_messages[tmp_name] += " ";
			person_messages[tmp_name] += message.at("text").get<std::string>();
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << "\n";
		}
	}

	//text to lower case
	std::vector<std::pair<std::string, std::string>> data(person_messages.size());
	int index = 0;
	for (auto it = person_messages.begin(); it != person_messages.end(); ++it)
	{
		std::for_each((it->second).begin(), (it->second).end(), [](char& c) {
			c = std::tolower(c);
			});
		data[index] = std::make_pair(it->first, it->second);
		index++;
	}

	return data;
}

std::stringstream main_run(int thread_count, int n, const std::string filename)
{
	std::stringstream ss;
	ss << "Top " + std::to_string(n) + " words...\n\n";
	
	std::vector<std::pair<std::string, std::string>> data = parse_json(filename);
	ss << frequent_words(data, thread_count, n).str();

	return ss;
}

std::stringstream time_test(int thread_count, int n, std::vector<std::pair<std::string, std::string>> &data)
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