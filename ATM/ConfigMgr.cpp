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
        //��Ŀ¼
        filesystem::path path_obj(this->path);
        filesystem::path dir_obj = path_obj.parent_path();
        filesystem::create_directories(dir_obj);
        //�����ļ�
        ofstream myStream(this->path);
        myStream.close();
    }
    //�ȳ�ʼһ���ڴ��е�����
    this->update();
}
ConfigMgr::~ConfigMgr()
{
    
}

bool ConfigMgr::set(string section, string key, string value)
{
    // ��� section �����ڣ������µ� section
    if (cfgData.find(section) == cfgData.end()) {
        cfgData[section] = map<string, string>();
    }

    // ���»���� key-value ����
    cfgData[section][key] = value;

    // ������д�ص��ļ�
    return writeToFile();

}

bool ConfigMgr::writeToFile()
{
    // ǰ�����ļ�����
    if (filesystem::exists(path)) 
    {
        ofstream myStream(path);
        for (const auto& section : cfgData) 
        {
            // дsection
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
        cerr << "�޷����ļ� " << path << " ���ж�ȡ" << endl;
        return false;
    }

    string line;
    // ��ǰ�� section ����
    string currentSection;
    while (getline(inFile, line))
    {
        // ȥ������β�Ŀհ��ַ�
        line = line.substr(line.find_first_not_of(" \t"));
        line = line.substr(0, line.find_last_not_of(" \t") + 1);
        // ����ǿ��л�ע���У�����
        if (line.empty() || line[0] == ';' || line[0] == '#')
        {
            continue;
        }
        // �����section
        if (line[0] == '[' && line[line.size() - 1] == ']')
        {
            // ��ȡsection����
            currentSection = line.substr(1, line.size() - 2);
            // ����� section �������� configData �У���ʼ��һ���� map
            if (cfgData.find(currentSection) == cfgData.end())
            {
                cfgData[currentSection] = map<string, string>();
            }
        }
        //����Ǽ�ֵ��
        else
        {
            size_t pos = line.find('=');
            if (pos != string::npos)
            {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                //ȥ������ֵ���˵Ŀհ��ַ�
                key = key.substr(key.find_first_not_of(" \t"));
                key = key.substr(0, key.find_last_not_of(" \t") + 1);
                value = value.substr(value.find_first_not_of(" \t"));
                value = value.substr(0, value.find_last_not_of(" \t") + 1);
                // �洢������
                if (!currentSection.empty()) {
                    cfgData[currentSection][key] = value;
                }
            }
        }
    }
    inFile.close();
    return true;
}