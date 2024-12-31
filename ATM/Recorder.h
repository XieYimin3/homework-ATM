#pragma once
#include <string>
#include <vector>
using namespace std;
/*
Y-m-d-H-M-S,[�û���],([ȡ����])
*/
class Recorder
{
public:
    Recorder(string path);
    ~Recorder();
    //д
    bool write(string content);
    //�����û�����ȡʱ���¼
    vector<vector<int>> read_wrong_records(string username);
    //��ȡ�������
    int get_times(string username);
    //ɾ�������¼
    bool remove(string username);
    //��ȡ�û�������ȡ����
    int get_amount(string username);
private:
    string path;
    //��ȡ��ǰʱ��
    vector<int> get_time();
    //���ӣ��и��ַ���
    vector<string> split(string str, string delimiter);

};
