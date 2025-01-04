#include "CLI.h"
#include "Machine.h"
#include <iostream>
#include <map>
#include <iomanip>
#include <string>
#include <thread>
#include <functional>
#include <future>
#include <fstream>
#include <conio.h>

#define RESET   "\033[0m"
//颜色符号
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
//24位色
#define RGB_66CCFF  "\033[38;2;102;204;255m"
using namespace std;


CLI::CLI() {
    try
    {
        activity_register();
    }
    catch (const std::exception& e)
    {
        cout << RED << "活动注册时发生错误：" << e.what() << RESET << endl;
    }
    //读取作者信息
    string author;
    ifstream author_file("author.txt");
    if (author_file.is_open())
    {
        getline(author_file, author);
        author_file.close();
    }
    else
    {
        //默认作者
        author = "XieYimin3";
    }
    //欢迎界面
    cout << GREEN << "欢迎使用模拟ATM" << RESET << endl;
    cout << "c++程序设计课程项目 作者：" + author << endl;
    cout << YELLOW << "建议使用最新版Windows终端" << RESET << endl;
    // 启动封面活动
    try
    {
        start("cover");
    }
    catch (const std::exception& e)
    {
        cout << RED << "启动封面活动时发生错误：" << e.what() << RESET << endl;
    }
}

CLI::~CLI() {}

void CLI::cover() {
    title("锁屏");
    int option = -1;
    while (true) {
        cout << RGB_66CCFF;
        cout << "[1] 登录" << endl;
        cout << "[2] 注册" << endl;
        cout << "[3] 帮助" << endl;
        cout << "[0] 退出系统" << endl;
        cout << RESET;
        
        option = input_int();
        if (option == -1)
        {
            cout << RED << "输入的不是数字！" << RESET << endl;
            continue;
        }

        if (option == 1) {
            start("login");
            return;
        }
        else if (option == 2) {
            start("signup");
            return;
        }
        else if (option == 3) {
            start("help");
            return;
        }
        else if (option == 0) {
            cout << GREEN << "已退出，欢迎下次使用" << RESET << endl;
            return;
        }
        else {
            cout << RED << "???" << RESET << endl;
        }
    }
}

void CLI::home()
{
    string username = myMachine.get_current_user();
    title("主页（当前用户：" + username + "）");
    int option = -1;
    while(true)
    {
        cout << RGB_66CCFF;
        cout << "[1] 取款" << endl;
        cout << "[2] 转账" << endl;
        cout << "[3] 余额查询" << endl;
        cout << "[4] 修改密码" << endl;
        cout << "[0] 登出" << endl;
        cout << RESET;
        
        option = input_int();
        if (option == -1)
        {
            cout << RED << "输入的不是数字！" << RESET << endl;
            continue;
        }

        if (option == 1)
        {
            start("withdraw");
            return;
        }
        else if (option == 2)
        {
            start("transfer");
            return;
        }
        else if (option == 3)
        {
            start("query");
            return;
        }
        else if (option == 4)
        {
            start("revise_pwd");
            return;
        }
        else if (option == 0)
        {
            start("logout");
            return;
        }
        else
        {
            cout << RED << "???" << RESET << endl;
        }
    }

}

void CLI::login()
{
    title("登录");

    string username;
    string password;
    cout << CYAN << "插卡：（输入账号）" << RESET <<endl;
    cin >> username;
    //循环输入密码
    while (true)
    {
        cout << CYAN << "输入密码：" << RESET << endl;
        //输入密码
        password = this->input_password();
        //后端登录
        map<string, string> res = myMachine.login(username, password);
        if (res["code"] == "0")
        {
            cout << GREEN << "您好，" << res["msg"] << "！欢迎使用" << RESET << endl;
            start("home");
            return;
        }
        else if (res["code"] == "1")
        {
            cout << RED << "密码错误，今日剩余" << MAGENTA << res["msg"] << RED << "次尝试机会" << RESET << endl;
            //交互循环
            while (true)
            {
                cout << RGB_66CCFF;
                cout << "[1] 重试" << endl;
                cout << "[0] 返回" << endl;
                cout << RESET;
                int option = -1;
                
                option = input_int();
                if (option == -1)
                {
                    cout << RED << "输入的不是数字！" << RESET << endl;
                    continue;
                }

                if (option == 1)
                {
                    break;
                }
                else if (option == 0)
                {
                    start("cover");
                    return;
                }
                else
                {
                    cout << RED << "???" << RESET << endl;
                }
            }
        }
        else if(res["code"] == "2")
        {
            cout << RED << "登陆失败，因为该用户今日尝试次数已达上限" << RESET << endl;
            start("cover");
            return;
        }
        else if (res["code"] == "3")
        {
            cout << RED << "登陆失败，因为该用户未注册" << RESET << endl;
            start("cover");
            return;
        }
        else
        {
            cout << RED << "发生未知错误，请联系管理员" << RESET << endl;
            start("cover");
            return;
        }
    }
}

void CLI::withdraw()
{
    title("取款");

    int value;
    cout << CYAN << "输入取款金额" << RESET << endl;
    value = input_int();
    if (value == -1)
    {
        cout << RED << "输入的不是数字！" << RESET << endl;
        start("home");
        return;
    }

    map<string, string> res = myMachine.withdraw(value);
    if (res["code"] == "0")
    {
        cout << GREEN << "取款成功" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        cout << RED << "取款失败，因为：" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

void CLI::signup()
{
    title("注册");
    string username, password, id, name;
    cout << CYAN << "输入银行卡号（19位数字，用于登录）：" << RESET << endl;
    cin >> username;
    cout << CYAN << "输入身份证号：" << RESET << endl;
    cin >> id;
    cout << CYAN << "输入姓名：" << RESET << endl;
    cin >> name;
    cout << CYAN << "设置6位数字密码：" << RESET << endl;
    password = input_password();
    cout << CYAN << "确认密码：" << RESET << endl;
    if (password != input_password())
    {
        cout << RED << "注册失败，因为：密码不一致" << RESET << endl;
        start("cover");
        return;
    }
    map<string, string> res = myMachine.signup(username, password, id, name);
    if (res["code"] == "0")
    {
        cout << GREEN << "用户" << username << "注册成功" << RESET << endl;
        cout << MAGENTA << "感谢注册！已为新用户赠送10000元。" << RESET << endl;
        start("cover");
        return;
    }
    else
    {
        cout << RED << "注册失败，因为：" << res["msg"] << RESET << endl;
        start("cover");
        return;
    }

}

void CLI::transfer()
{
    title("转账");

    int value;
    string target_username;
    string target_username_2;
    cout << CYAN << "输入对方用户账号：" << RESET << endl;
    cin >> target_username;
    cout << MAGENTA << "为避免错误操作，请再次确认对方用户账号：" << RESET << endl;
    cin >> target_username_2;
    if (target_username != target_username_2)
    {
        cout << RED << "两次输入的账号不一致，转账被取消！" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        cout << CYAN << "输入转账金额：" << RESET << endl;
        value = input_int();
        if (value == -1)
        {
            cout << RED << "输入的不是数字！" << RESET << endl;
            start("home");
            return;
        }
        map<string, string> res = myMachine.transfer(target_username, value);

        if (res["code"] == "0")
        {
            cout << GREEN << "转账成功" << RESET << endl;
            start("home");
            return;
        }
        else
        {
            cout << RED << "转账失败，因为：" << res["msg"] << RESET << endl;
            start("home");
            return;
        }
    }
}

void CLI::query()
{
    title("余额查询");


    cout << BLUE << "查询中..." << RESET << endl;
    map<string, string> res = myMachine.query();

    if (res["code"] == "0")
    {
        cout << GREEN << "账户余额：" << MAGENTA << res["msg"] << GREEN << "元" << RESET << endl;
        int option = -1;
        while (true)
        {
            cout << RGB_66CCFF << "[0] 返回" << RESET << endl;
            option = input_int();
            if (option == -1)
            {
                cout << RED << "输入的不是数字！" << RESET << endl;
                continue;
            }
            if (option == 0)
            {
                start("home");
                return;
            }
            else
            {
                cout << RED << "???" << RESET << endl;
            }
        }
    }
    else
    {
        cout << RED << "取款失败，因为：" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

void CLI::logout()
{
    map<string, string> res = myMachine.logout();
    if (res["code"] == "0")
    {
        cout << GREEN << "已登出" << RESET << endl;
        start("cover");
        return;
    }
    else
    {
        cout << RED <<  "登出失败，因为：" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

string CLI::input_password()
{
    string password;
    char ch;
    while ((ch = _getch()) != '\r') 
    { // 不是Enter时
        //退格
        if (ch == '\b') 
        { 
            if (!password.empty()) 
            {
                std::cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            password.push_back(ch);
            std::cout << '*';
        }
    }
    cout << endl;
    return password;
}

void CLI::title(string title)
{
    cout << RGB_66CCFF;
    cout << setw(20) << setfill('_') << "";
    cout << title;
    cout << setw(20) << setfill('_') << "" << endl;
    cout << RESET;
}

void CLI::help()
{
    cout << "使用说明：" << endl;
    //todo
    cout << "访问 https://github.com/XieYimin3/homework-ATM 以获取更多信息" << endl;
    start("cover");
    return;
}

void CLI::start(string target)
{
    // 查找函数并调用
    auto it = activity_map.find(target);
    if (it != activity_map.end()) {
        try
        {
            auto f = async(launch::async, it->second, this);
        }
        catch (const std::exception& e)
        {
            cout << RED << "启动活动时发生错误：" << e.what() << RESET << endl;
        }
    }
    else {
        cout << "Invalid function id" << endl;
    }

}

void CLI::activity_register() {
    activity_map["cover"] = &CLI::cover;
    activity_map["home"] = &CLI::home;
    activity_map["login"] = &CLI::login;
    activity_map["logout"] = &CLI::logout;
    activity_map["signup"] = &CLI::signup;
    activity_map["withdraw"] = &CLI::withdraw;
    activity_map["help"] = &CLI::help;
    activity_map["query"] = &CLI::query;
    activity_map["transfer"] = &CLI::transfer;
    activity_map["revise_pwd"] = &CLI::revise_pwd;
}

void CLI::revise_pwd()
{
    title("修改密码");

    string old_pwd;
    string new_pwd;
    cout << CYAN << "输入旧密码：" << RESET << endl;
    old_pwd = input_password();
    cout << CYAN << "输入新密码：" << RESET << endl;
    new_pwd = input_password();
    cout << CYAN << "再次确认新密码：" << RESET << endl;
    if (new_pwd != input_password())
    {
        cout << RED << "两次输入的新密码不一致！" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        map<string, string> res = myMachine.revise_pwd(old_pwd, new_pwd);
        if (res["code"] == "0")
        {
            cout << GREEN << "修改成功" << RESET << endl;
            cout << BLUE << "即将退出，请重新登录。" << RESET << endl;
            start("logout");
            return;
        }
        else
        {
            cout << RED << "修改失败，因为：" << res["msg"] << RESET << endl;
            start("home");
            return;
        }
    }
}

int CLI::input_int()
{
    int input;
    cin >> input;
    //检查输入的option是否为数字
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1024, '\n');
        return - 1;
    }
    return input;
}