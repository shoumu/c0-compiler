/*
 * Parser类的实现
 */

#include <string>
#include "Parser.h"
#include "Error.h"
#include "commons.h"

using namespace std;

// 构造函数
Parser::Parser(string source)
{
	sourcefile = source; // 传递进来要分析的文件名
	analyze.load_file(sourcefile);
	current_table_name = "global";		// 用来表明当前符号表的名字，初始化为global，为全局符号表
}

// 析构函数
Parser::~Parser()
{ 
	symbol_table.print_symbol();
}

void Parser::parser_sourcefile()
{
	
}

// 程序
void Parser::program()
{
	token = analyze.next_token();
	if(token.get_type() == token_type::CONSTTK)  // 常量说明
	{
		const_declar(); // 常量说明
	}

	list<Token> temp = analyze.look_ahead(2);// 超前读两个词进行判断
	Token temp1 = temp.front();
	temp.pop_front(); // 移出去最前面的一个
	Token temp2 = temp.front();
	temp.pop_front();

	int flag = 0; // 用来标志，上面的temp1和temp2使用没有

	if((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK) && 
		(temp1.get_type() == token_type::IDEN) && (temp2.get_type() != token_type::LPARENT))  // 这里表示变量说明，主要是判断temp2的类型，如果是(，那么就显然是函数了
	{
		varialbe_declar();
		flag = 1;
	}

	if(flag == 1)  // 如果前面已经使用了，则需要重新读取，否则不需要重新读取
	{
		list<Token> temp = analyze.look_ahead(2);// 超前读两个词进行判断
		temp1 = temp.front();
		temp.pop_front(); // 移出去最前面的一个
		temp2 = temp.front();
		temp.pop_front();
	}

	while((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK ||
		token.get_type() == token_type::VOIDTK) && (temp1.get_type() == token_type::IDEN))
	{
		function_define(); // 函数定义，可能存在多个函数定义的部分
		
		// 超前读取进行判断
		list<Token> temp = analyze.look_ahead(1);
		temp1 = temp.front();
		temp.pop_front();
	}

	main_function(); // 主函数，这里分析了就分析完了

	if(token.get_type() == token_type::ENDOFFILE)  // 分析到文件末尾
	{
		cout << "已经分析到文件末尾" << endl;
	}
}

// 常量说明
void Parser::const_declar()
{
	if(token.get_type() == token_type::CONSTTK)
	{
		token = analyze.next_token();
		const_define();
		if(token.get_type() == token_type::SEMICN)
		{
			token = analyze.next_token();
			while(token.get_type() == token_type::CONSTTK)  // 常量说明部分包含多个常量定义语句
			{
				token = analyze.next_token();
				const_define();
				if(token.get_type() == token_type::SEMICN)
				{
					token = analyze.next_token();

					// test
					cout << "finish 常量说明" << endl;
				}
				else 
				{
					error_handler.error(17,token.get_linenumber());
				}
			}
		}
		else
		{
			error_handler.error(17,token.get_linenumber());
		}
	}
	else 
	{
		error_handler.error(16,token.get_linenumber());
	}
}

// 常量定义
void Parser::const_define()
{
	sym.kind = symbol_kind::CONST;

	if(token.get_type() == token_type::INTTK)
	{
		token = analyze.next_token();

		/********************符号表*********************/
		sym.type = symbol_type::INT;

		if(token.get_type() == token_type::IDEN)
		{
			/********************符号表*********************/
			sym.name = token.get_value();

			token = analyze.next_token();
			if(token.get_type() == token_type::ASSIGN)
			{
				// 这里需要处理整数
				token = analyze.next_token();
				integer(); // 处理整数

				// 这里填写符号表
				symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

				while(token.get_type() == token_type::COMMA)   // 处理有多个常量定义的情况
				{
					token = analyze.next_token();
					if(token.get_type() == token_type::IDEN)
					{
						sym.name = token.get_value();

						token = analyze.next_token();
						if(token.get_type() == token_type::ASSIGN)
						{
							token = analyze.next_token();
							integer();

							// 这里填写符号表
							symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

							//token = analyze.next_token();

						}
						else
						{
							error_handler.error(19,token.get_linenumber());  
						}
					}
					else
					{
						error_handler.error(15,token.get_linenumber());
					}
				}
			}
			else 
			{
				error_handler.error(19,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(15,token.get_linenumber());
		}
	}

	// 对于字符型的处理
	else if(token.get_type() == token_type::CHARTK)
	{
		sym.type = symbol_type::CHAR; // 符号类型为字符常量

		token = analyze.next_token();
		if(token.get_type() == token_type::IDEN)
		{
			// symbol_table
			sym.name = token.get_value();

			token = analyze.next_token();
			if(token.get_type() == token_type::ASSIGN)
			{
				token = analyze.next_token();
				if(token.get_type() == token_type::CHARCON)
				{
					sym.char_value = token.get_value()[1];
					symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

					token = analyze.next_token();
					while(token.get_type() == token_type::COMMA)  // 这里处理可能有多个常量定义的地方
					{
						token = analyze.next_token();
						if(token.get_type() == token_type::IDEN)
						{
							sym.name = token.get_value();

							token = analyze.next_token();
							if(token.get_type() == token_type::ASSIGN)
							{
								token = analyze.next_token();
								if(token.get_type() == token_type::CHARCON)
								{
									sym.char_value = token.get_value()[1];
									symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

									token = analyze.next_token();
								}
								else
								{
									error_handler.error(22,token.get_linenumber());
								}
							}
							else
							{
								error_handler.error(19,token.get_linenumber());
							}
						}
						else
						{
							error_handler.error(15,token.get_linenumber());
						}
					}
				}
				else
				{
					error_handler.error(22,token.get_linenumber());
				}
			}
			else 
			{
				error_handler.error(19,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(15,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(18,token.get_linenumber());
	}
}

// 变量说明
void Parser::varialbe_declar()
{
	variable_define();
	if(token.get_type() == token_type::SEMICN)
	{
		token = analyze.next_token();
		// 判断后面的是否符合语义
		while(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK)   // 如果后面接的是int或者char，那么需要判断是否符合,这里应该是一个循环才对
		{
			// 在这里需要预读取进行判断
			// 这里的预读取判断需要读取后面的两个
			// 通过这两个的类型来判断是变量定义还是是函数定义，或者主函数

			list<Token> temp = analyze.look_ahead(2);// 超前读两个词进行判断
			Token temp1 = temp.front();
			temp.pop_front(); // 移出去最前面的一个
			Token temp2 = temp.front();
			temp.pop_front();

			if(temp1.get_type() == token_type::IDEN)  // 首先判断int或者char后面应该是标识符
			{
				if(temp2.get_type() == token_type::LPARENT)  // 如果是函数的话，那这里应该是（
				{
					return; // 当判定后面是函数了，这里应该直接返回了
				}
				else  // 上面说明不是函数，所以就需要进行分析了
				{
					variable_define();  // 变量定义部分
					if(token.get_type() == token_type::SEMICN)  // 判断后面是否接着分号
					{
						token = analyze.next_token();

						// test
						cout << "finish 变量说明" << endl;
					}
					else
					{
						error_handler.error(17,token.get_linenumber());  // 后面应该紧接着分号
					}
				}
			}
			else 
			{
				error_handler.error(15,temp1.get_linenumber());
			}

		}

		// test
		cout << "finish 变量说明" << endl;
	}
	else
	{
		error_handler.error(17,token.get_linenumber()); // 后面必须是以分号结尾
	}
}

// 变量定义
void Parser::variable_define()
{
	sym.kind = symbol_kind::VARIABLE; //变量

	type_identifier(); // 类型标识符
	int flag = 0;
	do 
	{
		if(flag == 1 && token.get_type() == COMMA) // 读取后面的，
			token = analyze.next_token();

		if(token.get_type() == token_type::IDEN)
		{
			sym.name = token.get_value();
			// 预读取一个，判断是否是数组
			Token temp1 = analyze.look_ahead(1).front(); // 预读取一个
			if(temp1.get_type() != token_type::LBRACK)
			{
				sym.kind = symbol_kind::VARIABLE; // 不是数组的时候，就只能够使变量，这里数组作为变量的一种特殊类型存在
				symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());
			}

			token = analyze.next_token(); // 获取下一个词语判断
			if(token.get_type() == token_type::LBRACK)  // 当前的为[，如果当前不是[的话，直接跳过这里就行了
			{
				sym.kind = symbol_kind::ARRAY; // 这里判定是数组

				token = analyze.next_token(); 
				if(token.get_type() == token_type::INTCON && token.get_value() != "0")
				{
					int value = 0;
					string t = token.get_value();
					for(int i = 0;i < t.length();i ++)
					{
						value = (10 * value + t[i] - '0');
					}
					sym.array_len = value;

					token = analyze.next_token(); // 获取下一个词语
					if(token.get_type() == token_type::RBRACK)  // 是]
					{
						// 插入到符号表中
						symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

						token = analyze.next_token();  // 跳过

						// test
						cout << "finish 数组变量定义" << endl;
					}
					else
					{
						error_handler.error(8,token.get_linenumber());
					}
				}
				else
				{
					error_handler.error(25,token.get_linenumber()); // 数组的长度小不能够为0
				}
			}
			
			cout << "finish 变量定义" << endl;

			flag = 1; // 用作标志位，表示是否已经分析过这里，主要用于后面的while
		}
		else 
		{
			error_handler.error(15,token.get_linenumber());
		}
	}while(token.get_type() == token_type::COMMA);  // 这里如果是逗号的话，继续上面的分析
}

// 声明头部
void Parser::declar_head()
{
	if(token.get_type() == token_type::INTTK)   // int开头的声明头部
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::IDEN)
		{
			token = analyze.next_token();
		}
		else
		{
			error_handler.error(15,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::CHARTK)  // char开头的声明头部
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::IDEN)
		{
			token = analyze.next_token();
		}
		else
		{
			error_handler.error(15,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(23,token.get_linenumber());
	}
}

// 类型标识符
void Parser::type_identifier()
{
	if(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK)
	{
		token = analyze.next_token();
		
		// 符号表相关，填入变量类型
		if(token.get_type() == token_type::INTTK)
			sym.type = symbol_type::INT;
		else
			sym.type = symbol_type::CHAR;
	}
	else
	{
		error_handler.error(24,token.get_linenumber());
	}
}

// 函数定义，将有返回值和无返回值的函数定义合并在一起
void Parser::function_define()
{
	

	if(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK || token_type::VOIDTK)   // 以int，char或者void开始
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::IDEN)
		{
			sym.kind = symbol_kind::FUNCTION;
			// 函数名
			sym.name = token.get_value(); // 获取函数名
			symbol_table.add_symbol("global",sym,token.get_linenumber());
			current_table_name = sym.name;


			token = analyze.next_token();
			if(token.get_type() == token_type::LPARENT) // (
			{
				// 进入到参数的工作
				token = analyze.next_token();
				argument();
				if(token.get_type() == token_type::RPARENT) // )
				{
					token = analyze.next_token();
					if(token.get_type() == token_type::LBRACE)
					{
						token = analyze.next_token(); // 进入复合语句部分
						compond_statement();
						if(token.get_type() == token_type::RBRACE)
						{
							token = analyze.next_token();  // 这里结束当前token的分析

							// test
						cout << "finish 函数定义" << endl;
						}
						else
						{
							error_handler.error(10,token.get_linenumber());
						}
					}
					else
					{
						error_handler.error(9,token.get_linenumber());
					}
				}
				else
				{
					error_handler.error(6,token.get_linenumber());
				}
			}
			else
			{
				error_handler.error(5,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(15,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(26,token.get_linenumber());
	}
}

// 复合语句
void Parser::compond_statement()
{
	// 这里需要预读取判断是否是进入复合语句的哪一部分
	list<Token> temp = analyze.look_ahead(2);// 超前读两个词进行判断
	Token temp1 = temp.front();
	temp.pop_front(); // 移出去最前面的一个
	Token temp2 = temp.front();
	temp.pop_front();

	if(token.get_type() == token_type::CONSTTK)
	{
		const_declar();  // 进入常量说明
	}

	if((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK) && 
		(temp2.get_type() == token_type::SEMICN) && (temp1.get_type() == token_type::IDEN))
	{
		varialbe_declar(); // 变量说明
	}

	// 语句列
	statement_list();
}

// 参数
void Parser::argument()
{
	argument_list(); // 直接调用参数表就行
}

// 参数表
void Parser::argument_list()
{
	int flag = 0;
	if(token.get_type() != token_type::INTTK && token.get_type() != token_type::CHARTK) // 这里表示参数列表为空
		return;

	// 当不是空的时候，尽心下面的分析
	do
	{
		sym.kind = symbol_kind::PARAM;


		if(flag == 1 && token.get_type() == token_type::COMMA)
			token = analyze.next_token();


		// 类型标识符
		if(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK)
		{
			if(token.get_type() == token_type::INTTK)
			{
				sym.type = symbol_type::INT;
			}
			else
			{
				sym.type = symbol_type::CHAR;
			}

			token = analyze.next_token();
			if(token.get_type() == token_type::IDEN)
			{
				sym.name  = token.get_value();
				symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());


				token = analyze.next_token();
			}
			else
			{
				error_handler.error(15,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(24,token.get_linenumber()); 
		}

		flag = 1; // 进行到这里以后置1
	}while(token.get_type() == token_type::COMMA);
}

// 主函数
void Parser::main_function()
{
	sym.kind = symbol_kind::FUNCTION;

	if(token.get_type() == token_type::VOIDTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::MAINTK)
		{
			sym.name = token.get_value();
			symbol_table.add_symbol("global",sym,token.get_linenumber());
			current_table_name = token.get_value();

			token = analyze.next_token();
			if(token.get_type() == token_type::LPARENT) //(
			{
				token = analyze.next_token();
				if(token.get_type() == token_type::RPARENT)
				{
					token = analyze.next_token();
					if(token.get_type() == token_type::LBRACE)
					{
						token = analyze.next_token();
						compond_statement(); // 复合语句部分
						if(token.get_type() == token_type::RBRACE)
						{
							token = analyze.next_token();

							// test
						cout << "finish 主函数分析" << endl;
						}
						else 
						{
							error_handler.error(10,token.get_linenumber());
						}
					}
					else
					{
						error_handler.error(9,token.get_linenumber());
					}
				}
				else
				{
					error_handler.error(6,token.get_linenumber());
				}
			}
			else
			{
				error_handler.error(5,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(28,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(27,token.get_linenumber());
	}
}

// 表达式
void Parser::expression()
{
	// 表达式前面可能有正负号
	if(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU)
	{
		token = analyze.next_token();
	}

	// 没有正负号则直接进入到项中
	term();
	while(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU) // 项后面接的什么，如果是加法运算符则可以直接往下走
	{
		token = analyze.next_token();
		term();
	}
	// 不是加法运算符的时候，则分析可以结束
}

// 项
void Parser::term()
{
	factor();
	// 看后面是否含有乘法运算符，如果含有，那么进行后面的分析，不含有，则结束
	if(token.get_type() == token_type::MULT || token.get_type() == token_type::DIV)
	{
		factor();
	}
}

// 因子
void Parser::factor()
{
	// 这里标识符，标识符[]，有返回值的函数调用语句都是以标识符开头的
	if(token.get_type() == token_type::IDEN)
	{
		list<Token> temp = analyze.look_ahead(1); // 超前读一个，用于判断
		Token temp1 = temp.front();
		temp.pop_front();


		if(temp1.get_type() == token_type::LBRACK)
		{
			token = analyze.next_token(); // 把左括号读出 
			token = analyze.next_token();
			expression(); // 表达式
			if(token.get_type() == token_type::RBRACK)
			{
				token = analyze.next_token();
			}
			else
			{
				error_handler.error(8,token.get_linenumber());
			}
		}
		else if(temp1.get_type() == token_type::LPARENT)
		{
			func_call_statement(); // 执行有返回值的函数调用语句
		}
		else // 只有一个表达式的情况
		{
			token = analyze.next_token();
		}
	}
	else if(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU ||
		token.get_type() == token_type::INTCON )  // 整数
	{
		integer(); // 整数
	}
	else if(token.get_type() == token_type::CHARCON) // 字符
	{
		token = analyze.next_token();
	}
	else if(token.get_type() == token_type::LPARENT)
	{
		token = analyze.next_token();
		expression();
		if(token.get_type() == token_type::RPARENT)  // )
		{
			token = analyze.next_token();
		}
		else
		{
			error_handler.error(6,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(36,token.get_linenumber());
	}
}

// 语句
void Parser::statement()
{
	if(token.get_type() == token_type::IFTK) //条件语句
	{
		condition_statement(); // 条件语句
	}
	else if(token.get_type() == token_type::DOTK || token.get_type() == token_type::WHILETK) // 循环语句
	{
		loop_statement();  // 循环语句
	}
	else if(token.get_type() == token_type::LBRACE) // 语句列
	{
		token = analyze.next_token();
		statement_list();
		if(token.get_type() == token_type::RBRACE) // }
		{
			token = analyze.next_token();
		}
		else
		{
			error_handler.error(10,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::IDEN)  // 这里包括有返回值无返回值的函数调用语句，或者是赋值语句
	{
		list<Token> temp = analyze.look_ahead(1);  // 超前读取一个单词
		Token temp1 = temp.front();
		temp.pop_front();

		if(temp1.get_type() == token_type::LPARENT) // 这里表明是函数调用语句
		{
			func_call_statement(); // 函数调用语句
		}
		else if(temp1.get_type() == token_type::ASSIGN)  // 表明是赋值语句
		{
			assign_statement(); // 调用赋值语句
		}
		else
		{
			error_handler.error(37,token.get_linenumber());
			return ; // 发生错误之后，这里需要跳出，以免影响后面的判断
		}

		// 对语句后面的分号进行判断
		if(token.get_type() == token_type::SEMICN)  // 判断最后一个符号是不是;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::SCANFTK) //读语句
	{
		read_statement();
		if(token.get_type() == token_type::SEMICN)  // 判断最后一个符号是不是;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::PRINTFTK) // 写语句
	{
		write_statement(); // 写语句
		if(token.get_type() == token_type::SEMICN)  // 判断最后一个符号是不是;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::RETURNTK) // 返回语句
	{
		return_statement(); // 返回语句
		if(token.get_type() == token_type::SEMICN)
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::SEMICN) // 空语句，也就是直接读到了;
	{
		token = analyze.next_token();
	}
	else
	{
		error_handler.error(36,token.get_linenumber());
	}
}

// 赋值语句
void Parser::assign_statement()
{
	if(token.get_type() == token_type::IDEN)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::ASSIGN)  // 直接是等号
		{
			token = analyze.next_token();
			expression(); // 表达式
		}
		else if(token.get_type() == token_type::LBRACK)  // [  这里是对于数组的赋值
		{
			token = analyze.next_token(); // 获取下一个token
			expression();
			if(token.get_type() == token_type::RBRACK) // ]
			{
				token = analyze.next_token();  // 读入下一个token
				if(token.get_type() == token_type::ASSIGN)
				{
					token = analyze.next_token();
					expression(); // 分析赋值号右侧的表达式
				}
				else
				{
					error_handler.error(19,token.get_linenumber());
				}
			}
			else
			{
				error_handler.error(29,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(29,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(15,token.get_linenumber());
	}
}

// 条件语句
void Parser::condition_statement()
{
	if(token.get_type() == token_type::IFTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token(); // 获取下一个符号
			condition(); //条件
			if(token.get_type() == token_type::RPARENT) //)
			{
				token = analyze.next_token();
				statement();
				if(token.get_type() == token_type::ELSETK)
				{
					token = analyze.next_token();
					statement();
				}
				else  // 不是接着的else
				{
					return;
				}
			}
			else
			{
				error_handler.error(6,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(5,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(30,token.get_linenumber());
	}
}

// 条件
void Parser::condition()
{
	expression(); // 表达式
	// 判断是否是关系运算符
	if(token.get_type() == token_type::LSS || token.get_type() == token_type::LEQ ||
		token.get_type() == token_type::GRE || token.get_type() == token_type::GEQ ||
		token.get_type() == token_type::NEQ || token.get_type() == token_type::EQL)
	{
		token = analyze.next_token();
		expression(); // 表达式分析
	}
	else   // 不是关系运算符的时候，直接使用表达式分析就结束了
	{
		return;
	}
}

// 循环语句
void Parser::loop_statement()
{
	if(token.get_type() == token_type::DOTK)
	{
		dowhile_statement();
	}
	else if(token.get_type() == token_type::FORTK)
	{
		for_statement();
	}
	else
	{
		error_handler.error(36,token.get_linenumber());
	}
}

// do while循环
void Parser::dowhile_statement()
{
	if(token.get_type() == token_type::DOTK)
	{
		statement();
		if(token.get_type() == token_type::WHILETK)
		{
			token = analyze.next_token(); // 读取下一个词
			if(token.get_type() == token_type::LPARENT) // 判断是否是(
			{
				token = analyze.next_token();
				condition();
				if(token.get_type() == token_type::RPARENT)
				{
					token = analyze.next_token();
				}
				else
				{
					error_handler.error(6,token.get_linenumber());
				}
			}
			else
			{
				error_handler.error(5,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(32,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(31,token.get_linenumber());
	}
}

// for 循环
void Parser::for_statement()
{
	if(token.get_type() == token_type::FORTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token();
			if(token.get_type() == token_type::IDEN)
			{
				token = analyze.next_token();
				if(token.get_type() == token_type::ASSIGN)
				{
					token = analyze.next_token(); // 重新读入一个单词
					expression(); // 表达式
					if(token.get_type() == token_type::SEMICN) // ;
					{
						token = analyze.next_token();
						condition(); // 条件分析
						if(token.get_type() == token_type::SEMICN) // 判断是是否是;
						{
							token = analyze.next_token();
							if(token.get_type() == token_type::IDEN)
							{
								token = analyze.next_token();
								if(token.get_type() == token_type::ASSIGN)
								{
									token = analyze.next_token();
									if(token.get_type() == token_type::IDEN) // 应该是标识符，这里应该还要加入判断，需要判断这里的标识符是否等于前面的标识符
									{
										token = analyze.next_token();
										if(token.get_type() == token_type::MINU || token.get_type() == token_type::PLUS) // 如果是加法运算符
										{
											token = analyze.next_token();
											if(token.get_type() == token_type::INTCON) // 是无符号整数的情况
											{
												token = analyze.next_token(); // 获取下一个词
												if(token.get_type() == token_type::RPARENT)
												{
													token = analyze.next_token();
													statement(); // 语句
												}
												else
												{
													error_handler.error(6,token.get_linenumber());
												}
											}
											else
											{
												error_handler.error(20,token.get_linenumber());
											}
										}
										else
										{
											error_handler.error(35,token.get_linenumber());
										}
									}
									else
									{
										error_handler.error(15,token.get_linenumber());
									}
								}
								else
								{
									error_handler.error(19,token.get_linenumber());
								}
							}
							else
							{
								error_handler.error(15,token.get_linenumber());
							}
						}
						else
						{
							error_handler.error(34,token.get_linenumber());
						}
					}
					else
					{
						error_handler.error(34,token.get_linenumber());
					}
				}
				else
				{
					error_handler.error(19,token.get_linenumber());
				}
			}
			else
			{
				error_handler.error(15,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(5,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(33,token.get_linenumber());
	}
}

// 函数调用语句
void Parser::func_call_statement()
{
	if(token.get_type() == token_type::IDEN)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token();
			parameter_list();
			if(token.get_type() == token_type::RPARENT)  // 右括号
			{
				token = analyze.next_token(); // 读入下一个符号
			}
			else
			{
				error_handler.error(6,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(5,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(15,token.get_linenumber());
	}
}

// 值参数表
void Parser::parameter_list()
{
	if(token.get_type() == token_type::RPARENT)   // 是右括号的情况
		return;
	expression();  // 不是右括号的时候，值参数表中就包含了表达式
	while(token.get_type() == token_type::COMMA)
	{
		token = analyze.next_token();
		expression();
	}
}

// 语句列
void Parser::statement_list()
{
	while(token.get_type() == token_type::IFTK || token.get_type() == token_type::DOTK || 
		token.get_type() == token_type::WHILETK || token.get_type() == token_type::LBRACE ||
		token.get_type() == token_type::IDEN || token.get_type() == token_type::SCANFTK ||
		token.get_type() == token_type::PRINTFTK || token.get_type() == token_type::RETURNTK ||
		token.get_type() == token_type::SEMICN)   // 是这些符号开始的都会是语句
	{
		statement(); // 语句
	}
}

// 读语句
void Parser::read_statement()
{
	if(token.get_type() == token_type::SCANFTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token();
			if(token.get_type() == token_type::IDEN)
			{
				token = analyze.next_token();
				while(token.get_type() == token_type::COMMA)  // 读入的时候如果存在多个标识符的时候
				{
					token = analyze.next_token();
					if(token.get_type() == token_type::IDEN)
					{
						token = analyze.next_token();
					}
					else 
					{
						error_handler.error(15,token.get_linenumber());
					}
				}

				// )的判断
				if(token.get_type() == token_type::RPARENT)  // 判断是否分析完读语句
				{
					token = analyze.next_token();
				}
				else
				{
					error_handler.error(6,token.get_linenumber());
				}

			}
			else
			{
				error_handler.error(15,token.get_linenumber());
			}
		}
		else
		{
			error_handler.error(5,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(14,token.get_linenumber());
	}
}

// 写语句
void Parser::write_statement()
{
	if(token.get_type() == token_type::PRINTFTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT) // 这里必须是左括号
		{
			token = analyze.next_token();
			if(token.get_type() == token_type::STRCON)  // 后面跟着字符串了
			{
				token = analyze.next_token();
				if(token.get_type() == token_type::COMMA)
				{
					token = analyze.next_token();
					expression();
					if(token.get_type() == token_type::RPARENT)
					{
						token = analyze.next_token();
					}
					else 
					{
					error_handler.error(6,token.get_linenumber());
					}
				}
				else if(token.get_type() == token_type::RPARENT)
				{
					token = analyze.next_token();
				}
				else 
				{
					error_handler.error(13,token.get_linenumber());
				}
			}
			else  // 这里如果读取到的第一个不是字符串，那么就只有可能进入表达式的判断中了 
			{
				expression();
				if(token.get_type() == token_type::RPARENT) // 判断是否是右括号
				{
					token = analyze.next_token();
				}
				else
				{
					error_handler.error(6,token.get_linenumber());
				}
			}
		}
		else
		{
			error_handler.error(5,token.get_linenumber());
		}
	}
	else
	{
		error_handler.error(12,token.get_linenumber());
	}
}

// 返回语句
void Parser::return_statement()
{
	if(token.get_type() == token_type::RETURNTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)  // 判断是否等于左括号
		{
			token = analyze.next_token();
			expression(); // 表达式分析
			if(token.get_type() == token_type::RPARENT)  // 判断后面接着的是否是右括号
			{
				token = analyze.next_token();  
				return;
			} 
			else   // 这里是返回语句缺少右括号，通常情况下这里应该要完成跳读的工作
			{
				error_handler.error(6,token.get_linenumber());
				return;
			}
		}
		else  // 不是左括号的，这个时候关于返回语句的分析结束
		{
			return ;
		}
	}
	else 
	{
		error_handler.error(4,token.get_linenumber());
	}
}

void Parser::integer()
{
	if(token.get_type() == token_type::INTCON)   // 当开始不包含加减号的时候，可以直接分析结束
	{
		// 对整型值进行处理
		string temp = token.get_value();
		int value = 0;
		for(int i = 0;i < temp.length();i ++)
		{
			value = 10 * value + (int)(temp[i] - '0');
		} 
		sym.int_value = value;

		token = analyze.next_token(); //
	}
	else if(token.get_type() == token_type::MINU || token.get_type() == token_type::ASSIGN)  // 这里开始的是加或者减
	{
		// 得到这个整数
		string s = token.get_value();  // 表示符号

		token = analyze.next_token();
		if(token.get_type() == token_type::INTCON)
		{	
			// 对整型值进行处理，处理的时候应该在读取之前处理
			string temp = token.get_value();
			int value = 0;
			for(int i = 0;i < temp.length();i ++)
			{
				value = 10 * value + (int)(temp[i] - '0');
			}
			if(s == "-")
				sym.int_value = - value;
			else 
				sym.int_value = value;

			token = analyze.next_token();

		}
		else
		{
			error_handler.error(20,token.get_linenumber());
		}
	}
	else // 
	{
		error_handler.error(21,token.get_linenumber());
	}
}