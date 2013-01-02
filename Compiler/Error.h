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
	void error(int err_num,int line_num); // 错误函数

private:
	map<int,string> error_types; // 表示错误的类型
	//static int error_nums; // 表示当前err的数量

	void initialize(); // 这里对错误的类型进行初始化
};



#endif