//hallTools.cpp
//Author:Hanyuu
//Commit:
//	工具函数

#include"hallTools.h"
#define DEBUG

std::string int2String(int i)
{
	//int转string
	std::stringstream ssConv;
	std::string strNo = "";
	ssConv << i;
	ssConv >> strNo;
	return strNo;
}

std::string float2String(float i)
{
	//int转string
	std::stringstream ssConv;
	std::string strNo = "";
	ssConv << i;
	ssConv >> strNo;
	return strNo;
}

int string2Int(std::string str)
{
	int res;
	std::stringstream stream(str);
	stream >> res;
	return res;
}

void hanyuuLog(std::string logInf)
{
#ifdef DEBUG
	std::fstream debugLog("hanyuu.log", std::fstream::app);
	debugLog << logInf << "\n";
	debugLog.close();
#endif // DEBUG
}
