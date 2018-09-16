#include "CommonSetting.h"
/*
    Description : 用于一些全局变量设置 (to set the global variables and settings)
    Author : 张建东(Zhang Jiandong)
    Date : 2018/8/20
*/

CommonSetting* CommonSetting::instance = nullptr;

const std::string CommonSetting::Direction[8] = { "West", "NorthWest", "North", "NorthEast",
                                                  "East", "SouthEast", "South", "SouthWest" };
const std::string CommonSetting::bossDirection[8] = { "East","North","NorthEast","WestEast",
                                                      "South","SouthEast","WestEast","West" };

CommonSetting::CommonSetting() {
    while (!mHashId.empty())  mHashId.erase(mHashId.begin());
}

CommonSetting::~CommonSetting() {

}

void CommonSetting::setNetworkCnt(const int&x)
{
    m_iNetworkCnt = x;
}

int CommonSetting::getNetworkID() { return m_iNetworkID; }
int CommonSetting::getNetworkCnt() { return m_iNetworkCnt; }
void CommonSetting::setNetworkID(const int & x) { m_iNetworkID = x; }

CommonSetting* CommonSetting::getInstance() {
    if (nullptr == instance) instance = new CommonSetting();
    return instance;
}