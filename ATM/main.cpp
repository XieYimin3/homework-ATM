// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include "CLI.h"
#include "ConfigMgr.h"
#include "Machine.h"
#include <iostream>
#include <stdexcept>
#include <csignal>
#include <future>
using namespace std;

void signalHandler(int signum)
{
    cout << "\033[31m接收到信号：" << signum << "，即将退出程序！...";
    //等待2秒
    this_thread::sleep_for(chrono::seconds(2));
    exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler);
    cout << "初始化中..." << endl;
    try
    {
        CLI myUI;
    }
    catch (const std::runtime_error & e)
    {
        cout << "发生运行时错误：" << e.what() << endl;
    }
    catch (const std::exception& e)
    {
        cout << "发生错误：" << e.what() << endl;
    }
    catch (...)
    {
        cout << "发生未知错误" << endl;
    }
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
