#ifndef __COMMONS_H__
#define __COMMONS_H__

#include <map>
#include <iostream>

const static enum token_type   //�ʷ������Ǵʵ�����
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
	ENDOFFILE  // ��ʾ�����ļ���β
};

// ������ŵ�����
const static enum symbol_kind
{
	CONST,			// ����
	VARIABLE,		// ����
	ARRAY,			// �����ر����һ��array�����ͣ��������֣�����arrayֻ�п����Ǳ���
	FUNCTION,		// ����
	PARAM,			// ����
	NK				// δ֪
};

const static enum symbol_type
{
	INT,
	CHAR,
	NT
};

#endif