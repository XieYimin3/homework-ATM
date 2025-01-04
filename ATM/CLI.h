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
    //����
    void title(string title);
    //���º������Ǻ��û������ģ��Ứ��ʱ��
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
    //����ʽ��������
    string input_password();
    //����һ���
    void start(string target);
    //ע��Ļ�б�
    map<string, function<void(CLI*)>> activity_map;
    //�ע����
    void activity_register();
    //���������������򷵻�-1
    int input_int();
};
