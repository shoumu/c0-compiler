/*
 * Parser���ʵ��
 */

#include <string>
#include "Parser.h"
#include "Error.h"
#include "commons.h"

using namespace std;

// ���캯��
Parser::Parser(string source)
{
	sourcefile = source; // ���ݽ���Ҫ�������ļ���
	analyze.load_file(sourcefile);
	current_table_name = "global";		// ����������ǰ���ű�����֣���ʼ��Ϊglobal��Ϊȫ�ַ��ű�
}

// ��������
Parser::~Parser()
{ 
	symbol_table.print_symbol();
}

void Parser::parser_sourcefile()
{
	
}

// ����
void Parser::program()
{
	token = analyze.next_token();
	if(token.get_type() == token_type::CONSTTK)  // ����˵��
	{
		const_declar(); // ����˵��
	}

	list<Token> temp = analyze.look_ahead(2);// ��ǰ�������ʽ����ж�
	Token temp1 = temp.front();
	temp.pop_front(); // �Ƴ�ȥ��ǰ���һ��
	Token temp2 = temp.front();
	temp.pop_front();

	int flag = 0; // ������־�������temp1��temp2ʹ��û��

	if((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK) && 
		(temp1.get_type() == token_type::IDEN) && (temp2.get_type() != token_type::LPARENT))  // �����ʾ����˵������Ҫ���ж�temp2�����ͣ������(����ô����Ȼ�Ǻ�����
	{
		varialbe_declar();
		flag = 1;
	}

	if(flag == 1)  // ���ǰ���Ѿ�ʹ���ˣ�����Ҫ���¶�ȡ��������Ҫ���¶�ȡ
	{
		list<Token> temp = analyze.look_ahead(2);// ��ǰ�������ʽ����ж�
		temp1 = temp.front();
		temp.pop_front(); // �Ƴ�ȥ��ǰ���һ��
		temp2 = temp.front();
		temp.pop_front();
	}

	while((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK ||
		token.get_type() == token_type::VOIDTK) && (temp1.get_type() == token_type::IDEN))
	{
		function_define(); // �������壬���ܴ��ڶ����������Ĳ���
		
		// ��ǰ��ȡ�����ж�
		list<Token> temp = analyze.look_ahead(1);
		temp1 = temp.front();
		temp.pop_front();
	}

	main_function(); // ����������������˾ͷ�������

	if(token.get_type() == token_type::ENDOFFILE)  // �������ļ�ĩβ
	{
		cout << "�Ѿ��������ļ�ĩβ" << endl;
	}
}

// ����˵��
void Parser::const_declar()
{
	if(token.get_type() == token_type::CONSTTK)
	{
		token = analyze.next_token();
		const_define();
		if(token.get_type() == token_type::SEMICN)
		{
			token = analyze.next_token();
			while(token.get_type() == token_type::CONSTTK)  // ����˵�����ְ�����������������
			{
				token = analyze.next_token();
				const_define();
				if(token.get_type() == token_type::SEMICN)
				{
					token = analyze.next_token();

					// test
					cout << "finish ����˵��" << endl;
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

// ��������
void Parser::const_define()
{
	sym.kind = symbol_kind::CONST;

	if(token.get_type() == token_type::INTTK)
	{
		token = analyze.next_token();

		/********************���ű�*********************/
		sym.type = symbol_type::INT;

		if(token.get_type() == token_type::IDEN)
		{
			/********************���ű�*********************/
			sym.name = token.get_value();

			token = analyze.next_token();
			if(token.get_type() == token_type::ASSIGN)
			{
				// ������Ҫ��������
				token = analyze.next_token();
				integer(); // ��������

				// ������д���ű�
				symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

				while(token.get_type() == token_type::COMMA)   // �����ж��������������
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

							// ������д���ű�
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

	// �����ַ��͵Ĵ���
	else if(token.get_type() == token_type::CHARTK)
	{
		sym.type = symbol_type::CHAR; // ��������Ϊ�ַ�����

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
					while(token.get_type() == token_type::COMMA)  // ���ﴦ������ж����������ĵط�
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

// ����˵��
void Parser::varialbe_declar()
{
	variable_define();
	if(token.get_type() == token_type::SEMICN)
	{
		token = analyze.next_token();
		// �жϺ�����Ƿ��������
		while(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK)   // �������ӵ���int����char����ô��Ҫ�ж��Ƿ����,����Ӧ����һ��ѭ���Ŷ�
		{
			// ��������ҪԤ��ȡ�����ж�
			// �����Ԥ��ȡ�ж���Ҫ��ȡ���������
			// ͨ�����������������ж��Ǳ������廹���Ǻ������壬����������

			list<Token> temp = analyze.look_ahead(2);// ��ǰ�������ʽ����ж�
			Token temp1 = temp.front();
			temp.pop_front(); // �Ƴ�ȥ��ǰ���һ��
			Token temp2 = temp.front();
			temp.pop_front();

			if(temp1.get_type() == token_type::IDEN)  // �����ж�int����char����Ӧ���Ǳ�ʶ��
			{
				if(temp2.get_type() == token_type::LPARENT)  // ����Ǻ����Ļ���������Ӧ���ǣ�
				{
					return; // ���ж������Ǻ����ˣ�����Ӧ��ֱ�ӷ�����
				}
				else  // ����˵�����Ǻ��������Ծ���Ҫ���з�����
				{
					variable_define();  // �������岿��
					if(token.get_type() == token_type::SEMICN)  // �жϺ����Ƿ���ŷֺ�
					{
						token = analyze.next_token();

						// test
						cout << "finish ����˵��" << endl;
					}
					else
					{
						error_handler.error(17,token.get_linenumber());  // ����Ӧ�ý����ŷֺ�
					}
				}
			}
			else 
			{
				error_handler.error(15,temp1.get_linenumber());
			}

		}

		// test
		cout << "finish ����˵��" << endl;
	}
	else
	{
		error_handler.error(17,token.get_linenumber()); // ����������ԷֺŽ�β
	}
}

// ��������
void Parser::variable_define()
{
	sym.kind = symbol_kind::VARIABLE; //����

	type_identifier(); // ���ͱ�ʶ��
	int flag = 0;
	do 
	{
		if(flag == 1 && token.get_type() == COMMA) // ��ȡ����ģ�
			token = analyze.next_token();

		if(token.get_type() == token_type::IDEN)
		{
			sym.name = token.get_value();
			// Ԥ��ȡһ�����ж��Ƿ�������
			Token temp1 = analyze.look_ahead(1).front(); // Ԥ��ȡһ��
			if(temp1.get_type() != token_type::LBRACK)
			{
				sym.kind = symbol_kind::VARIABLE; // ���������ʱ�򣬾�ֻ�ܹ�ʹ����������������Ϊ������һ���������ʹ���
				symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());
			}

			token = analyze.next_token(); // ��ȡ��һ�������ж�
			if(token.get_type() == token_type::LBRACK)  // ��ǰ��Ϊ[�������ǰ����[�Ļ���ֱ���������������
			{
				sym.kind = symbol_kind::ARRAY; // �����ж�������

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

					token = analyze.next_token(); // ��ȡ��һ������
					if(token.get_type() == token_type::RBRACK)  // ��]
					{
						// ���뵽���ű���
						symbol_table.add_symbol(current_table_name,sym,token.get_linenumber());

						token = analyze.next_token();  // ����

						// test
						cout << "finish �����������" << endl;
					}
					else
					{
						error_handler.error(8,token.get_linenumber());
					}
				}
				else
				{
					error_handler.error(25,token.get_linenumber()); // ����ĳ���С���ܹ�Ϊ0
				}
			}
			
			cout << "finish ��������" << endl;

			flag = 1; // ������־λ����ʾ�Ƿ��Ѿ������������Ҫ���ں����while
		}
		else 
		{
			error_handler.error(15,token.get_linenumber());
		}
	}while(token.get_type() == token_type::COMMA);  // ��������Ƕ��ŵĻ�����������ķ���
}

// ����ͷ��
void Parser::declar_head()
{
	if(token.get_type() == token_type::INTTK)   // int��ͷ������ͷ��
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
	else if(token.get_type() == token_type::CHARTK)  // char��ͷ������ͷ��
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

// ���ͱ�ʶ��
void Parser::type_identifier()
{
	if(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK)
	{
		token = analyze.next_token();
		
		// ���ű���أ������������
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

// �������壬���з���ֵ���޷���ֵ�ĺ�������ϲ���һ��
void Parser::function_define()
{
	

	if(token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK || token_type::VOIDTK)   // ��int��char����void��ʼ
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::IDEN)
		{
			sym.kind = symbol_kind::FUNCTION;
			// ������
			sym.name = token.get_value(); // ��ȡ������
			symbol_table.add_symbol("global",sym,token.get_linenumber());
			current_table_name = sym.name;


			token = analyze.next_token();
			if(token.get_type() == token_type::LPARENT) // (
			{
				// ���뵽�����Ĺ���
				token = analyze.next_token();
				argument();
				if(token.get_type() == token_type::RPARENT) // )
				{
					token = analyze.next_token();
					if(token.get_type() == token_type::LBRACE)
					{
						token = analyze.next_token(); // ���븴����䲿��
						compond_statement();
						if(token.get_type() == token_type::RBRACE)
						{
							token = analyze.next_token();  // ���������ǰtoken�ķ���

							// test
						cout << "finish ��������" << endl;
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

// �������
void Parser::compond_statement()
{
	// ������ҪԤ��ȡ�ж��Ƿ��ǽ��븴��������һ����
	list<Token> temp = analyze.look_ahead(2);// ��ǰ�������ʽ����ж�
	Token temp1 = temp.front();
	temp.pop_front(); // �Ƴ�ȥ��ǰ���һ��
	Token temp2 = temp.front();
	temp.pop_front();

	if(token.get_type() == token_type::CONSTTK)
	{
		const_declar();  // ���볣��˵��
	}

	if((token.get_type() == token_type::INTTK || token.get_type() == token_type::CHARTK) && 
		(temp2.get_type() == token_type::SEMICN) && (temp1.get_type() == token_type::IDEN))
	{
		varialbe_declar(); // ����˵��
	}

	// �����
	statement_list();
}

// ����
void Parser::argument()
{
	argument_list(); // ֱ�ӵ��ò��������
}

// ������
void Parser::argument_list()
{
	int flag = 0;
	if(token.get_type() != token_type::INTTK && token.get_type() != token_type::CHARTK) // �����ʾ�����б�Ϊ��
		return;

	// �����ǿյ�ʱ�򣬾�������ķ���
	do
	{
		sym.kind = symbol_kind::PARAM;


		if(flag == 1 && token.get_type() == token_type::COMMA)
			token = analyze.next_token();


		// ���ͱ�ʶ��
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

		flag = 1; // ���е������Ժ���1
	}while(token.get_type() == token_type::COMMA);
}

// ������
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
						compond_statement(); // ������䲿��
						if(token.get_type() == token_type::RBRACE)
						{
							token = analyze.next_token();

							// test
						cout << "finish ����������" << endl;
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

// ���ʽ
void Parser::expression()
{
	// ���ʽǰ�������������
	if(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU)
	{
		token = analyze.next_token();
	}

	// û����������ֱ�ӽ��뵽����
	term();
	while(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU) // �����ӵ�ʲô������Ǽӷ�����������ֱ��������
	{
		token = analyze.next_token();
		term();
	}
	// ���Ǽӷ��������ʱ����������Խ���
}

// ��
void Parser::term()
{
	factor();
	// �������Ƿ��г˷��������������У���ô���к���ķ����������У������
	if(token.get_type() == token_type::MULT || token.get_type() == token_type::DIV)
	{
		factor();
	}
}

// ����
void Parser::factor()
{
	// �����ʶ������ʶ��[]���з���ֵ�ĺ���������䶼���Ա�ʶ����ͷ��
	if(token.get_type() == token_type::IDEN)
	{
		list<Token> temp = analyze.look_ahead(1); // ��ǰ��һ���������ж�
		Token temp1 = temp.front();
		temp.pop_front();


		if(temp1.get_type() == token_type::LBRACK)
		{
			token = analyze.next_token(); // �������Ŷ��� 
			token = analyze.next_token();
			expression(); // ���ʽ
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
			func_call_statement(); // ִ���з���ֵ�ĺ����������
		}
		else // ֻ��һ�����ʽ�����
		{
			token = analyze.next_token();
		}
	}
	else if(token.get_type() == token_type::PLUS || token.get_type() == token_type::MINU ||
		token.get_type() == token_type::INTCON )  // ����
	{
		integer(); // ����
	}
	else if(token.get_type() == token_type::CHARCON) // �ַ�
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

// ���
void Parser::statement()
{
	if(token.get_type() == token_type::IFTK) //�������
	{
		condition_statement(); // �������
	}
	else if(token.get_type() == token_type::DOTK || token.get_type() == token_type::WHILETK) // ѭ�����
	{
		loop_statement();  // ѭ�����
	}
	else if(token.get_type() == token_type::LBRACE) // �����
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
	else if(token.get_type() == token_type::IDEN)  // ��������з���ֵ�޷���ֵ�ĺ���������䣬�����Ǹ�ֵ���
	{
		list<Token> temp = analyze.look_ahead(1);  // ��ǰ��ȡһ������
		Token temp1 = temp.front();
		temp.pop_front();

		if(temp1.get_type() == token_type::LPARENT) // ��������Ǻ����������
		{
			func_call_statement(); // �����������
		}
		else if(temp1.get_type() == token_type::ASSIGN)  // �����Ǹ�ֵ���
		{
			assign_statement(); // ���ø�ֵ���
		}
		else
		{
			error_handler.error(37,token.get_linenumber());
			return ; // ��������֮��������Ҫ����������Ӱ�������ж�
		}

		// ��������ķֺŽ����ж�
		if(token.get_type() == token_type::SEMICN)  // �ж����һ�������ǲ���;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::SCANFTK) //�����
	{
		read_statement();
		if(token.get_type() == token_type::SEMICN)  // �ж����һ�������ǲ���;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::PRINTFTK) // д���
	{
		write_statement(); // д���
		if(token.get_type() == token_type::SEMICN)  // �ж����һ�������ǲ���;
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::RETURNTK) // �������
	{
		return_statement(); // �������
		if(token.get_type() == token_type::SEMICN)
		{
			token = analyze.next_token();
		}
		else 
		{
			error_handler.error(34,token.get_linenumber());
		}
	}
	else if(token.get_type() == token_type::SEMICN) // ����䣬Ҳ����ֱ�Ӷ�����;
	{
		token = analyze.next_token();
	}
	else
	{
		error_handler.error(36,token.get_linenumber());
	}
}

// ��ֵ���
void Parser::assign_statement()
{
	if(token.get_type() == token_type::IDEN)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::ASSIGN)  // ֱ���ǵȺ�
		{
			token = analyze.next_token();
			expression(); // ���ʽ
		}
		else if(token.get_type() == token_type::LBRACK)  // [  �����Ƕ�������ĸ�ֵ
		{
			token = analyze.next_token(); // ��ȡ��һ��token
			expression();
			if(token.get_type() == token_type::RBRACK) // ]
			{
				token = analyze.next_token();  // ������һ��token
				if(token.get_type() == token_type::ASSIGN)
				{
					token = analyze.next_token();
					expression(); // ������ֵ���Ҳ�ı��ʽ
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

// �������
void Parser::condition_statement()
{
	if(token.get_type() == token_type::IFTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token(); // ��ȡ��һ������
			condition(); //����
			if(token.get_type() == token_type::RPARENT) //)
			{
				token = analyze.next_token();
				statement();
				if(token.get_type() == token_type::ELSETK)
				{
					token = analyze.next_token();
					statement();
				}
				else  // ���ǽ��ŵ�else
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

// ����
void Parser::condition()
{
	expression(); // ���ʽ
	// �ж��Ƿ��ǹ�ϵ�����
	if(token.get_type() == token_type::LSS || token.get_type() == token_type::LEQ ||
		token.get_type() == token_type::GRE || token.get_type() == token_type::GEQ ||
		token.get_type() == token_type::NEQ || token.get_type() == token_type::EQL)
	{
		token = analyze.next_token();
		expression(); // ���ʽ����
	}
	else   // ���ǹ�ϵ�������ʱ��ֱ��ʹ�ñ��ʽ�����ͽ�����
	{
		return;
	}
}

// ѭ�����
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

// do whileѭ��
void Parser::dowhile_statement()
{
	if(token.get_type() == token_type::DOTK)
	{
		statement();
		if(token.get_type() == token_type::WHILETK)
		{
			token = analyze.next_token(); // ��ȡ��һ����
			if(token.get_type() == token_type::LPARENT) // �ж��Ƿ���(
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

// for ѭ��
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
					token = analyze.next_token(); // ���¶���һ������
					expression(); // ���ʽ
					if(token.get_type() == token_type::SEMICN) // ;
					{
						token = analyze.next_token();
						condition(); // ��������
						if(token.get_type() == token_type::SEMICN) // �ж����Ƿ���;
						{
							token = analyze.next_token();
							if(token.get_type() == token_type::IDEN)
							{
								token = analyze.next_token();
								if(token.get_type() == token_type::ASSIGN)
								{
									token = analyze.next_token();
									if(token.get_type() == token_type::IDEN) // Ӧ���Ǳ�ʶ��������Ӧ�û�Ҫ�����жϣ���Ҫ�ж�����ı�ʶ���Ƿ����ǰ��ı�ʶ��
									{
										token = analyze.next_token();
										if(token.get_type() == token_type::MINU || token.get_type() == token_type::PLUS) // ����Ǽӷ������
										{
											token = analyze.next_token();
											if(token.get_type() == token_type::INTCON) // ���޷������������
											{
												token = analyze.next_token(); // ��ȡ��һ����
												if(token.get_type() == token_type::RPARENT)
												{
													token = analyze.next_token();
													statement(); // ���
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

// �����������
void Parser::func_call_statement()
{
	if(token.get_type() == token_type::IDEN)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)
		{
			token = analyze.next_token();
			parameter_list();
			if(token.get_type() == token_type::RPARENT)  // ������
			{
				token = analyze.next_token(); // ������һ������
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

// ֵ������
void Parser::parameter_list()
{
	if(token.get_type() == token_type::RPARENT)   // �������ŵ����
		return;
	expression();  // ���������ŵ�ʱ��ֵ�������оͰ����˱��ʽ
	while(token.get_type() == token_type::COMMA)
	{
		token = analyze.next_token();
		expression();
	}
}

// �����
void Parser::statement_list()
{
	while(token.get_type() == token_type::IFTK || token.get_type() == token_type::DOTK || 
		token.get_type() == token_type::WHILETK || token.get_type() == token_type::LBRACE ||
		token.get_type() == token_type::IDEN || token.get_type() == token_type::SCANFTK ||
		token.get_type() == token_type::PRINTFTK || token.get_type() == token_type::RETURNTK ||
		token.get_type() == token_type::SEMICN)   // ����Щ���ſ�ʼ�Ķ��������
	{
		statement(); // ���
	}
}

// �����
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
				while(token.get_type() == token_type::COMMA)  // �����ʱ��������ڶ����ʶ����ʱ��
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

				// )���ж�
				if(token.get_type() == token_type::RPARENT)  // �ж��Ƿ����������
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

// д���
void Parser::write_statement()
{
	if(token.get_type() == token_type::PRINTFTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT) // ���������������
		{
			token = analyze.next_token();
			if(token.get_type() == token_type::STRCON)  // ��������ַ�����
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
			else  // ���������ȡ���ĵ�һ�������ַ�������ô��ֻ�п��ܽ�����ʽ���ж����� 
			{
				expression();
				if(token.get_type() == token_type::RPARENT) // �ж��Ƿ���������
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

// �������
void Parser::return_statement()
{
	if(token.get_type() == token_type::RETURNTK)
	{
		token = analyze.next_token();
		if(token.get_type() == token_type::LPARENT)  // �ж��Ƿ����������
		{
			token = analyze.next_token();
			expression(); // ���ʽ����
			if(token.get_type() == token_type::RPARENT)  // �жϺ�����ŵ��Ƿ���������
			{
				token = analyze.next_token();  
				return;
			} 
			else   // �����Ƿ������ȱ�������ţ�ͨ�����������Ӧ��Ҫ��������Ĺ���
			{
				error_handler.error(6,token.get_linenumber());
				return;
			}
		}
		else  // ���������ŵģ����ʱ����ڷ������ķ�������
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
	if(token.get_type() == token_type::INTCON)   // ����ʼ�������Ӽ��ŵ�ʱ�򣬿���ֱ�ӷ�������
	{
		// ������ֵ���д���
		string temp = token.get_value();
		int value = 0;
		for(int i = 0;i < temp.length();i ++)
		{
			value = 10 * value + (int)(temp[i] - '0');
		} 
		sym.int_value = value;

		token = analyze.next_token(); //
	}
	else if(token.get_type() == token_type::MINU || token.get_type() == token_type::ASSIGN)  // ���￪ʼ���Ǽӻ��߼�
	{
		// �õ��������
		string s = token.get_value();  // ��ʾ����

		token = analyze.next_token();
		if(token.get_type() == token_type::INTCON)
		{	
			// ������ֵ���д��������ʱ��Ӧ���ڶ�ȡ֮ǰ����
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