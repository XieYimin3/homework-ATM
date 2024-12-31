#pragma once
#include <iostream>
#include <map>
using namespace std;

class ConfigMgr
{
public:
	ConfigMgr(string path);
	~ConfigMgr();
    //��������
    bool set(string section, string key, string value);
    //�����ڴ���������ã�
    map<string, map<string, string>> cfgData;
private:
    //�����ļ�·��
    string path;
    //���ڴ����������д�����
    bool writeToFile(); 
    //�Ӵ��̶����ã��Ѹ����ڴ��е�����
    bool update();
};
