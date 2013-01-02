#include <iostream>
#include <string>
#include "Error.h"

using namespace std;

Error::Error()
{
	initialize();  // 调用初始化函数
}

// 输出错误的类型
void Error::error(int err_num,int line_num)
{
	cout << "line " << line_num << ":" <<"error " << err_num << ":" << error_types[err_num] << endl;
}

void Error::initialize()
{
	error_types.insert(make_pair(1,"输入文件不存在"));
	error_types.insert(make_pair(2,"'!='缺少'='"));
	error_types.insert(make_pair(3,"字符缺少左部的\"'\""));
	error_types.insert(make_pair(4,"缺少return关键字"));
	error_types.insert(make_pair(5,"缺少'('括号"));
	error_types.insert(make_pair(6,"缺少')'括号"));
	error_types.insert(make_pair(7,"缺少'['括号"));
	error_types.insert(make_pair(8,"缺少']'括号"));
	error_types.insert(make_pair(9,"缺少'{'括号"));
	error_types.insert(make_pair(10,"缺少'}'括号"));
	error_types.insert(make_pair(11,"字符串中的两个引号必须在同一行存在"));
	error_types.insert(make_pair(12,"写语句应该是printf开始"));
	error_types.insert(make_pair(13,"写语句中字符串后面所接符号不合法"));
	error_types.insert(make_pair(14,"读语句应该以scanf开始"));
	error_types.insert(make_pair(15,"应该为标识符"));
	error_types.insert(make_pair(16,"应该为const"));
	error_types.insert(make_pair(17,"应以分号结尾"));
	error_types.insert(make_pair(18,"常量定义只能够以int或者char开始"));
	error_types.insert(make_pair(19,"应该是赋值符号="));
	error_types.insert(make_pair(20,"应该是一个整数"));
	error_types.insert(make_pair(21,"不能够表示整数"));
	error_types.insert(make_pair(22,"应该是一个字符常量"));
	error_types.insert(make_pair(23,"不合法的声明头部"));
	error_types.insert(make_pair(24,"类型标识符只能够使int或者char"));
	error_types.insert(make_pair(25,"应输入大于0的整数"));
	error_types.insert(make_pair(26,"应该以int，char或者void开始"));
	error_types.insert(make_pair(27,"主函数返回值类型只能够使void"));
	error_types.insert(make_pair(28,"主函数名只能够是main"));
	error_types.insert(make_pair(29,"不正确的赋值语句"));
	error_types.insert(make_pair(30,"缺少if"));
	error_types.insert(make_pair(31,"缺少do关键字"));
	error_types.insert(make_pair(32,"缺少while关键字"));
	error_types.insert(make_pair(33,"缺少for关键字"));
	error_types.insert(make_pair(34,"缺少;"));
	error_types.insert(make_pair(35,"缺少加法运算符+/-"));
	error_types.insert(make_pair(36,"不正确的开始符号"));
	error_types.insert(make_pair(37,"缺少=或者（"));
	error_types.insert(make_pair(38,"已存在变量名"));
}