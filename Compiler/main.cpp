#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "TokenAnalyze.h"

using namespace std;

int main()
{
	string sourcefile;
	cout << "ÇëÊäÈëÔ´³ÌÐò£º";
	cin >> sourcefile;
	TokenAnalyze analyze(sourcefile);
	int i = 1;
	while(true)
	{
		
		Token temp = analyze.next_token();
		if(temp.get_type() == token_type::ENDOFFILE)
			break;

		if(i == 6)
		{
			Token temp1 = analyze.look_ahead();
			Token temp2 = analyze.look_ahead();
			/*cout << setw(4) << i ++ ;
			cout << setw(15)<< temp1.get_value() << "  " << setw(10) << analyze.token_typename[temp1.get_type()] <<
				"  " << setw(10) << temp1.get_linenumber() << endl;
			cout << setw(4) << i ++ ;
			cout << setw(15)<< temp2.get_value() << "  " << setw(10) << analyze.token_typename[temp2.get_type()] <<
				"  " << setw(10) << temp2.get_linenumber() << endl;*/
		}
		cout << setw(4) << i ++ ;
		cout << setw(15)<< temp.get_value() << "  " << setw(10) << analyze.token_typename[temp.get_type()] <<
			"  " << setw(10) << temp.get_linenumber() << endl;
	}
	return 0;
}