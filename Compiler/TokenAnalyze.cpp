#include "TokenAnalyze.h"

// Token�ĳ�ʼ��
Token::Token() {}

Token::Token(token_type ty):type(ty)
{ }

Token::Token(string str,token_type ty,int i):token(str),type(ty),line_number(i)
{ }


// TokenAnalyze�ĳ�ʼ������
void TokenAnalyze::initialize()
{
	/********** ��ʼ���������ֵ� **********/
	// ����13��
	reservedWord.insert(make_pair("const",CONSTTK));
	reservedWord.insert(make_pair("int",INTTK));
	reservedWord.insert(make_pair("char",CHARTK));
	reservedWord.insert(make_pair("void",VOIDTK));
	reservedWord.insert(make_pair("main",MAINTK));
	reservedWord.insert(make_pair("if",IFTK));
	reservedWord.insert(make_pair("else",ELSETK));
	reservedWord.insert(make_pair("do",DOTK));
	reservedWord.insert(make_pair("while",WHILETK));
	reservedWord.insert(make_pair("for",FORTK));
	reservedWord.insert(make_pair("scanf",SCANFTK));
	reservedWord.insert(make_pair("printf",PRINTFTK));
	reservedWord.insert(make_pair("return",RETURNTK));
	/**********************************************/

	/*********** ��ʼ������ **************/
	ssym.insert(make_pair("+",PLUS));
	ssym.insert(make_pair("-",MINU));
	ssym.insert(make_pair("*",MULT));
	ssym.insert(make_pair("/",DIV));
	ssym.insert(make_pair("<",LSS));
	ssym.insert(make_pair("<=",LEQ));
	ssym.insert(make_pair(">",GRE));
	ssym.insert(make_pair(">=",GEQ));
	ssym.insert(make_pair("==",EQL));
	ssym.insert(make_pair("!=",NEQ));
	ssym.insert(make_pair("=",ASSIGN));
	ssym.insert(make_pair(";",SEMICN));
	ssym.insert(make_pair(",",COMMA));
	ssym.insert(make_pair("(",LPARENT));
	ssym.insert(make_pair(")",RPARENT));
	ssym.insert(make_pair("[",LBRACK));
	ssym.insert(make_pair("]",RBRACK));
	ssym.insert(make_pair("{",LBRACE));
	ssym.insert(make_pair("}",RBRACE));
	/***************************************/


	/******************��Ҫ��һ���£�������ǽṹû����ƺð�********************/
	token_typename.insert(make_pair(IDEN,"IDEN"));
	token_typename.insert(make_pair(INTCON,"INTCON"));
	token_typename.insert(make_pair(CHARCON,"CHARCON"));
	token_typename.insert(make_pair(STRCON,"STRCON"));
	token_typename.insert(make_pair(CONSTTK,"CONSTTK"));
	token_typename.insert(make_pair(INTTK,"INTTK"));
	token_typename.insert(make_pair(CHARTK,"CHARTK"));
	token_typename.insert(make_pair(VOIDTK,"VOIDTK"));
	token_typename.insert(make_pair(MAINTK,"MAINTK"));
	token_typename.insert(make_pair(IFTK,"IFTK"));
	token_typename.insert(make_pair(ELSETK,"ELSETK"));
	token_typename.insert(make_pair(DOTK,"DOTK"));
	token_typename.insert(make_pair(WHILETK,"WHILETK"));
	token_typename.insert(make_pair(FORTK,"FORTK"));
	token_typename.insert(make_pair(SCANFTK,"SCANFTK"));
	token_typename.insert(make_pair(PRINTFTK,"PRINTFTK"));
	token_typename.insert(make_pair(RETURNTK,"RETURNTK"));
	token_typename.insert(make_pair(PLUS,"PLUS"));
	token_typename.insert(make_pair(MINU,"MINU"));
	token_typename.insert(make_pair(MULT,"MULT"));
	token_typename.insert(make_pair(DIV,"DIV"));
	token_typename.insert(make_pair(LSS,"LSS"));
	token_typename.insert(make_pair(LEQ,"LEQ"));
	token_typename.insert(make_pair(GRE,"GRE"));
	token_typename.insert(make_pair(GEQ,"GEQ"));
	token_typename.insert(make_pair(EQL,"EQL"));
	token_typename.insert(make_pair(NEQ,"NEQ"));
	token_typename.insert(make_pair(ASSIGN,"ASSIGN"));
	token_typename.insert(make_pair(SEMICN,"SEMICN"));
	token_typename.insert(make_pair(COMMA,"COMMA"));
	token_typename.insert(make_pair(LPARENT,"LPARENT"));
	token_typename.insert(make_pair(RPARENT,"RPARENT"));
	token_typename.insert(make_pair(LBRACK,"LBRACK"));
	token_typename.insert(make_pair(RBRACK,"RBRACK"));
	token_typename.insert(make_pair(LBRACE,"LBRACE"));
	token_typename.insert(make_pair(RBRACE,"RBRACE"));
}


/*
//TokenAnalyse���캯������Ҫ����ɹ��ڳ�ʼ����صĹ���
TokenAnalyze::TokenAnalyze(string input)
{
	filein.open(input.c_str());
	if(!filein)
	{
		error_handler.error(1,0);  // �ļ������ڵ�ʱ�򱨴�
	}

	ofstream fileout("out.txt");
	initialize();
	line_number = 1;  // �ʼ���ļ���ʱ�򣬴��ڵ�һ��
}

*/
// ���캯��
TokenAnalyze::TokenAnalyze()
{
	initialize();
	line_number = 1;
}

// ��ȡ��һ���ʣ����ص�����ΪToken
Token TokenAnalyze::next_token()
{
	
	if(!ahead_list.empty())  // ��aheadlist���ǿյ�ʱ�򣬴������ȡ
	{
		Token temp = ahead_list.front();
		ahead_list.pop_front();
		return temp;
	}
	else if(has_tokens())  // ������tokens
	{
		return _next_token();
	}
	else
	{
		return Token("",token_type::ENDOFFILE,line_number);
	}
}

// ���Ƿ�����ļ���β
bool TokenAnalyze::has_tokens()
{
	char ch;
	while(filein.get(ch) && is_space(ch))  // ���հ�
		if(ch == '\n')
			line_number ++;
	if(filein.eof()) // �ж����Ƿ񵽴��ļ���β
	{
		return false;
	}
	else 
	{
		filein.unget();
		return true;
	}
}

// ��ǰ��ȡһ���ַ�
list<Token> TokenAnalyze::look_ahead(int num)  // �����num��ʾҪ��ȡ�ĸ���
{
	//Token temp = _next_token(); // ��ʱ����
	//ahead_list.push_back(temp);
	//return temp;
	list<Token> temp;
	int i = 0;
	for(list<Token>::iterator iter = ahead_list.begin();i < ahead_list.size() && i < num;iter ++,i ++)
	{
		temp.push_back((*iter)); // ���ｫ���е����ŵ�temp
	}

	if((i + 1)!= ahead_list.size())   // ˵��û�ж�ȡ�㹻��Token
	{
		for(;i < num;i ++)
		{
			Token tmp = _next_token();
			ahead_list.push_back(tmp);
			temp.push_back(tmp);
		}
	}
	return temp;
}

// װ���ļ�
void TokenAnalyze::load_file(string file)
{
	filein.open(file.c_str());
	if(!filein)  // ���ļ�ʧ��
	{
		error_handler.error(1,0);
	}
}


// ˽�ж�ȡ��һ������ķ���
Token TokenAnalyze::_next_token()
{
	if(!filein)  // �������򿪴���
	{
		return Token("",token_type::ENDOFFILE,line_number);
	}


	char ch;
	// ���ȹ��˿հ׷�
	while(filein.get(ch) && is_space(ch))
		if(ch == '\n') 
			line_number ++ ;

	if(is_letter(ch))
	{
		return identifier(ch);
	}
	else if(is_digit(ch))
	{
		return number(ch);
	}
	else 
	{
		switch(ch)
		{
		case '\'':
			return char_con(ch);break;
		case '"':
			return string_con(ch);break;
		case '+':
		case '-':
			return add_operator(ch);break;
		case '*':
		case '/':
			return multiply_operator(ch);break;
		case '=':
			return assign_equal_operator(ch);break;
		case '<':
		case '>':
		case '!':
			return relation_operator(ch);break;
		case ';':
			return is_semicn(ch);break;
		case ',':
			return is_comma(ch);break;
		case '(':
			return left_parent(ch);break;
		case ')':
			return right_parent(ch);break;
		case '[':
			return left_brack(ch);break;
		case ']':
			return right_brack(ch);break;
		case '{':
			return left_brace(ch);break;
		case '}':
			return right_brace(ch);break;
		default:
			return Token(""+ch,token_type::UNKNOWN,line_number);
		}
	}
}


// �жϻ�õ�һ�������Ƿ��Ǳ�����
bool TokenAnalyze::is_reseve(string str)
{
	if(reservedWord.find(str) != reservedWord.end()) 
		return true;
	else
		return false;
}

// �ж��Ƿ����ַ����е��ַ�
bool TokenAnalyze::is_str_ascii(char ch)
{
	if(ch == 32 || ch == 33 || (ch >= 35 && ch <= 126))
		return true;
	else 
		return false;
}

// �ж��Ƿ�����ĸ,�������ĸ����_,a~z,A~Z
bool TokenAnalyze::is_letter(char ch)
{
	if(ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	return false;
}

// �ж��Ƿ�������
bool TokenAnalyze::is_digit(char ch)
{
	if(ch >= '0' && ch <= '9')
		return true;
	return false;
}

// �ж��Ƿ��ǿհ׷�
bool TokenAnalyze::is_space(char ch)
{
	if(ch == ' ' || ch == '\t' || ch == '\n')
		return true;
	return false;
}

// �Ա�ʶ���Ĵ�����
Token TokenAnalyze::identifier(char ch)
{
	string value;
	do
	{
		value += ch;
	}while (filein.get(ch) && (is_letter(ch) || is_digit(ch)));
	filein.unget();  // ����һ���ַ�
	if(is_reseve(value))  // �Ǳ����ֵ�ʱ�򷵻ر����ֵ�����
		return Token(value,reservedWord[value],line_number);
	else // ���ǵ�ʱ�򷵻ر�ʶ��
		return Token(value,token_type::IDEN,line_number);
}


// �����ֵĴ���
Token TokenAnalyze::number(char ch)
{
	string value;
	do{
		value += ch;
	}while(filein.get(ch) && is_digit(ch));
	filein.unget();
	return Token(value,token_type::INTCON,line_number);
}

/*********************************************************************/

// ����һ�£���������ַ������ַ��ķ���ֱ�Ӿͷ��ش������ŵ���ʽ

/*********************************************************************/

// ���ַ����д���
Token TokenAnalyze::char_con(char ch)
{
	//string value;
	//filein.get(ch); // �ȶ���һ�����ʼ�ĵ����Ź��˵�
	//value += ch; //
	//filein.get(ch);
	//if(ch != '\'')
	//{
	//	error_handler.error(3,line_number);
	//	return Token(value,token_type::UNKNOWN,line_number);
	//}
	//else 
	//	return Token(value,token_type::CHARCON,line_number);

	string value;
	value += ch; // ���Ƚ����ݽ�����ch����
	filein.get(ch);
	value += ch;
	filein.get(ch);
	if(ch != '\'')
	{
		error_handler.error(3,line_number);
		return Token(value,token_type::UNKNOWN,line_number);
	}
	else 
	{
		value += ch;  // ���ﲻ�ö��
		return Token(value,token_type::CHARCON,line_number);
	}

}

// ���ַ����Ĵ���
Token TokenAnalyze::string_con(char ch)
{
	//string value;
	//do
	//{
	//	if(ch == '"')
	//		continue;
	//	if(ch == '\n')  // �ַ����в��ܹ��������з�
	//	{
	//		cout << "error" << endl;
	//	}
	//	value += ch;
	//}while(filein.get(ch) && ch != '"');
	//// ���ﲻ�û��ˣ���Ϊ�����ַ��������ǵ����ŵġ�
	//return Token(value,token_type::STRCON,line_number);

	string value;
	do
	{
		if(ch == '\n')  // �ַ����в��ܹ��������з�
		{
			error_handler.error(11,line_number);
		}
		value += ch;
	}while(filein.get(ch) && ch != '"');
	// ���ﲻ�û��ˣ���Ϊ�����ַ��������ǵ����ŵġ�
	value += ch;
	return Token(value,token_type::STRCON,line_number);

}

// �ӷ����㷨�Ĵ���
Token TokenAnalyze::add_operator(char ch)
{
	string value;
	value += ch;  
	if(ch == '+') // �Ӻ�
		return Token(value,token_type::PLUS,line_number);
	else if(ch == '-')  // ����
		return Token(value,token_type::MINU,line_number);
	else 
		return Token(value,token_type::UNKNOWN,line_number);
}

// �˷����������
Token TokenAnalyze::multiply_operator(char ch)
{
	string value;
	value += ch;
	if(ch == '*')  // �˺�
		return Token(value,token_type::MULT,line_number);
	else if(ch == '/') // ����
		return Token(value,token_type::DIV,line_number);
	else
		return Token(value,token_type::UNKNOWN,line_number);
}

// ���ڸ�ֵ���Ż��ߵ��ڵıȽϷ��ŵ��ж�
Token TokenAnalyze::assign_equal_operator(char ch)
{
	string value;
	value += ch;  // ������Ҫ�ж�һ���Ƿ���+=������������һ���ַ�
	filein.get(ch);
	if(ch == '=')
	{
		value += ch;
		return Token(value,token_type::EQL,line_number);
	}
	else
	{
		filein.unget();
		return Token(value,token_type::ASSIGN,line_number);
	}
}

// �Թ�ϵ��������ж�,����Ĺ�ϵ���������
// <,<=,>,>=,!=����
Token TokenAnalyze::relation_operator(char ch)
{
	string value;
	value += ch;
	if(ch == '<')
	{
		filein.get(ch);
		if(ch == '=')
		{
			value += ch;
			return Token(value,token_type::LEQ,line_number);
		}
		else
		{
			filein.unget();
			return Token(value,token_type::LSS,line_number);
		}
	}
	else if(ch == '>')
	{
		filein.get(ch);
		if(ch == '=')
		{
			value += ch;
			return Token(value,token_type::GEQ,line_number);
		}
		else
		{
			filein.unget();
			return Token(value,token_type::GRE,line_number);
		}
	}
	else  // �����ڵ����
	{
		filein.get(ch);
		if(ch == '=')
		{
			value += ch;
			return Token(value,token_type::NEQ,line_number);
		}
		else
		{
			error_handler.error(2,line_number);
			return Token(value,token_type::UNKNOWN,line_number);
		}
	}
}

// ;�Ŵ���
Token TokenAnalyze::is_semicn(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::SEMICN,line_number);
}

// ,�Ŵ���
Token TokenAnalyze::is_comma(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::COMMA,line_number);
}

// (
Token TokenAnalyze::left_parent(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::LPARENT,line_number);
}

// )
Token TokenAnalyze::right_parent(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::RPARENT,line_number);
}

// [
Token TokenAnalyze::left_brack(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::LBRACK,line_number);
}

// ]
Token TokenAnalyze::right_brack(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::RBRACK,line_number);
}

// {
Token TokenAnalyze::left_brace(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::LBRACE,line_number);
}

// }
Token TokenAnalyze::right_brace(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::RBRACE,line_number);
}