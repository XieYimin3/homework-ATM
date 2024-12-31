#pragma once
#include <iostream>
#include <map>
using namespace std;

class ConfigMgr
{
public:
	ConfigMgr(string path);
	~ConfigMgr();
    //设置配置
    bool set(string section, string key, string value);
    //公开内存里面的配置？
    map<string, map<string, string>> cfgData;
private:
    //配置文件路径
    string path;
    //将内存里面的配置写入磁盘
    bool writeToFile(); 
    //从磁盘读配置，已更新内存中的配置
    bool update();
};
