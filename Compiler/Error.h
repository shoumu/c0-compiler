#ifndef __ERROR_H__
#define __ERROR_H__

#include <iostream>
#include <map>
#include <string>

using namespace std;

class Error
{
public:
	Error(); // constructor
	void error(int err_num,int line_num); // ������

private:
	map<int,string> error_types; // ��ʾ���������
	//static int error_nums; // ��ʾ��ǰerr������

	void initialize(); // ����Դ�������ͽ��г�ʼ��
};



#endif