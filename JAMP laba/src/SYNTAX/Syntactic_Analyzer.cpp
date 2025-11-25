#include "Syntactic_Analyzer.h"

#include <Windows.h>

#define SKIP_SPACING 	while (current_toke->get_type() == Token::divider_ and current_toke->get_lexema() == " ") \
	{ \
		++current_toke; \
	}
#define CONTINUE_EXPR if (current_toke->get_type() == Token::opErator) \
			{ \
				if (current_toke->get_lexema() == "=") \
				{\
					std::string mes = "Unexpected operator. Expected <+/->. " + current_toke->get_lexema() + " " + " met.";\
					output_error(mes);\
				}\
				else \
				{\
					expr_node.childs.push_back(Operator_parsing()); \
					\
					SKIP_SPACING\
						\
						expr_node.childs.push_back(Expr_parsing());\
				}\
			}

void Syntactic_Analyzer::output_error(const std::string& error)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 4);

	std::cout << char(16) << " line: " << parsed_line_counter << " Syntax error: " << error << '\n';

	SetConsoleTextAttribute(h, 7);
}

Syntactic_Analyzer::Syntactic_Analyzer(const std::vector<Token>& token_stream)
{
	this->token_stream = &token_stream;
}

Syntactic_Analyzer::~Syntactic_Analyzer()
{
	delete this->syntax_tree;
}

void Syntactic_Analyzer::Program_parsing()
{
	this->syntax_tree = new tree_node;
	syntax_tree->rule = "Program:";

	current_toke = &((*token_stream).front());

	syntax_tree->childs.push_back(Begin_parsing());
	syntax_tree->childs.push_back(Descriptions_parsing());
	syntax_tree->childs.push_back(Operators_parsing());
	syntax_tree->childs.push_back(End_parsing());


}


void Syntactic_Analyzer::tree_node::show_parsed_str(tree_node& node)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 2);

	std::cout <<' ' << node.data;
	for (int i = 0; i < node.childs.size(); ++i)
	{
		show_parsed_str(node.childs[i]);
	}

	SetConsoleTextAttribute(h, 7);
}


Syntactic_Analyzer::tree_node Syntactic_Analyzer::Begin_parsing()
{
	
	++parsed_line_counter; 
	tree_node begin_node;
	begin_node.rule = "Begin:";

	if (current_toke->get_type() != Token::key_word)
	{
		std::string mes = "Expected keyword. " + current_toke->get_lexema() + " met.";
		output_error(mes);
	}

	if (current_toke->get_lexema() != "PROGRAM")
	{
		std::string mes = "Expected keyword <PROGRAM>. " + current_toke->get_lexema() + " met.";
		output_error(mes);
	}
	//matched token
	begin_node.data = "PROGRAM";
	++current_toke;
	

	while (current_toke->get_type() == Token::divider_)
	{
		if (current_toke->get_lexema() != " ") 
		{
			std::string mes = "Unexpected separator. Expected < >. "  + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
		++current_toke;
		
	}

	begin_node.childs.push_back(Id_parsing());
	
	

	return begin_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Descriptions_parsing()
{
	
	tree_node descriptions_node;
	descriptions_node.rule = "Descriptions:";

	if (current_toke->get_type() != Token::key_word)
	{
		std::string mes = "Expected keyword. " + current_toke->get_lexema() + " met.";
		output_error(mes);
	}

	while (current_toke->get_type() == Token::key_word)
	{
		descriptions_node.childs.push_back(Descr_parsing());
	}

	return descriptions_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Operators_parsing()
{
	
	tree_node operators_node;
	operators_node.rule = "Operators:";

	while (current_toke->get_type() == Token::identifi_)
	{
		operators_node.childs.push_back(Op_parsing());
	}

	return operators_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::End_parsing()
{
	if (current_toke == (&((*token_stream).front()) + token_stream->size())) 
	{
		std::string mes = "Expected <END>. Nothing met.";
		output_error(mes);
		return tree_node(); 
	}

	++parsed_line_counter;
	tree_node begin_node;
	begin_node.rule = "End:";

	if (current_toke->get_type() != Token::key_word)
	{
		std::string mes = "Expected keyword. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	if (current_toke->get_lexema() != "END")
	{
		std::string mes = "Expected <END>. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}
	//matched token
	begin_node.data = "END";
	++current_toke;
	
	if (current_toke == (&((*token_stream).front()) + token_stream->size()))
	{
		std::string mes = "Expected < >. Nothing met.";
		output_error(mes);
		return tree_node();
	}



	while (current_toke->get_type() == Token::divider_)
	{
		if (current_toke->get_lexema() != " ")
		{
			std::string mes = "Unexpected separator. Expected < >. "  + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
		++current_toke;
		if (current_toke == (&((*token_stream).front()) + token_stream->size()))
		{
			std::string mes = "Expected ID. Nothing met.";
			output_error(mes);
			return tree_node();
		}

		
	}

	begin_node.childs.push_back(Id_parsing());

	return begin_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Id_parsing()
{
	
	tree_node id_node;
	id_node.rule = "Id:";

	if (current_toke->get_type() != Token::identifi_)
	{
		std::string mes = "ID expected. " + current_toke->get_lexema() + " met.";
		output_error(mes);
	}

	//matched token
	id_node.data = current_toke->get_lexema();
	++current_toke;
	

	return id_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Descr_parsing()
{
	
	++parsed_line_counter;
	tree_node descr_node;
	descr_node.rule = "Descr:";
	
	descr_node.childs.push_back(Type_parsing());

	SKIP_SPACING

	descr_node.childs.push_back(VarList_parsing());

	return descr_node;
}
Syntactic_Analyzer::tree_node Syntactic_Analyzer::Type_parsing()
{
	
	tree_node type_node;
	type_node.rule = "Type:";

	if (current_toke->get_type() != Token::key_word)
	{
		std::string mes = "Expected keyword. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	if (current_toke->get_lexema() != "INTEGER" and current_toke->get_lexema() != "REAL")
	{
		std::string mes = "Expected <INTEGER/REAL>. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	//matched token
	type_node.data = current_toke->get_lexema();
	++current_toke;
	

	return type_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::VarList_parsing()
{
	
	tree_node varList_node;
	varList_node.rule = "Var:";

	varList_node.childs.push_back(Id_parsing());

	SKIP_SPACING

	if (current_toke->get_type() == Token::divider_)
	{
		if (current_toke->get_lexema() == ",") 
		{
			varList_node.childs.push_back(Divider_parsing());
			
			SKIP_SPACING

			varList_node.childs.push_back(VarList_parsing());
		}
		else
		{
			std::string mes = "Unexpected separator. Expected <,>. " + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
	}
	return varList_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Op_parsing()
{
	
	++parsed_line_counter;
	tree_node op_node;
	op_node.rule = "Op:";
	op_node.childs.push_back(Id_parsing());

	SKIP_SPACING

	if (current_toke->get_type() == Token::divider_)
	{
		std::string mes = "Unexpected separator between the operator and the operand. Expected < >. " + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}
	if (current_toke->get_type() == Token::opErator)
	{
		if (current_toke->get_lexema() == "=") { op_node.childs.push_back(Operator_parsing()); }
		else
		{
			std::string mes = "Expected <=>. " + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
	}
	else
	{
		std::string mes = "Expected Operator. " + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	SKIP_SPACING

	op_node.childs.push_back(Expr_parsing());

	return op_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Operator_parsing()
{
	
	tree_node operator_node;
	operator_node.rule = "Operator:";
	if (current_toke->get_type() != Token::opErator)
	{
		std::string mes = "Expected operator. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}
	operator_node.data = current_toke->get_lexema();
	
	++current_toke;


	return operator_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Const_parsing()
{
	
	tree_node const_node;
	const_node.rule = "Const:";

	if (current_toke->get_type() != Token::constant)
	{
		std::string mes = "Expected const. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	const_node.data = current_toke->get_lexema();

	++current_toke;

	return const_node;
}
Syntactic_Analyzer::tree_node Syntactic_Analyzer::Function_parsing()
{
	
	tree_node function_node;
	function_node.rule = "Function:";
	if (current_toke->get_type() != Token::function)
	{
		std::string mes = "Expected function. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	function_node.data = current_toke->get_lexema();
	
	++current_toke;


	return function_node;
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Expr_parsing()
{
	
	tree_node expr_node;
	expr_node.rule = "Expr:";

	switch (current_toke->get_type())
	{
	case Token::identifi_: {
		expr_node.childs.push_back(Id_parsing());

		SKIP_SPACING

			CONTINUE_EXPR

		return expr_node;
	
	} break;



	case Token::constant: {
		expr_node.childs.push_back(Const_parsing());

		SKIP_SPACING

			CONTINUE_EXPR

		return expr_node;
	}break;
	case Token::divider_: {
		expr_node.rule = "( Expr ):";

		if (current_toke->get_lexema() != "(")
		{
			std::string mes = "Unexpected separator. Expected <(>. " + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
		else
		{
			expr_node.childs.push_back(Divider_parsing());

			SKIP_SPACING

				expr_node.childs.push_back(Expr_parsing());

			SKIP_SPACING

				if (current_toke->get_lexema() != ")")
				{
					std::string mes = "Unexpected separator. Expected <)>. " + current_toke->get_lexema() + " " + " met.";
					output_error(mes);
				}
				else
				{
					expr_node.childs.push_back(Divider_parsing());
				}

			SKIP_SPACING

				CONTINUE_EXPR
		}



		return expr_node;
	}break;
	case Token::function: {
		expr_node.rule = "Function( Expr ):";
		expr_node.childs.push_back(Function_parsing());
		
		if (current_toke->get_type() != Token::divider_)
		{
			std::string mes = "Expected separator after function call. " + current_toke->get_lexema() + " " + " met.";
			output_error(mes);
		}
		else
		{
			if (current_toke->get_lexema() != "(")
			{
				std::string mes = "Expected separator <(> after function call. " + current_toke->get_lexema() + " " + " met.";
				output_error(mes);
			}
			else
			{
				expr_node.childs.push_back(Divider_parsing());
				
				SKIP_SPACING
					expr_node.childs.push_back(Expr_parsing());
				SKIP_SPACING

					if (current_toke->get_type() != Token::divider_)
					{
						std::string mes = "Expected ender-separator after function call. " + current_toke->get_lexema() + " " + " met.";
						output_error(mes);
					}
					else
					{
							if (current_toke->get_lexema() != ")")
							{
								std::string mes = "Expected ender-separator <)> after function call. " + current_toke->get_lexema() + " " + " met.";
								output_error(mes);
							}
							else
							{
								expr_node.childs.push_back(Divider_parsing());
							}
					}
				SKIP_SPACING

					CONTINUE_EXPR

			}
		}
		




		return expr_node;
	}break;
	default: 
	{
		output_error("Unexpected Expr");
		return expr_node;
	} break;
	}
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Divider_parsing()
{
	
	tree_node divider_node;
	divider_node.rule = "Divider:";

	if (current_toke->get_type() != Token::divider_)
	{
		std::string mes = "Expected separator. "  + current_toke->get_lexema() + " " + " met.";
		output_error(mes);
	}

	divider_node.data = current_toke->get_lexema();
	++current_toke;
	

	return divider_node;
}

void Syntactic_Analyzer::tree_node::show_tree(int deep)
{
	for (int i = 0; i < deep; ++i)
	{
		if (i == 0)
		{
			std::cout << '|';
			continue;
		}
		if (i == 1) 
		{
			std::cout << "---";
			continue;
		}
		std::cout << "----";
	}
	std::cout << rule;

	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 6);

	std::cout<< data << '\n';

	SetConsoleTextAttribute(h, 7);
	++deep;
	for (int i = 0; i < childs.size(); ++i)
	{
		this->childs[i].show_tree(deep);
	}

	if (this->rule == "Begin:" or this->rule == "Descr:" or this->rule == "Op:" or this->rule == "End:")
	{
		show_parsed_str(*this); std::cout << '\n';
	}
}
