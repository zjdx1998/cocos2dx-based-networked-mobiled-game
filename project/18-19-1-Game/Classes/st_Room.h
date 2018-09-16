#pragma once
#include<string>
#include<sstream>
#include<boost/smart_ptr.hpp>
#include<vector>
using std::string;
typedef boost::shared_array<char> string_type;
struct st_room
{
	string ipv4;
	int playernum;
	std::vector<std::string> nameList;
};