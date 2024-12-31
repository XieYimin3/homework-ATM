#pragma once
#include <string>
#include <vector>
using namespace std;
/*
Y-m-d-H-M-S,[用户名],([取款金额])
*/
class Recorder
{
public:
    Recorder(string path);
    ~Recorder();
    //写
    bool write(string content);
    //根据用户名读取时间记录
    vector<vector<int>> read_wrong_records(string username);
    //获取错误次数
    int get_times(string username);
    //删除错误记录
    bool remove(string username);
    //获取用户今日总取款金额
    int get_amount(string username);
private:
    string path;
    //获取当前时间
    vector<int> get_time();
    //轮子：切割字符串
    vector<string> split(string str, string delimiter);

};
