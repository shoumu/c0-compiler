/*
 * �������Ҫ���������﷨������
 * ���õݹ��ӳ���
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
	Parser(string);					// ���캯��
	~Parser();						// ��������


	void parser_sourcefile();		// ������������
	/*
	 * �������õݹ��ӳ���
	 * �����ǹ��ڸ����ֳ���Ķ���
	 */
	void program();					// ����
	void const_declar();			// ����˵��
	void const_define();			// ��������
	void varialbe_declar();			// ����˵��
	void variable_define();			// ��������
	void declar_head();				// ����ͷ��
	void type_identifier();			// ���ͱ�ʶ��
	void function_define();			// �������壬���ｫ�з���ֵ�ĺ���������޷���ֵ�Ķ���ϲ���һ���ˡ�
	void compond_statement();		// �������
	void argument();				// ����
	void argument_list();			// ������
	void main_function();			// ������
	void expression();				// ���ʽ
	void term();					// ��
	void factor();					// ����
	void statement();				// ���
	void assign_statement();		// ��ֵ���
	void condition_statement();		// �������
	void condition();				// ����
	void loop_statement();			// ѭ�����
	void dowhile_statement();		// do while���ѭ��
	void for_statement();			// for���ѭ��
	void func_call_statement();		// �����������
	void parameter_list();			// ֵ������
	void statement_list();			// �����
	void read_statement();			// �����
	void write_statement();			// д���
	void return_statement();		// �������

	// С����Ҫ����Ĳ���
	void integer();					// ����

private:
	Error error_handler;			// ������
	string sourcefile;				// Դ�����ļ���
	TokenAnalyze analyze;			// �������дʷ�����
	Token token;					// �洢��ǰ�Ĵ�
	SymbolTable symbol_table;		// ���ű���

	string current_table_name; 		// ������д���ű�����У���Ҫ��Ϊ�˱��浱ǰ�����
	Symbol sym;						// �����ʾ��ǰ�ķ���
	QuaternionList quaternion_list;			// ��Ԫʽ�б�
	Quaternion quaternion;			// ��Ԫʽ

};

#endif