#include <iostream>
#include <string>
#include "Quaternion.h"

using namespace std;

// 构造函数
Quaternion::Quaternion()
{}

Quaternion::Quaternion(int ind,string str,Symbol a1,Symbol a2,Symbol res):
	index(ind),op(str),arg1(a1),arg2(a2),result(res) {}

Quaternion::Quaternion(int ind,string str,Symbol a1,Symbol a2,int i):
	index(ind),op(str),arg1(a1),arg2(a2),label(i) {}

Quaternion::Quaternion(int ind,string str,int i):
	index(ind),op(str),label(i){}


// 析构函数
Quaternion::~Quaternion()
{}



/*
 * QuaternionList
 */

QuaternionList::QuaternionList()
{
	index = 1;
}

void QuaternionList::add_quaternion(Quaternion qua)
{
	quaternion_list.push_back(qua);
}

int QuaternionList::get_cur_index()
{
	return index ++;
}


