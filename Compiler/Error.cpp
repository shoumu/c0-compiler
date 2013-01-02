#include <iostream>
#include <string>
#include "Error.h"

using namespace std;

Error::Error()
{
	initialize();  // ���ó�ʼ������
}

// ������������
void Error::error(int err_num,int line_num)
{
	cout << "line " << line_num << ":" <<"error " << err_num << ":" << error_types[err_num] << endl;
}

void Error::initialize()
{
	error_types.insert(make_pair(1,"�����ļ�������"));
	error_types.insert(make_pair(2,"'!='ȱ��'='"));
	error_types.insert(make_pair(3,"�ַ�ȱ���󲿵�\"'\""));
	error_types.insert(make_pair(4,"ȱ��return�ؼ���"));
	error_types.insert(make_pair(5,"ȱ��'('����"));
	error_types.insert(make_pair(6,"ȱ��')'����"));
	error_types.insert(make_pair(7,"ȱ��'['����"));
	error_types.insert(make_pair(8,"ȱ��']'����"));
	error_types.insert(make_pair(9,"ȱ��'{'����"));
	error_types.insert(make_pair(10,"ȱ��'}'����"));
	error_types.insert(make_pair(11,"�ַ����е��������ű�����ͬһ�д���"));
	error_types.insert(make_pair(12,"д���Ӧ����printf��ʼ"));
	error_types.insert(make_pair(13,"д������ַ����������ӷ��Ų��Ϸ�"));
	error_types.insert(make_pair(14,"�����Ӧ����scanf��ʼ"));
	error_types.insert(make_pair(15,"Ӧ��Ϊ��ʶ��"));
	error_types.insert(make_pair(16,"Ӧ��Ϊconst"));
	error_types.insert(make_pair(17,"Ӧ�ԷֺŽ�β"));
	error_types.insert(make_pair(18,"��������ֻ�ܹ���int����char��ʼ"));
	error_types.insert(make_pair(19,"Ӧ���Ǹ�ֵ����="));
	error_types.insert(make_pair(20,"Ӧ����һ������"));
	error_types.insert(make_pair(21,"���ܹ���ʾ����"));
	error_types.insert(make_pair(22,"Ӧ����һ���ַ�����"));
	error_types.insert(make_pair(23,"���Ϸ�������ͷ��"));
	error_types.insert(make_pair(24,"���ͱ�ʶ��ֻ�ܹ�ʹint����char"));
	error_types.insert(make_pair(25,"Ӧ�������0������"));
	error_types.insert(make_pair(26,"Ӧ����int��char����void��ʼ"));
	error_types.insert(make_pair(27,"����������ֵ����ֻ�ܹ�ʹvoid"));
	error_types.insert(make_pair(28,"��������ֻ�ܹ���main"));
	error_types.insert(make_pair(29,"����ȷ�ĸ�ֵ���"));
	error_types.insert(make_pair(30,"ȱ��if"));
	error_types.insert(make_pair(31,"ȱ��do�ؼ���"));
	error_types.insert(make_pair(32,"ȱ��while�ؼ���"));
	error_types.insert(make_pair(33,"ȱ��for�ؼ���"));
	error_types.insert(make_pair(34,"ȱ��;"));
	error_types.insert(make_pair(35,"ȱ�ټӷ������+/-"));
	error_types.insert(make_pair(36,"����ȷ�Ŀ�ʼ����"));
	error_types.insert(make_pair(37,"ȱ��=���ߣ�"));
	error_types.insert(make_pair(38,"�Ѵ��ڱ�����"));
}