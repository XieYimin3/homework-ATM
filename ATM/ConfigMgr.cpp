#include "ConfigMgr.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
using namespace std;

ConfigMgr::ConfigMgr(string path)
{
    this->path = path;
    if (!filesystem::exists(path))
    {
        //父目录
        filesystem::path path_obj(this->path);
        filesystem::path dir_obj = path_obj.parent_path();
        filesystem::create_directories(dir_obj);
        //创建文件
        ofstream myStream(this->path);
        myStream.close();
    }
    //先初始一下内存中的配置
    this->update();
}
ConfigMgr::~ConfigMgr()
{
    
}

bool ConfigMgr::set(string section, string key, string value)
{
    // 如果 section 不存在，创建新的 section
    if (cfgData.find(section) == cfgData.end()) {
        cfgData[section] = map<string, string>();
    }

    // 更新或添加 key-value 配置
    cfgData[section][key] = value;

    // 将配置写回到文件
    return writeToFile();

}

bool ConfigMgr::writeToFile()
{
    // 前提是文件存在
    if (filesystem::exists(path)) 
    {
        ofstream myStream(path);
        for (const auto& section : cfgData) 
        {
            // 写section
            myStream << "[" << section.first << "]\n";
            for (const auto& kv : section.second) 
            {
                myStream << kv.first << "=" << kv.second << "\n";
            }
        }
        myStream.close();
        return true;
    }
    return false;
}

bool ConfigMgr::update()
{
    ifstream inFile(path);
    if (!inFile.is_open()) {
        cerr << "无法打开文件 " << path << " 进行读取" << endl;
        return false;
    }

    string line;
    // 当前的 section 名称
    string currentSection;
    while (getline(inFile, line))
    {
        // 去掉行首尾的空白字符
        line = line.substr(line.find_first_not_of(" \t"));
        line = line.substr(0, line.find_last_not_of(" \t") + 1);
        // 如果是空行或注释行，跳过
        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            continue;
        }
        // 如果是section
        if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            // 提取section名称
            currentSection = line.substr(1, line.size() - 2);
            // 如果该 section 不存在于 configData 中，初始化一个空 map
            if (cfgData.find(currentSection) == cfgData.end())
            {
                cfgData[currentSection] = map<string, string>();
            }
        }
        //如果是键值对
        else
        {
            size_t pos = line.find('=');
            if (pos != string::npos)
            {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                //去掉键和值两端的空白字符
                key = key.substr(key.find_first_not_of(" \t"));
                key = key.substr(0, key.find_last_not_of(" \t") + 1);
                value = value.substr(value.find_first_not_of(" \t"));
                value = value.substr(0, value.find_last_not_of(" \t") + 1);
                // 存储配置项
                if (!currentSection.empty()) {
                    cfgData[currentSection][key] = value;
                }
            }
        }
    }
    inFile.close();
    return true;
}