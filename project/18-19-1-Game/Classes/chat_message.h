#pragma once
#include<string>
using std::string;
struct ChatMessage
{
	int playerID;
	string message;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & message;
		ar & playerID;
	}
};