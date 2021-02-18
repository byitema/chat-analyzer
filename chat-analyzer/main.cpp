#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <json.hpp>
#include <exception>

using json = nlohmann::json;

int main()
{	
	std::ifstream fin("chat.json");
	json chat_json = json::parse(fin);
	
	std::map<std::string, std::string> person_messages;
	std::string tmp_name;
	
	//parse json to the map (name, all messages string)
	auto &messages = chat_json["messages"];
	for (auto &message: messages)
	{
		try
		{
			tmp_name = message.at("from").get<std::string>();
			person_messages[tmp_name] += " ";
			person_messages[tmp_name] += message.at("text").get<std::string>();
		}
		catch (std::exception &ex)
		{
			std::cout << ex.what() << "\n";
		}	
	}

	//text to lower case
	for (auto it = person_messages.begin(); it != person_messages.end(); ++it)
	{
		std::for_each((it->second).begin(), (it->second).end(), [](char& c) {
			c = std::tolower(c);
			});
	}
	
	/* output messages data
	for (auto it = person_messages.cbegin(); it != person_messages.cend(); ++it)
	{
		std::cout << it->first << ": " << it->second << "\n\n\n";
	}
	*/
	
	return 0;
}