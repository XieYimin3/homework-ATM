#pragma once
#include <map>
#include <string>
#include <vector>
#include "ConfigMgr.h"
#include "Recorder.h"
using namespace std;

/*
规定所有的返回：
code -1 未知错误
code 0 成功
code 1 尝试次数之内的密码错误
    msg 剩余尝试次数
code 2 错误次数达到上限
code 3 未注册
code 4 账号已注册
code 5 不是100的整数倍
code 6 余额不足
code 7 越权操作
code 8 超出单日取款限额
code 9 超出单次取款限额
code 10 目标用户不存在
code 11 旧密码不正确
code 12 密码不符合安全策略
code 13 虚假身份证号
code 14 银行卡号不符合要求
code 15 金额不合法
*/
class Machine
{
public:
    Machine();
    ~Machine();
    //登录
    map<string, string> login(string username, string password);
    //登出
    map<string, string> logout();
    //取款
    map<string, string> withdraw(int value);
    //转账
    map<string, string> transfer(string target_username, int value);
    //注册
    map<string, string> signup(string username, string password, string id, string name);
    //修改密码
    map<string, string> revise_pwd(string old_pwd, string new_pwd);
    //查询余额
    map<string, string> query();
    //获取当前用户
    string get_current_user();
    //获取单次取款限额
    int get_once_limit();
    //获取单日取款限额
    int get_day_limit();


private:
    ConfigMgr userCfg;
    Recorder loginRec;
    Recorder withdrawRec;
    //是否已经登录
    bool is_login = false;
    //当前用户
    string current_user;
    //密码格式检验
    bool is_pwd_valid(string password);
    //身份证号格式检验
    bool is_id_valid(string id);
    //用户名格式检验
    bool is_username_valid(string username);
    //身份证号校验位
    const vector<char> verify_code_vec = 
    { '1', '0', 'X', '9', '8', '7', '6', '5','4', '3', '2'};
    //身份证号校验位对应的加权因子
    const vector<int> verify_sum_vec = 
    {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    //单日取款限额
    const int day_limit = 2000;
    //单次取款限额
    const int once_limit = 500;
};