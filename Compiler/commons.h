#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <map>
#include <iostream>

const static enum token_type   // ��������
{
	// ǰ���һ���ֶ�����Ϊ�����֣������һ�������������д��Ҫ��Ϊ�˲����жϷ��㡣
	CONSTTK,INTTK,CHARTK,VOIDTK,   
	MAINTK,IFTK,ELSETK,DOTK,WHILETK,
	FORTK,SCANFTK,PRINTFTK,RETURNTK,
	PLUS,MINU,MULT,DIV,  // �Ӽ��˳�
	LSS,LEQ,GRE,GEQ,EQL,NEQ, // �Ƚ������
	ASSIGN,SEMICN,COMMA,
	LPARENT,RPARENT,LBRACK,RBRACK,LBRACE,RBRACE,

	IDEN,INTCON,CHARCON,STRCON,
	UNKNOWN, // ��һ��������ʾ������δ֪�ַ�
	ENDOFFILE
};

/*
class Error
{
public:
	Error();
	// ��������
	void error(int line,int error_num);

private:
	map<int,string> error_types; // �����map��������еĴ���
};

*/

#endif