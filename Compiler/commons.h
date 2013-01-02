#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <map>
#include <iostream>

const static enum token_type   //词法分析是词的类型
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
	ENDOFFILE  // 表示到达文件结尾
};

// 定义符号的类型
const static enum symbol_kind
{
	CONST,			// 常量
	VARIABLE,		// 变量
	ARRAY,			// 这里特别加入一个array的类型，方便区分，并且array只有可能是变量
	FUNCTION,		// 函数
	PARAM,			// 参数
	NK				// 未知
};

const static enum symbol_type
{
	INT,
	CHAR,
	NT
};

#endif