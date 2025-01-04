#pragma once
#include "Machine.h"
#include <functional>
#include <map>
#include <string>
using namespace std;

class CLI {
public:
    CLI();
    ~CLI();
    //标题
    void title(string title);
    //以下函数都是和用户交互的，会花费时间
    void help();
    void cover();
    void home();
    void signup();
    void withdraw();
    void transfer();
    void query();
    void login();
    void logout();
    void revise_pwd();

private:
    Machine myMachine;
    //隐藏式输入密码
    string input_password();
    //启动一个活动
    void start(string target);
    //注册的活动列表
    map<string, function<void(CLI*)>> activity_map;
    //活动注册器
    void activity_register();
    //输入正整数，否则返回-1
    int input_int();
};
