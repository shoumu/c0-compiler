// ��������Ԫʽ���������

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include <iostream>
#include <fstream>
#include "SymbolTable.h"

using namespace std;

/*
 * ��Ԫʽ
 * ��Ԫʽ��Ҫ��ô����أ�����
 */
class Quaternion
{
public:
	
	Quaternion();
	// ע�⣬����ǲ����ڷ��ŵ�ʱ������ͽ�Symbol��Ϊconst_sym
	Quaternion(int,string,Symbol,Symbol,Symbol);
	Quaternion(int,string,Symbol,Symbol,int); 
	Quaternion(int,string,int);

	~Quaternion();

public:
//private:
	const Symbol const_sym;

	int index;	// ��ʾ�к�
	string op; // ������
	Symbol arg1; // ������1
	Symbol arg2; // ������2
	Symbol result;	// ������3
	int label;	// ��ת��ʱ��ʹ�ã������labl��Ӧ��ֵΪindex
};

// �����ﶨ����Ԫʽ�������һЩ���ͣ�
/********************��Ԫʽ

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
PARA	arg1	--		--		||	arg1��Ϊ��������
RETURN	arg1	--		--		||	arg1��Ϊ����ֵ����
CALL	arg1	--		--		||	���ú���arg1
SCANF	--		--		result	||	�������ݵ�result
PRINTF	arg1	--		--		||	���arg1��arg1Ϊ�ַ������߱��ʽ
PRINTF	arg1	arg2	--		||	���arg1��arg2������̨��arg1Ϊ�ַ�����arg2Ϊ���ʽ

*/

/*
 * ������Ԫʽ��ʱ�����ʱ�����ı�ʾΪ��$ti (i�Ǻ�)
 */

// ���ﱣ�����е���Ԫʽ����һ��list����ʽ����
class QuaternionList
{
public:
	QuaternionList();
	void add_quaternion(Quaternion);
	int get_cur_index(); // ��ȡ��ǰ�Ŀ�������к�

private:
	list<Quaternion> quaternion_list;
	int index;
};

#endif