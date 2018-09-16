//#pragma once
//hallTools.h
//Author:Hanyuu
//Commit:
//	工具函数

#ifndef HALL_TOOLS
#define HALL_TOOLS

#include<string>
#include<sstream>
#include<fstream>
//#include<boost/shared_ptr.hpp>
//#include "chatroom_server.h"
//#include"chatroom_client.h"
//#include <boost/thread.hpp>

/*
Function name:int2String
Description:将int类型的变量转换为string类型
Created:18/08/24
Paremeter:int:待转换的int变量
Return code:string 返回string结果
Author:Hanyuu
*/
std::string int2String(int i);

/*
Function name:float2String
Description:将float类型的变量转换为string类型
Created:18/09/13
Paremeter:int:待转换的float变量
Return code:string 返回string结果
Author:Hanyuu
*/
std::string float2String(float i);

/*
Function name:string2Int
Description:将string类型的变量转换为int类型
Created:18/09/05
Paremeter:int:待转换的string变量
Return code:string 返回int结果
Author:Hanyuu
*/
int string2Int(std::string);

/*
Function name:log
Description:输出log到日志
Created:18/0826
Paremeter:String:要打印的log信息
Return code:VOID
Author:Hanyuu
*/
void hanyuuLog(std::string);

/*
Function:connectMessage
Description:信息收发
Created:18/08/29
Parameter:VOID
Return:VOID
Author:Hanyuu
*/
//void connectMessage();

/*
Function name:clientGetMessage
Description:监听信息
Created:18/08/29
Parameter:shared_ptr:调用对象的智能指针
Return:VOID
Author:Hanyuu
*/
//void clientGetMessage(boost::shared_ptr<ChatMessage> mp);

//
////UTF-8到GB2312的转换
//char* utf2GB(const char* utf8)
//{
//	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
//	wchar_t* wstr = new wchar_t[len + 1];
//	memset(wstr, 0, len + 1);
//	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
//	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
//	char* str = new char[len + 1];
//	memset(str, 0, len + 1);
//	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
//	if (wstr) delete[] wstr;
//	return str;
//}
//
////GB2312到UTF-8的转换
//char* GB2utf(const char* gb2312)
//{
//	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
//	wchar_t* wstr = new wchar_t[len + 1];
//	memset(wstr, 0, len + 1);
//	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
//	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
//	char* str = new char[len + 1];
//	memset(str, 0, len + 1);
//	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
//	if (wstr) delete[] wstr;
//	return str;
//}
#endif // !HALL_TOOLS
