// 这个类和四元式的生成相关

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <iostream>
#include <fstream>
#include "SymbolTable.h"

using namespace std;

/*
 * 四元式
 * 四元式需要怎么设计呢？？？
 */
class Quaternion
{
public:
	
	Quaternion();
	// 注意，如果是不存在符号的时候，这里就将Symbol置为const_sym
	Quaternion(int,string,Symbol,Symbol,Symbol);
	Quaternion(int,string,Symbol,Symbol,int); 
	Quaternion(int,string,int);

	~Quaternion();

public:
//private:
	const Symbol const_sym;

	int index;	// 表示行号
	string op; // 操作符
	Symbol arg1; // 操作数1
	Symbol arg2; // 操作数2
	Symbol result;	// 操作数3
	int label;	// 跳转的时候使用，这里的labl对应的值为index
};

// 在这里定义四元式有下面的一些类型：
/********************四元式

ASSIGN	arg1	--		result	||	result = arg1
MUL		arg1	arg2	result	||	result = arg1 + arg2
SUB		arg1	arg2	result	||	result = arg1 - arg2
MUL		arg1	arg2	result	||	result = arg1 * arg2
DIV		arg1	arg2	result	||	result = arg1 / arg2
LABEL
JA		arg1	arg2	label	||	if(arg1 > arg2) goto label
JB		arg1	arg2	label	||	if(arg1 < arg2) goto label
JE		arg1	arg2	label	||	if(arg1 == arg2) goto label
JNE		arg1	arg2	label	||	if(arg1 != arg2) goto label
JAE		arg1	arg2	label	||	if(arg1 >= arg2) goto label
JBE		arg1	arg2	label	||	if(arg1 <= arg2) goto label
MINUS	arg1	--		result	||	result = -arg1
GOTO	--		--		label	||	goto label
PARA	arg1	--		--		||	arg1作为参数传递
RETURN	arg1	--		--		||	arg1作为返回值返回
CALL	arg1	--		--		||	调用函数arg1
SCANF	--		--		result	||	读入数据到result
PRINTF	arg1	--		--		||	输出arg1，arg1为字符串或者表达式
PRINTF	arg1	arg2	--		||	输出arg1和arg2到控制台，arg1为字符串，arg2为表达式

*/

/*
 * 生成四元式的时候的临时变量的表示为：$ti (i是号)
 */

// 这里保存所有的四元式，用一个list的形式保存
class QuaternionList
{
public:
	QuaternionList();
	void add_quaternion(Quaternion);
	int get_cur_index(); // 获取当前的可以填的行号

private:
	list<Quaternion> quaternion_list;
	int index;
};

#endif