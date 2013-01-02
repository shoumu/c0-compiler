/*
 * 这个类主要是用来做语法分析，
 * 采用递归子程序法
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include <iostream>
#include <string>
#include <list>
#include "Error.h"
#include "TokenAnalyze.h"
#include "SymbolTable.h"
#include "Quaternion.h"

using namespace std;

class Parser
{
public:
	Parser(string);					// 构造函数
	~Parser();						// 析构函数


	void parser_sourcefile();		// 分析整个程序
	/*
	 * 分析采用递归子程序法
	 * 这里是关于各个分程序的定义
	 */
	void program();					// 程序
	void const_declar();			// 常量说明
	void const_define();			// 常量定义
	void varialbe_declar();			// 变量说明
	void variable_define();			// 变量定义
	void declar_head();				// 声明头部
	void type_identifier();			// 类型标识符
	void function_define();			// 函数定义，这里将有返回值的函数定义和无返回值的定义合并在一起了。
	void compond_statement();		// 复合语句
	void argument();				// 参数
	void argument_list();			// 参数表
	void main_function();			// 主函数
	void expression();				// 表达式
	void term();					// 项
	void factor();					// 因子
	void statement();				// 语句
	void assign_statement();		// 赋值语句
	void condition_statement();		// 条件语句
	void condition();				// 条件
	void loop_statement();			// 循环语句
	void dowhile_statement();		// do while语句循环
	void for_statement();			// for语句循环
	void func_call_statement();		// 函数调用语句
	void parameter_list();			// 值参数表
	void statement_list();			// 语句列
	void read_statement();			// 读语句
	void write_statement();			// 写语句
	void return_statement();		// 返回语句

	// 小的需要处理的部分
	void integer();					// 整数

private:
	Error error_handler;			// 错误处理
	string sourcefile;				// 源程序文件名
	TokenAnalyze analyze;			// 用来进行词法分析
	Token token;					// 存储当前的词
	SymbolTable symbol_table;		// 符号表类

	string current_table_name; 		// 用在填写符号表过程中，主要是为了保存当前的情况
	Symbol sym;						// 这个表示当前的符号
	QuaternionList quaternion_list;			// 四元式列表
	Quaternion quaternion;			// 四元式

};

#endif