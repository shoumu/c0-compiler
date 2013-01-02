#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include "commons.h"
#include <string>
#include <map>
#include <list>
#include <vector> 
#include <iomanip>
#include "Error.h"

using namespace std;

// 定义单个的符号
struct Symbol
{
	symbol_kind kind; //表示种类
	symbol_type type;	// 这里就是int，char之类的
	string name;  // 这里表示符号的名字，对于字符串和

	// 值相关
	char str_value[50]; // 字符串常量的值
	int int_value; // 整形的值
	char char_value; // 用来存储字符常量的值
	int array_len; // 当变量类型是数组的时候，这里用来保存数组的长度
	bool is_return; // 表示是否是返回值

	// 地址相关
	int address; // 地址
	unsigned int space_size; // 地址空间大小

	// 构造函数
	Symbol();
	void reset_value();	//重置所有的值
};

// 定义符号表
// 这里应该是将所有的符号全部存储到这个地方，但是，可以弄一个全局的，怎么弄局部的呢？
// 用一个map进行保存是否可行
// 符号表中的项
// 全局符号表的名字是global，局部的名字是函数的名字
class SymbolTable
{
public:
	void add_symbol(string,Symbol &,int);  // 添加符号，当为全局变量的时候，这个时候的名字是global，其它时候的名字就是函数的名字
	void pop_symbol(string);  // 将符号从符号表中移出来，通常需要移出来的情况就是分析结束了某一个部分
	Symbol search_symbol(string table,string name); // 查找符号表,通过符号的名字进行查找，传递进两个参数，一个是符号表的名字，另一个是变量的名字
	vector<Symbol> search_table(string table); // 查找一个符号表，返回vector
	void print_symbol(); // 将符号表打印出来
	

// private: 虽然这里应该封装，但是没有办法了
public:
	map<string,vector<Symbol>> symbol_table; // 符号表
	Error error_handler;
};

#endif