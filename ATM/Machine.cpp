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
    //存放结果
    map<string, string> res;
    //配置
    map<string, map<string, string>> config = userCfg.cfgData;
    //遍历用户
    for (auto& i : config)
    {
        //如果找到了用户
        if (i.first == username)
        {
            //错误次数
            int wrong_times = loginRec.get_times(username);
            //错误次数小于三时
            if (wrong_times < 3)
            {
                //密码正确
                if (i.second["password"] == password)
                {
                    current_user = username;
                    is_login = true;
                    loginRec.remove(username);
                    res["code"] = "0";
                    //返回用户姓名
                    res["msg"] = i.second["name"];
                    return res;
                }
                //最后一次密码错误
                else if (wrong_times == 2)
                {
                    res["code"] = "2";
                    res["msg"] = "错误次数达到上限";
                    //追加错误记录
                    loginRec.write(username);
                    return res;
                }
                //一般密码错误
                else
                {
                    res["code"] = "1";
                    res["msg"] = to_string(2 - wrong_times);
                    //追加错误记录
                    loginRec.write(username);
                    return res;
                }
            }
            //错误次数>=3
            else if (wrong_times >= 3)
            {
                res["code"] = "2";
                res["msg"] = "错误次数达到上限";
                return res;
            }
        }
    }
    res["code"] = "3";
    res["msg"] = "此用户未注册";
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
            res["msg"] = "不是100的整数倍";
            return res;
        }
        else if (value <= 0)
        {
            res["code"] = "15";
            res["msg"] = "金额不合法";
            return res;
        }
        else if (value > this->once_limit)
        {
            res["code"] = "9";
            res["msg"] = "超出单次取款限额";
            return res;
        }
        else if (remain < value)
        {
            res["code"] = "6";
            res["msg"] = "余额不足";
            return res;
        }
        else if ((withdrawRec.get_amount(current_user) + value) > this->day_limit)
        {
            res["code"] = "8";
            res["msg"] = "超出单日取款限额";
            return res;
        }
        else
        {
            userCfg.set(current_user, "remain", to_string(remain - value));
            withdrawRec.write(current_user + ',' + to_string(value));
            res["code"] = "0";
            res["msg"] = "取款成功";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "越权操作";
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
            res["msg"] = "旧密码不正确";
            return res;
        }
        else if (!is_pwd_valid(new_pwd))
        {
            res["code"] = "12";
            res["msg"] = "密码不符合安全策略：6位 数字";
            return res;
        }
        else if (old_pwd == new_pwd)
        {
            res["code"] = "16";
            res["msg"] = "新密码与旧密码相同";
            return res;
        }
        else
        {
            userCfg.set(current_user, "password", new_pwd);
            res["code"] = "0";
            res["msg"] = "修改成功";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "越权操作";
    return res;
}

map<string, string> Machine::signup(string username, string password, string id, string name)
{
    map<string, string> res;
    //检查密码是否合法
    if (!is_pwd_valid(password))
    {
        res["code"] = "12";
        res["msg"] = "密码不符合安全策略：6位 数字";
        return res;
    }
    //身份证号是否合法
    if (!is_id_valid(id))
    {
        res["code"] = "13";
        res["msg"] = "虚假身份证号";
        return res;
    }
    //用户名是否合法
    if (!is_username_valid(username))
    {
        res["code"] = "14";
        res["msg"] = "银行卡号不符合要求";
        return res;
    }
    map<string, map<string, string>> config = userCfg.cfgData;
    for (auto& i : config)
    {
        if (i.first == username)
        {
            res["code"] = "4";
            res["msg"] = "账号" + username + "已经被注册过";
            return res;
        }
    }
    if (userCfg.set(username, "password", password) and 
        userCfg.set(username, "remain", "10000") and 
        userCfg.set(username, "id", id) and 
        userCfg.set(username, "name", name))
    {
        res["code"] = "0";
        res["msg"] = "账号" + username + "注册成功";
        return res;
    }
    res["code"] = "-1";
    res["msg"] = "未知错误";
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
    res["msg"] = "已登出";
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
            res["msg"] = "目标用户不存在";
            return res;
        }
        else if (value <= 0)
        {
            res["code"] = "15";
            res["msg"] = "金额不合法";
            return res;
        }
        else if (remain < value)
        {
            res["code"] = "6";
            res["msg"] = "余额不足";
            return res;
        }
        else
        {
            userCfg.set(current_user, "remain", to_string(remain - value));
            userCfg.set(target_username, "remain", to_string(target_remain + value));
            res["code"] = "0";
            res["msg"] = "转账成功";
            return res;
        }
    }
    res["code"] = "7";
    res["msg"] = "越权操作";
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
    res["msg"] = "越权操作";
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