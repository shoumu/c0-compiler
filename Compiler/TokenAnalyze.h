#ifndef __TOKENANALYZE_H__
#define __TOKENANALYZE_H__

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <list>
#include "commons.h"
#include "Error.h"

using namespace std;

/*
 * 这个类主要是用来存储表示一个词，我们需要获得的是词的基本的一些属性
 */
class Token
{
public:
	Token(string str,token_type ty,int num);
	Token(token_type ty);
	Token();

	string get_value() const {return token;}
	token_type get_type() const {return type;}
	int get_linenumber() const {return line_number;}


private:

	string token; // 用来存储token的值
	token_type type; // 用来存储token的类型
	int line_number; // 用来存储token所在的行
};


/*
 * 定义词法分析的类，从这个类中能够得到每个词
 */
class TokenAnalyze
{
public:
	//TokenAnalyze(string input);  // 输入流
	TokenAnalyze();
	Token next_token();  // 用这个方法获取下一个token
	list<Token> look_ahead(int);  // 超前读取
	void load_file(string filename);  // 载入文件

	map<token_type,string> token_typename; // 这一个主要是token的名字对应的字符串，主要是为了词法分析的输出方便而已

private:
	ifstream filein;  // 文件流

	// 下面是四个map，主要是和保留字符号相关
	map<string,token_type> reservedWord; // 保留字
	map<string,token_type> ssym; // 符号
	list<Token> ahead_list; // 用来存储超前读取的部分

	// error
	Error error_handler;

	//map<token_type,string> token_typename; // 这一个主要是token的名字对应的字符串，主要是为了词法分析的输出方便而已

	int line_number;

	Token _next_token(); // 这里定义一个私有方法，主要是为了后面的调用，后面想要加入一个能够预读的方法，这里加入了这个方法以后就能够通过这个方法的调用完成。
	bool has_tokens();  // 用来判断是否还含有token

	void initialize(); // 初始化一些数据
	bool is_reseve(string str);  // 判断是否是保留字
	bool is_str_ascii(char ch);  // 判断是否是字符串中的字符
	bool is_letter(char ch);  // 判断是否是字符
	bool is_digit(char); // 判断是否是数字
	bool is_space(char); // 判断是否是空白符
	 
	// 对各个子模块进行处理
	Token number(char); // 数字
	Token identifier(char); // 标识符
	Token char_con(char); // 字符
	Token string_con(char); // 字符串
	Token add_operator(char); // 加法运算符
	Token multiply_operator(char); // 乘法运算符
	Token assign_equal_operator(char); // 赋值符号或者关系等
	Token relation_operator(char); // 关系运算符
	Token is_semicn(char); // ;号
	Token is_comma(char); // ,号
	Token left_parent(char); // (
	Token right_parent(char); // )
	Token left_brack(char); // [
	Token right_brack(char); // ]
	Token left_brace(char); // {
	Token right_brace(char); // }
};

#endif