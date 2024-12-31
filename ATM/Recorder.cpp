#include "Recorder.h"
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
using namespace std;


Recorder::Recorder(string path)
{
    this->path = path;
    if (!filesystem::exists(path))
    {
        filesystem::path path_obj(this->path);
        filesystem::path dir_obj = path_obj.parent_path();
        filesystem::create_directories(dir_obj);
        ofstream myStream(this->path);
        myStream.close();
    }
}

Recorder::~Recorder()
{
}

bool Recorder::write(string content)
{
    //���ļ�����׷�ӣ�
    ofstream myStream(path, ofstream::app);
    //��ȡʱ��
    vector<int> nowtime = get_time();
    //д���ļ�
    for (size_t i = 0; i < 6; i++)
    {
        if (i < 5)
        {
            myStream << nowtime[i] << '-';
        }
        else
        {
            myStream << nowtime[i];
        }
    }
    myStream << ',' << content << endl;
    //�ر��ļ�
    myStream.close();
    return true;
}

vector<int> Recorder::get_time()
{
    // ��ȡ��ǰʱ��㣨�߾���ʱ�ӣ�
    auto now = chrono::system_clock::now();

    // ����ǰʱ���ת��Ϊϵͳʱ�䣨std::time_t ���ͣ�
    time_t now_time_t = chrono::system_clock::to_time_t(now);

    // �� std::time_t ת��Ϊ����ʱ�䣨����ʱ�䣩��ϵͳʱ����
    struct tm local_time;
    if (localtime_s(&local_time, &now_time_t) != 0) {
        throw runtime_error("����ʱ�����޷���ȡ��ǰʱ��");
    }

    // ʹ���ַ��������и�ʽ��
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", &local_time);

    // �и��ַ���
    string time_str(buffer);
    vector<string> splited_time = split(time_str, "-");

    // ת��Ϊ����������
    vector<int> res;
    for (const auto& str : splited_time)
    {
        res.push_back(stoi(str));
    }

    return res;
}

vector<string> Recorder::split(string str, string delimiter)
{
    vector<std::string> result;
    size_t pos = 0;
    size_t delimiter_len = delimiter.length();

    try
    {
        while ((pos = str.find(delimiter)) != string::npos) {
            result.push_back(str.substr(0, pos));
            str.erase(0, pos + delimiter_len);
        }

        result.push_back(str);  // �����һ�����ּӽ�ȥ
    }
    catch (const std::exception& e)
    {
        cout << "�ָ��ַ���ʱ��������" << e.what() << endl;
    }
    return result;
}

vector<vector<int>> Recorder::read_wrong_records(string username)
{
    ifstream myStream(path);
    vector<vector<int>> res;
    string line;
    while (getline(myStream, line))
    {
        vector<string> splited_line;
        vector<string> str_timepart;
        vector<int> int_timepart;
        splited_line = split(line, ",");
        if (splited_line[1] == username)
        {
            str_timepart = split(splited_line[0], "-");
            for (auto& i : str_timepart)
            {
                int_timepart.push_back(stoi(i));
            }
            res.push_back(int_timepart);
        }
    }
    return res;
}

int Recorder::get_times(string username)
{
    //����������
    int times = 0;
    //��ȡ��ǰʱ��
    vector<int> nowtime = get_time();
    for (auto& i : read_wrong_records(username))
    {
        //����ǽ������

        //�������
        /*cout << i[0] << endl;
        cout << nowtime[0] << endl;
        cout << i[1] << endl;
        cout << nowtime[1] << endl;
        cout << i[2] << endl;
        cout << nowtime[2] << endl;*/

        if (i[0] == nowtime[0] and i[1] == nowtime[1] and i[2] == nowtime[2])
        {
            times++;
        }
    }
    return times;
}

bool Recorder::remove(string username)
{
    ifstream old(path);
    //��ʱlines
    vector<string> new_lines;
    string line;
    while (getline(old, line))
    {
        vector<string> splited_line;
        splited_line = split(line, ",");
        //�������Ŀ��
        if (splited_line[1] != username)
        {
            new_lines.push_back(line);
        }
    }
    old.close();
    //��д���ļ�
    ofstream s(path);
    for (auto& i : new_lines)
    {
        s << i << endl;
    }
    s.close();
    return true;
}

int Recorder::get_amount(string username)
{
    //�ܽ��
    int res = 0;
    ifstream myStream(path);
    string line;
    //����ʱ��
    vector<int> nowtime = get_time();
    while (getline(myStream, line))
    {
        vector<string> splited_line;
        vector<int> int_timepart;
        splited_line = split(line, ",");
        //���������û�
        if (splited_line[1] == username)
        {
            //��������ǽ���
            if (int_timepart[0] == nowtime[0] and int_timepart[1] == nowtime[1] and int_timepart[2] == nowtime[2])
            {
                res += stoi(splited_line[2]);
            }
        }
    }
    return res;
}