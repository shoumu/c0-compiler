#include "SymbolTable.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Symbol�Ĺ��캯����������Ҫ�ǽ�if_func ��is_const��Ϊ0;
Symbol::Symbol()
{
	is_return = false;
	kind = symbol_kind::NK; // ��ʼ����ʱ��ֵΪһ����֪���Ķ���
	type = symbol_type::NT; // ��ʼ����ʱ���ܹ�Ϊ������
	name = "const_sym";
	str_value[0] = '\0'; // ��ʼ��Ϊ���ַ���
	int_value = -1;
	char_value = -1;
	array_len = 0;
	address = 0;
	space_size = 0;
}

// ����Symbol�е�ֵ
void Symbol::reset_value()
{
	str_value[0] = '\0'; // ��ʼ��Ϊ���ַ���
	int_value = -1;
	char_value = -1;
	array_len = 0;
	address = 0;
	space_size = 0;
}

// �����ű��������Ŀ
void SymbolTable::add_symbol(string name,Symbol &sym,int line_num)
{
	map<string,vector<Symbol>>::iterator iter = symbol_table.find(name);
	if(iter != symbol_table.end())  //����ܹ��ڷ��ű����ҵ�
	{
		for(vector<Symbol>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end();iter1 ++)
		{
			if(iter1->name == sym.name) //��ʾ�������Ѿ�����
			{
				error_handler.error(38,line_num);
				return ;
			}
		}
		symbol_table[name].push_back(sym);
	}
	else   // ������Ų��ܹ��ڷ��ű����ҵ�
	{
		vector<Symbol> temp;
		temp.push_back(sym);  // ���ȴ���һ��vector����sym�����ȥ
		symbol_table.insert(make_pair(name,temp));  // �ڷ��ű������
	}

	//add_symbol�Ժ�sym���е�ֵ����Ϣ�óɳ�ʼ��ʱ������
	sym.reset_value(); //����

	return ;
}

// ɾ�����ű�����Ŀ
void SymbolTable::pop_symbol(string name)
{
	for(map<string,vector<Symbol>>::iterator iter = symbol_table.begin();iter != symbol_table.end();iter ++)
	{
		if(iter->first == name)   // ����ҵ��������
		{
			symbol_table.erase(iter);
		}
	}
}

// ����һ�����ű��е�һ��Ԫ��
Symbol SymbolTable::search_symbol(string table,string name)
{
	vector<Symbol> temp;
	for(map<string,vector<Symbol>>:: iterator iter = symbol_table.begin();iter != symbol_table.end();iter ++)
	{
		if(iter ->first == table)
		{
			temp = iter->second;
			for(vector<Symbol>::iterator iter2 = temp.begin();iter2 != temp.end();iter2 ++)
			{
				if(iter2->name == name)
				{
					return (*iter2);  // ������������
				}
			}
			break;
		}
	}
	return Symbol();
}

// ����һ�����ű�
vector<Symbol> SymbolTable::search_table(string table)
{
	for(map<string,vector<Symbol>>:: iterator iter = symbol_table.begin();iter != symbol_table.end();iter ++)
	{
		if(iter ->first == table)
		{
			return iter->second;
		}
	}
	return vector<Symbol>();
}

void SymbolTable::print_symbol()
{
	map<string,vector<Symbol>>::iterator iter;
	for(iter = symbol_table.begin();iter != symbol_table.end();iter ++)
	{
		vector<Symbol>::iterator iter2;
		for(iter2 = iter->second.begin();iter2 != iter->second.end();iter2 ++)
		{
			cout<<setw(15) << iter->first << setw(15) << iter2->kind << setw(15) << iter2->name << setw(15) << iter2->int_value <<endl;
		}

		cout << "==============================================" << endl;
	}
}