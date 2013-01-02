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
 * �������Ҫ�������洢��ʾһ���ʣ�������Ҫ��õ��ǴʵĻ�����һЩ����
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

	string token; // �����洢token��ֵ
	token_type type; // �����洢token������
	int line_number; // �����洢token���ڵ���
};


/*
 * ����ʷ��������࣬����������ܹ��õ�ÿ����
 */
class TokenAnalyze
{
public:
	//TokenAnalyze(string input);  // ������
	TokenAnalyze();
	Token next_token();  // �����������ȡ��һ��token
	list<Token> look_ahead(int);  // ��ǰ��ȡ
	void load_file(string filename);  // �����ļ�

	map<token_type,string> token_typename; // ��һ����Ҫ��token�����ֶ�Ӧ���ַ�������Ҫ��Ϊ�˴ʷ�����������������

private:
	ifstream filein;  // �ļ���

	// �������ĸ�map����Ҫ�Ǻͱ����ַ������
	map<string,token_type> reservedWord; // ������
	map<string,token_type> ssym; // ����
	list<Token> ahead_list; // �����洢��ǰ��ȡ�Ĳ���

	// error
	Error error_handler;

	//map<token_type,string> token_typename; // ��һ����Ҫ��token�����ֶ�Ӧ���ַ�������Ҫ��Ϊ�˴ʷ�����������������

	int line_number;

	Token _next_token(); // ���ﶨ��һ��˽�з�������Ҫ��Ϊ�˺���ĵ��ã�������Ҫ����һ���ܹ�Ԥ���ķ����������������������Ժ���ܹ�ͨ����������ĵ�����ɡ�
	bool has_tokens();  // �����ж��Ƿ񻹺���token

	void initialize(); // ��ʼ��һЩ����
	bool is_reseve(string str);  // �ж��Ƿ��Ǳ�����
	bool is_str_ascii(char ch);  // �ж��Ƿ����ַ����е��ַ�
	bool is_letter(char ch);  // �ж��Ƿ����ַ�
	bool is_digit(char); // �ж��Ƿ�������
	bool is_space(char); // �ж��Ƿ��ǿհ׷�
	 
	// �Ը�����ģ����д���
	Token number(char); // ����
	Token identifier(char); // ��ʶ��
	Token char_con(char); // �ַ�
	Token string_con(char); // �ַ���
	Token add_operator(char); // �ӷ������
	Token multiply_operator(char); // �˷������
	Token assign_equal_operator(char); // ��ֵ���Ż��߹�ϵ��
	Token relation_operator(char); // ��ϵ�����
	Token is_semicn(char); // ;��
	Token is_comma(char); // ,��
	Token left_parent(char); // (
	Token right_parent(char); // )
	Token left_brack(char); // [
	Token right_brack(char); // ]
	Token left_brace(char); // {
	Token right_brace(char); // }
};

#endif