#pragma once
#include <map>
#include <string>
#include <vector>
#include "ConfigMgr.h"
#include "Recorder.h"
using namespace std;

/*
�涨���еķ��أ�
code -1 δ֪����
code 0 �ɹ�
code 1 ���Դ���֮�ڵ��������
    msg ʣ�ೢ�Դ���
code 2 ��������ﵽ����
code 3 δע��
code 4 �˺���ע��
code 5 ����100��������
code 6 ����
code 7 ԽȨ����
code 8 ��������ȡ���޶�
code 9 ��������ȡ���޶�
code 10 Ŀ���û�������
code 11 �����벻��ȷ
code 12 ���벻���ϰ�ȫ����
code 13 ������֤��
code 14 ���п��Ų�����Ҫ��
code 15 ���Ϸ�
*/
class Machine
{
public:
    Machine();
    ~Machine();
    //��¼
    map<string, string> login(string username, string password);
    //�ǳ�
    map<string, string> logout();
    //ȡ��
    map<string, string> withdraw(int value);
    //ת��
    map<string, string> transfer(string target_username, int value);
    //ע��
    map<string, string> signup(string username, string password, string id, string name);
    //�޸�����
    map<string, string> revise_pwd(string old_pwd, string new_pwd);
    //��ѯ���
    map<string, string> query();
    //��ȡ��ǰ�û�
    string get_current_user();
    //��ȡ����ȡ���޶�
    int get_once_limit();
    //��ȡ����ȡ���޶�
    int get_day_limit();


private:
    ConfigMgr userCfg;
    Recorder loginRec;
    Recorder withdrawRec;
    //�Ƿ��Ѿ���¼
    bool is_login = false;
    //��ǰ�û�
    string current_user;
    //�����ʽ����
    bool is_pwd_valid(string password);
    //���֤�Ÿ�ʽ����
    bool is_id_valid(string id);
    //�û�����ʽ����
    bool is_username_valid(string username);
    //���֤��У��λ
    const vector<char> verify_code_vec = 
    { '1', '0', 'X', '9', '8', '7', '6', '5','4', '3', '2'};
    //���֤��У��λ��Ӧ�ļ�Ȩ����
    const vector<int> verify_sum_vec = 
    {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    //����ȡ���޶�
    const int day_limit = 2000;
    //����ȡ���޶�
    const int once_limit = 500;
};