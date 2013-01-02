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

// ���嵥���ķ���
struct Symbol
{
	symbol_kind kind; //��ʾ����
	symbol_type type;	// �������int��char֮���
	string name;  // �����ʾ���ŵ����֣������ַ�����

	// ֵ���
	char str_value[50]; // �ַ���������ֵ
	int int_value; // ���ε�ֵ
	char char_value; // �����洢�ַ�������ֵ
	int array_len; // �����������������ʱ������������������ĳ���
	bool is_return; // ��ʾ�Ƿ��Ƿ���ֵ

	// ��ַ���
	int address; // ��ַ
	unsigned int space_size; // ��ַ�ռ��С

	// ���캯��
	Symbol();
	void reset_value();	//�������е�ֵ
};

// ������ű�
// ����Ӧ���ǽ����еķ���ȫ���洢������ط������ǣ�����Ūһ��ȫ�ֵģ���ôŪ�ֲ����أ�
// ��һ��map���б����Ƿ����
// ���ű��е���
// ȫ�ַ��ű��������global���ֲ��������Ǻ���������
class SymbolTable
{
public:
	void add_symbol(string,Symbol &,int);  // ��ӷ��ţ���Ϊȫ�ֱ�����ʱ�����ʱ���������global������ʱ������־��Ǻ���������
	void pop_symbol(string);  // �����Ŵӷ��ű����Ƴ�����ͨ����Ҫ�Ƴ�����������Ƿ���������ĳһ������
	Symbol search_symbol(string table,string name); // ���ҷ��ű�,ͨ�����ŵ����ֽ��в��ң����ݽ�����������һ���Ƿ��ű�����֣���һ���Ǳ���������
	vector<Symbol> search_table(string table); // ����һ�����ű�����vector
	void print_symbol(); // �����ű��ӡ����
	

// private: ��Ȼ����Ӧ�÷�װ������û�а취��
public:
	map<string,vector<Symbol>> symbol_table; // ���ű�
	Error error_handler;
};

#endif