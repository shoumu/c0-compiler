#include "TokenAnalyze.h"

// Token的初始化
Token::Token() {}

Token::Token(token_type ty):type(ty)
{ }

Token::Token(string str,token_type ty,int i):token(str),type(ty),line_number(i)
{ }


// TokenAnalyze的初始化工作
void TokenAnalyze::initialize()
{
	/********** 初始化保留字字典 **********/
	// 共有13个
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

	/*********** 初始化符号 **************/
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


	/******************又要费一次事，这个就是结构没有设计好啊********************/
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
//TokenAnalyse构造函数，主要是完成关于初始化相关的工作
TokenAnalyze::TokenAnalyze(string input)
{
	filein.open(input.c_str());
	if(!filein)
	{
		error_handler.error(1,0);  // 文件不存在的时候报错
	}

	ofstream fileout("out.txt");
	initialize();
	line_number = 1;  // 最开始打开文件的时候，处于第一行
}

*/
// 构造函数
TokenAnalyze::TokenAnalyze()
{
	initialize();
	line_number = 1;
}

// 获取下一个词，返回的类型为Token
Token TokenAnalyze::next_token()
{
	
	if(!ahead_list.empty())  // 当aheadlist不是空的时候，从这里读取
	{
		Token temp = ahead_list.front();
		ahead_list.pop_front();
		return temp;
	}
	else if(has_tokens())  // 还存在tokens
	{
		return _next_token();
	}
	else
	{
		return Token("",token_type::ENDOFFILE,line_number);
	}
}

// 看是否读到文件结尾
bool TokenAnalyze::has_tokens()
{
	char ch;
	while(filein.get(ch) && is_space(ch))  // 读空白
		if(ch == '\n')
			line_number ++;
	if(filein.eof()) // 判断是是否到达文件结尾
	{
		return false;
	}
	else 
	{
		filein.unget();
		return true;
	}
}

// 超前读取一个字符
list<Token> TokenAnalyze::look_ahead(int num)  // 这里的num表示要读取的个数
{
	//Token temp = _next_token(); // 临时保存
	//ahead_list.push_back(temp);
	//return temp;
	list<Token> temp;
	int i = 0;
	for(list<Token>::iterator iter = ahead_list.begin();i < ahead_list.size() && i < num;iter ++,i ++)
	{
		temp.push_back((*iter)); // 这里将所有的量放到temp
	}

	if((i + 1)!= ahead_list.size())   // 说明没有读取足够的Token
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

// 装载文件
void TokenAnalyze::load_file(string file)
{
	filein.open(file.c_str());
	if(!filein)  // 打开文件失败
	{
		error_handler.error(1,0);
	}
}


// 私有读取下一个词语的方法
Token TokenAnalyze::_next_token()
{
	if(!filein)  // 输入流打开错误
	{
		return Token("",token_type::ENDOFFILE,line_number);
	}


	char ch;
	// 首先过滤空白符
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


// 判断获得的一个词语是否是保留字
bool TokenAnalyze::is_reseve(string str)
{
	if(reservedWord.find(str) != reservedWord.end()) 
		return true;
	else
		return false;
}

// 判断是否是字符串中的字符
bool TokenAnalyze::is_str_ascii(char ch)
{
	if(ch == 32 || ch == 33 || (ch >= 35 && ch <= 126))
		return true;
	else 
		return false;
}

// 判断是否是字母,这里的字母包括_,a~z,A~Z
bool TokenAnalyze::is_letter(char ch)
{
	if(ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	return false;
}

// 判断是否是数字
bool TokenAnalyze::is_digit(char ch)
{
	if(ch >= '0' && ch <= '9')
		return true;
	return false;
}

// 判断是否是空白符
bool TokenAnalyze::is_space(char ch)
{
	if(ch == ' ' || ch == '\t' || ch == '\n')
		return true;
	return false;
}

// 对标识符的处理部分
Token TokenAnalyze::identifier(char ch)
{
	string value;
	do
	{
		value += ch;
	}while (filein.get(ch) && (is_letter(ch) || is_digit(ch)));
	filein.unget();  // 回退一个字符
	if(is_reseve(value))  // 是保留字的时候返回保留字的类型
		return Token(value,reservedWord[value],line_number);
	else // 不是的时候返回标识符
		return Token(value,token_type::IDEN,line_number);
}


// 对数字的处理
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

// 考虑一下，这里对于字符串和字符的返回直接就返回带有引号的形式

/*********************************************************************/

// 对字符进行处理
Token TokenAnalyze::char_con(char ch)
{
	//string value;
	//filein.get(ch); // 先读入一个，最开始的单引号过滤掉
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
	value += ch; // 首先将传递进来的ch加入
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
		value += ch;  // 这里不用多读
		return Token(value,token_type::CHARCON,line_number);
	}

}

// 对字符串的处理
Token TokenAnalyze::string_con(char ch)
{
	//string value;
	//do
	//{
	//	if(ch == '"')
	//		continue;
	//	if(ch == '\n')  // 字符串中不能够包含换行符
	//	{
	//		cout << "error" << endl;
	//	}
	//	value += ch;
	//}while(filein.get(ch) && ch != '"');
	//// 这里不用回退，因为对于字符串后面是单引号的。
	//return Token(value,token_type::STRCON,line_number);

	string value;
	do
	{
		if(ch == '\n')  // 字符串中不能够包含换行符
		{
			error_handler.error(11,line_number);
		}
		value += ch;
	}while(filein.get(ch) && ch != '"');
	// 这里不用回退，因为对于字符串后面是单引号的。
	value += ch;
	return Token(value,token_type::STRCON,line_number);

}

// 加法运算法的处理
Token TokenAnalyze::add_operator(char ch)
{
	string value;
	value += ch;  
	if(ch == '+') // 加号
		return Token(value,token_type::PLUS,line_number);
	else if(ch == '-')  // 减号
		return Token(value,token_type::MINU,line_number);
	else 
		return Token(value,token_type::UNKNOWN,line_number);
}

// 乘法运算符处理
Token TokenAnalyze::multiply_operator(char ch)
{
	string value;
	value += ch;
	if(ch == '*')  // 乘号
		return Token(value,token_type::MULT,line_number);
	else if(ch == '/') // 除号
		return Token(value,token_type::DIV,line_number);
	else
		return Token(value,token_type::UNKNOWN,line_number);
}

// 对于赋值符号或者等于的比较符号的判断
Token TokenAnalyze::assign_equal_operator(char ch)
{
	string value;
	value += ch;  // 这里需要判断一下是否是+=操作符，读入一个字符
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

// 对关系运算符的判断,这里的关系运算符包括
// <,<=,>,>=,!=五种
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
	else  // 不等于的情况
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

// ;号处理
Token TokenAnalyze::is_semicn(char ch)
{
	string value;
	value += ch;
	return Token(value,token_type::SEMICN,line_number);
}

// ,号处理
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