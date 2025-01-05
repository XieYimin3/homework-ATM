#include "Machine.h"
#include "Recorder.h"
#include <map>
using namespace std;


Machine::Machine():
    userCfg(".\\configs\\users.ini"), 
    withdrawRec(".\\records\\withdraw.log"), 
    loginRec(".\\records\\login.log")
{

}

Machine::~Machine()
{
}

map<string, string> Machine::login(string username, string password)
{
    //��Ž��
    map<string, string> res;
    //����
    map<string, map<string, string>> config = userCfg.cfgData;
    //�����û�
    for (auto& i : config)
    {
        //����ҵ����û�
        if (i.first == username)
        {
            //�������
            int wrong_times = loginRec.get_times(username);
            //�������С����ʱ
            if (wrong_times < 3)
            {
                //������ȷ
                if (i.second["password"] == password)
                {
                    current_user = username;
                    is_login = true;
                    loginRec.remove(username);
                    res["code"] = "0";
                    //�����û�����
                    res["msg"] = i.second["name"];
                    return res;
                }
                //���һ���������
                else if (wrong_times == 2)
                {
                    res["code"] = "2";
                    res["msg"] = "��������ﵽ����";
                    //׷�Ӵ����¼
                    loginRec.write(username);
                    return res;
                }
                //һ���������
                else
                {
                    res["code"] = "1";
                    res["msg"] = to_string(2 - wrong_times);
                    //׷�Ӵ����¼
                    loginRec.write(username);
                    return res;
                }
            }
            //�������>=3
            else if (wrong_times >= 3)
            {
                res["code"] = "2";
                res["msg"] = "��������ﵽ����";
                return res;
            }
        }
    }
    res["code"] = "3";
    res["msg"] = "���û�δע��";
    return res;
}

map<string, string> Machine::withdraw(int value)
{
    map<string, string> res;
    int remain = stoi(userCfg.cfgData[current_user]["remain"]);
    if (is_login)
    {
        if (value != (value / 100 * 100))
        {
            res["code"] = "5";
            res["msg"] = "����100��������";
            return res;
        }
        else if (value <= 0)
        {
            res["code"] = "15";
            res["msg"] = "���Ϸ�";
            return res;
        }
        else if (value > this->once_limit)
        {
            res["code"] = "9";
            res["msg"] = "��������ȡ���޶�";
            return res;
        }
        else if (remain < value)
        {
            res["code"] = "6";
            res["msg"] = "����";
            return res;
        }
        else if ((withdrawRec.get_amount(current_user) + value) > this->day_limit)
        {
            res["code"] = "8";
            res["msg"] = "��������ȡ���޶�";
            return res;
        }
        else
        {
            userCfg.set(current_user, "remain", to_string(remain - value));
            withdrawRec.write(current_user + ',' + to_string(value));
            res["code"] = "0";
            res["msg"] = "ȡ��ɹ�";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "ԽȨ����";
    return res;
}

map<string, string> Machine::revise_pwd(string old_pwd, string new_pwd)
{
    map<string, string> res;
    if (is_login)
    {
        if (old_pwd != userCfg.cfgData[current_user]["password"])
        {
            res["code"] = "11";
            res["msg"] = "�����벻��ȷ";
            return res;
        }
        else if (!is_pwd_valid(new_pwd))
        {
            res["code"] = "12";
            res["msg"] = "���벻���ϰ�ȫ���ԣ�6λ ����";
            return res;
        }
        else if (old_pwd == new_pwd)
        {
            res["code"] = "16";
            res["msg"] = "���������������ͬ";
            return res;
        }
        else
        {
            userCfg.set(current_user, "password", new_pwd);
            res["code"] = "0";
            res["msg"] = "�޸ĳɹ�";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "ԽȨ����";
    return res;
}

map<string, string> Machine::signup(string username, string password, string id, string name)
{
    map<string, string> res;
    //��������Ƿ�Ϸ�
    if (!is_pwd_valid(password))
    {
        res["code"] = "12";
        res["msg"] = "���벻���ϰ�ȫ���ԣ�6λ ����";
        return res;
    }
    //���֤���Ƿ�Ϸ�
    if (!is_id_valid(id))
    {
        res["code"] = "13";
        res["msg"] = "������֤��";
        return res;
    }
    //�û����Ƿ�Ϸ�
    if (!is_username_valid(username))
    {
        res["code"] = "14";
        res["msg"] = "���п��Ų�����Ҫ��";
        return res;
    }
    map<string, map<string, string>> config = userCfg.cfgData;
    for (auto& i : config)
    {
        if (i.first == username)
        {
            res["code"] = "4";
            res["msg"] = "�˺�" + username + "�Ѿ���ע���";
            return res;
        }
    }
    if (userCfg.set(username, "password", password) and 
        userCfg.set(username, "remain", "10000") and 
        userCfg.set(username, "id", id) and 
        userCfg.set(username, "name", name))
    {
        res["code"] = "0";
        res["msg"] = "�˺�" + username + "ע��ɹ�";
        return res;
    }
    res["code"] = "-1";
    res["msg"] = "δ֪����";
    return res;
}

string Machine::get_current_user()
{
    return current_user;
}

map<string, string> Machine::logout()
{
    map<string, string> res;
    is_login = false;
    current_user = "";
    res["code"] = "0";
    res["msg"] = "�ѵǳ�";
    return res;
}

map<string, string> Machine::transfer(string target_username, int value)
{
    map<string, string> res;
    if (is_login)
    {
        int remain = stoi(userCfg.cfgData[current_user]["remain"]);
        int target_remain = stoi(userCfg.cfgData[current_user]["remain"]);
        bool target_exist = false;
        for (auto& i : userCfg.cfgData)
        {
            if (i.first == target_username)
            {
                target_exist = true;
            }
        }
        if (!target_exist)
        {
            res["code"] = "10";
            res["msg"] = "Ŀ���û�������";
            return res;
        }
        else if (value <= 0)
        {
            res["code"] = "15";
            res["msg"] = "���Ϸ�";
            return res;
        }
        else if (remain < value)
        {
            res["code"] = "6";
            res["msg"] = "����";
            return res;
        }
        else
        {
            userCfg.set(current_user, "remain", to_string(remain - value));
            userCfg.set(target_username, "remain", to_string(target_remain + value));
            res["code"] = "0";
            res["msg"] = "ת�˳ɹ�";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "ԽȨ����";
    return res;
}

map<string, string> Machine::query()
{
    map<string, string> res;
    if (is_login)
    {
        string remain = userCfg.cfgData[current_user]["remain"];
        res["code"] = "0";
        res["msg"] = remain;
        return res;
    }
    res["code"] = "7";
    res["msg"] = "ԽȨ����";
    return res;
}

bool Machine::is_pwd_valid(string password)
{
    if (password.size() != 6)
    {
        return false;
    }
    for (char& i : password)
    {
        if (!(i >= '0' and i <= '9'))
        {
            return false;
        }
    }
    return true;
}

bool Machine::is_id_valid(string id)
{
    if (id.size() != 18)
    {
        return false;
    }
    int sum = 0;
    for (int i = 0; i < 17; i++)
    {
        sum += (id[i] - '0') * verify_sum_vec[i];
    }
    if (id[17] == verify_code_vec[sum % 11])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Machine::is_username_valid(string username)
{
    if (username.size() != 19)
    {
        return false;
    }
    for (char& i : username)
    {
        if (!(i >= '0' and i <= '9'))
        {
            return false;
        }
    }
    return true;
}

int Machine::get_once_limit()
{
    return this->once_limit;
}

int Machine::get_day_limit()
{
    return this->day_limit;
}