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
//��ɫ����
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
//24λɫ
#define RGB_66CCFF  "\033[38;2;102;204;255m"
using namespace std;


CLI::CLI() {
    try
    {
        activity_register();
    }
    catch (const std::exception& e)
    {
        cout << RED << "�ע��ʱ��������" << e.what() << RESET << endl;
    }
    //��ȡ������Ϣ
    string author;
    ifstream author_file("author.txt");
    if (author_file.is_open())
    {
        getline(author_file, author);
        author_file.close();
    }
    else
    {
        //Ĭ������
        author = "XieYimin3";
    }
    //��ӭ����
    cout << GREEN << "��ӭʹ��ģ��ATM" << RESET << endl;
    cout << "c++������ƿγ���Ŀ ���ߣ�" + author << endl;
    cout << YELLOW << "����ʹ�����°�Windows�ն�" << RESET << endl;
    // ��������
    try
    {
        start("cover");
    }
    catch (const std::exception& e)
    {
        cout << RED << "��������ʱ��������" << e.what() << RESET << endl;
    }
}

CLI::~CLI() {}

void CLI::cover() {
    title("����");
    int option = -1;
    while (true) {
        cout << RGB_66CCFF;
        cout << "[1] ��¼" << endl;
        cout << "[2] ע��" << endl;
        cout << "[3] ����" << endl;
        cout << "[0] �˳�ϵͳ" << endl;
        cout << RESET;
        
        option = input_int();
        if (option == -1)
        {
            cout << RED << "����Ĳ������֣�" << RESET << endl;
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
            cout << GREEN << "���˳�����ӭ�´�ʹ��" << RESET << endl;
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
    title("��ҳ����ǰ�û���" + username + "��");
    int option = -1;
    while(true)
    {
        cout << RGB_66CCFF;
        cout << "[1] ȡ��" << endl;
        cout << "[2] ת��" << endl;
        cout << "[3] ����ѯ" << endl;
        cout << "[4] �޸�����" << endl;
        cout << "[0] �ǳ�" << endl;
        cout << RESET;
        
        option = input_int();
        if (option == -1)
        {
            cout << RED << "����Ĳ������֣�" << RESET << endl;
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
    title("��¼");

    string username;
    string password;
    cout << CYAN << "�忨���������˺ţ�" << RESET <<endl;
    cin >> username;
    //ѭ����������
    while (true)
    {
        cout << CYAN << "�������룺" << RESET << endl;
        //��������
        password = this->input_password();
        //��˵�¼
        map<string, string> res = myMachine.login(username, password);
        if (res["code"] == "0")
        {
            cout << GREEN << "���ã�" << res["msg"] << "����ӭʹ��" << RESET << endl;
            start("home");
            return;
        }
        else if (res["code"] == "1")
        {
            cout << RED << "������󣬽���ʣ��" << MAGENTA << res["msg"] << RED << "�γ��Ի���" << RESET << endl;
            //����ѭ��
            while (true)
            {
                cout << RGB_66CCFF;
                cout << "[1] ����" << endl;
                cout << "[0] ����" << endl;
                cout << RESET;
                int option = -1;
                
                option = input_int();
                if (option == -1)
                {
                    cout << RED << "����Ĳ������֣�" << RESET << endl;
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
            cout << RED << "��½ʧ�ܣ���Ϊ���û����ճ��Դ����Ѵ�����" << RESET << endl;
            start("cover");
            return;
        }
        else if (res["code"] == "3")
        {
            cout << RED << "��½ʧ�ܣ���Ϊ���û�δע��" << RESET << endl;
            start("cover");
            return;
        }
        else
        {
            cout << RED << "����δ֪��������ϵ����Ա" << RESET << endl;
            start("cover");
            return;
        }
    }
}

void CLI::withdraw()
{
    title("ȡ��");

    int value;
    cout << CYAN << "����ȡ����" << RESET << endl;
    value = input_int();
    if (value == -1)
    {
        cout << RED << "����Ĳ������֣�" << RESET << endl;
        start("home");
        return;
    }

    map<string, string> res = myMachine.withdraw(value);
    if (res["code"] == "0")
    {
        cout << GREEN << "ȡ��ɹ�" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        cout << RED << "ȡ��ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

void CLI::signup()
{
    title("ע��");
    string username, password, id, name;
    cout << CYAN << "�������п��ţ�19λ���֣����ڵ�¼����" << RESET << endl;
    cin >> username;
    cout << CYAN << "�������֤�ţ�" << RESET << endl;
    cin >> id;
    cout << CYAN << "����������" << RESET << endl;
    cin >> name;
    cout << CYAN << "����6λ�������룺" << RESET << endl;
    password = input_password();
    cout << CYAN << "ȷ�����룺" << RESET << endl;
    if (password != input_password())
    {
        cout << RED << "ע��ʧ�ܣ���Ϊ�����벻һ��" << RESET << endl;
        start("cover");
        return;
    }
    map<string, string> res = myMachine.signup(username, password, id, name);
    if (res["code"] == "0")
    {
        cout << GREEN << "�û�" << username << "ע��ɹ�" << RESET << endl;
        cout << MAGENTA << "��лע�ᣡ��Ϊ���û�����10000Ԫ��" << RESET << endl;
        start("cover");
        return;
    }
    else
    {
        cout << RED << "ע��ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
        start("cover");
        return;
    }

}

void CLI::transfer()
{
    title("ת��");

    int value;
    string target_username;
    string target_username_2;
    cout << CYAN << "����Է��û��˺ţ�" << RESET << endl;
    cin >> target_username;
    cout << MAGENTA << "Ϊ���������������ٴ�ȷ�϶Է��û��˺ţ�" << RESET << endl;
    cin >> target_username_2;
    if (target_username != target_username_2)
    {
        cout << RED << "����������˺Ų�һ�£�ת�˱�ȡ����" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        cout << CYAN << "����ת�˽�" << RESET << endl;
        value = input_int();
        if (value == -1)
        {
            cout << RED << "����Ĳ������֣�" << RESET << endl;
            start("home");
            return;
        }
        map<string, string> res = myMachine.transfer(target_username, value);

        if (res["code"] == "0")
        {
            cout << GREEN << "ת�˳ɹ�" << RESET << endl;
            start("home");
            return;
        }
        else
        {
            cout << RED << "ת��ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
            start("home");
            return;
        }
    }
}

void CLI::query()
{
    title("����ѯ");


    cout << BLUE << "��ѯ��..." << RESET << endl;
    map<string, string> res = myMachine.query();

    if (res["code"] == "0")
    {
        cout << GREEN << "�˻���" << MAGENTA << res["msg"] << GREEN << "Ԫ" << RESET << endl;
        int option = -1;
        while (true)
        {
            cout << RGB_66CCFF << "[0] ����" << RESET << endl;
            option = input_int();
            if (option == -1)
            {
                cout << RED << "����Ĳ������֣�" << RESET << endl;
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
        cout << RED << "ȡ��ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

void CLI::logout()
{
    map<string, string> res = myMachine.logout();
    if (res["code"] == "0")
    {
        cout << GREEN << "�ѵǳ�" << RESET << endl;
        start("cover");
        return;
    }
    else
    {
        cout << RED <<  "�ǳ�ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
        start("home");
        return;
    }
}

string CLI::input_password()
{
    string password;
    char ch;
    while ((ch = _getch()) != '\r') 
    { // ����Enterʱ
        //�˸�
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
    cout << "ʹ��˵����" << endl;
    //todo
    cout << "���� https://github.com/XieYimin3/homework-ATM �Ի�ȡ������Ϣ" << endl;
    start("cover");
    return;
}

void CLI::start(string target)
{
    // ���Һ���������
    auto it = activity_map.find(target);
    if (it != activity_map.end()) {
        try
        {
            auto f = async(launch::async, it->second, this);
        }
        catch (const std::exception& e)
        {
            cout << RED << "�����ʱ��������" << e.what() << RESET << endl;
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
    title("�޸�����");

    string old_pwd;
    string new_pwd;
    cout << CYAN << "��������룺" << RESET << endl;
    old_pwd = input_password();
    cout << CYAN << "���������룺" << RESET << endl;
    new_pwd = input_password();
    cout << CYAN << "�ٴ�ȷ�������룺" << RESET << endl;
    if (new_pwd != input_password())
    {
        cout << RED << "��������������벻һ�£�" << RESET << endl;
        start("home");
        return;
    }
    else
    {
        map<string, string> res = myMachine.revise_pwd(old_pwd, new_pwd);
        if (res["code"] == "0")
        {
            cout << GREEN << "�޸ĳɹ�" << RESET << endl;
            cout << BLUE << "�����˳��������µ�¼��" << RESET << endl;
            start("logout");
            return;
        }
        else
        {
            cout << RED << "�޸�ʧ�ܣ���Ϊ��" << res["msg"] << RESET << endl;
            start("home");
            return;
        }
    }
}

int CLI::input_int()
{
    int input;
    cin >> input;
    //��������option�Ƿ�Ϊ����
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1024, '\n');
        return - 1;
    }
    return input;
}