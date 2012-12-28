#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <map>
#include <iostream>

const static enum token_type   // 符号类型
{
	// 前面的一部分都是作为保留字，后面的一部分是类别，这样写主要是为了查找判断方便。
	CONSTTK,INTTK,CHARTK,VOIDTK,   
	MAINTK,IFTK,ELSETK,DOTK,WHILETK,
	FORTK,SCANFTK,PRINTFTK,RETURNTK,
	PLUS,MINU,MULT,DIV,  // 加减乘除
	LSS,LEQ,GRE,GEQ,EQL,NEQ, // 比较运算符
	ASSIGN,SEMICN,COMMA,
	LPARENT,RPARENT,LBRACK,RBRACK,LBRACE,RBRACE,

	IDEN,INTCON,CHARCON,STRCON,
	UNKNOWN, // 这一个用来表示分析到未知字符
	ENDOFFILE
};

/*
class Error
{
public:
	Error();
	// 错误处理函数
	void error(int line,int error_num);

private:
	map<int,string> error_types; // 在这个map中添加所有的错误
};

*/

#endif