#include "SymbolTable.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

// Symbol的构造函数，这里主要是将if_func 和is_const置为0;
Symbol::Symbol()
{
	is_return = false;
	kind = symbol_kind::NK; // 初始化的时候赋值为一个不知道的东西
	type = symbol_type::NT; // 初始化的时候不能够为其它的
	name = "const_sym";
	str_value[0] = '\0'; // 初始化为空字符串
	int_value = -1;
	char_value = -1;
	array_len = 0;
	address = 0;
	space_size = 0;
}

// 重置Symbol中的值
void Symbol::reset_value()
{
	str_value[0] = '\0'; // 初始化为空字符串
	int_value = -1;
	char_value = -1;
	array_len = 0;
	address = 0;
	space_size = 0;
}

// 往符号表中添加项目
void SymbolTable::add_symbol(string name,Symbol &sym,int line_num)
{
	map<string,vector<Symbol>>::iterator iter = symbol_table.find(name);
	if(iter != symbol_table.end())  //如果能够在符号表中找到
	{
		for(vector<Symbol>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end();iter1 ++)
		{
			if(iter1->name == sym.name) //表示变量名已经存在
			{
				error_handler.error(38,line_num);
				return ;
			}
		}
		symbol_table[name].push_back(sym);
	}
	else   // 如果符号不能够在符号表中找到
	{
		vector<Symbol> temp;
		temp.push_back(sym);  // 首先创建一个vector，将sym插入进去
		symbol_table.insert(make_pair(name,temp));  // 在符号表中添加
	}

	//add_symbol以后将sym所有的值的信息置成初始化时候的情况
	sym.reset_value(); //重置

	return ;
}

// 删除符号表中项目
void SymbolTable::pop_symbol(string name)
{
	for(map<string,vector<Symbol>>::iterator iter = symbol_table.begin();iter != symbol_table.end();iter ++)
	{
		if(iter->first == name)   // 如果找到这个名字
		{
			symbol_table.erase(iter);
		}
	}
}

// 查找一个符号表中的一个元素
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
					return (*iter2);  // 返回整个符号
				}
			}
			break;
		}
	}
	return Symbol();
}

// 查找一个符号表
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