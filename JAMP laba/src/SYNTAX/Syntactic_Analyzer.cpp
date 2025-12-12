#include "Syntactic_Analyzer.h"

#include <Windows.h>

#include "../Compiler.h"

#define SKIP_SPACE \
		if (current_toke.get_type() == Token::token_type::divider_)\
		{ \
			if(	current_toke.get_lexema() == " ") \
			{\
				return;\
			}\
		}

void Syntactic_Analyzer::output_error(const std::string& error)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, 4);

	std::cout << char(16) << " line: " << parsed_line_counter << " Syntax error: " << error << '\n';

	SetConsoleTextAttribute(h, 7);
}

Syntactic_Analyzer::Syntactic_Analyzer(Compiler* comp)
{
	this->comp = comp;
}

Syntactic_Analyzer::~Syntactic_Analyzer()
{
	delete this->syntax_tree;
}

void Syntactic_Analyzer::continue_parsing(const Token& token_to_parse, tree_node* parent_node)
{
	current_toke = token_to_parse;
	switch (comp->current_rule.top())
	{
	case Program: { Program_parsing(); } break;
	case Begin:   { Begin_parsing(parent_node); }break;
	case End: { End_parsing(); } break;
	case Descriptions: { Descriptions_parsing(parent_node); } break;
	case Operators: { Operators_parsing(); } break;
	case Descr: { Descr_parsing(parent_node); } break;
	case Type: { Type_parsing(parent_node); } break;
	case VarList: { VarList_parsing(parent_node); } break;
	case Op: { Op_parsing(); } break;
	case Expr: { Expr_parsing(); } break;
	case Id: { Id_parsing(parent_node); } break;
	case Const: { Const_parsing(); } break;
	case Divider: { Divider_parsing(); } break;
	case Operator: { Operator_parsing(); } break;
	default:
		break;
	}
}


void Syntactic_Analyzer::Program_parsing()
{
	this->syntax_tree = new tree_node;
	
	syntax_tree->rule = comp->current_rule.top();

	comp->current_rule.push(rules::Begin);
	
	continue_parsing(current_toke,syntax_tree);
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


void Syntactic_Analyzer::Begin_parsing(tree_node* parent_node)
{
	if (comp->expected_token == Token::token_type::key_word)
	{
		++parsed_line_counter;
		tree_node begin_node;
		begin_node.rule = rules::Begin;

		if (current_toke.get_type() != Token::key_word)
		{
			std::string mes = "Expected keyword. " + current_toke.get_lexema() + " met.";
			output_error(mes);
		}
		if (current_toke.get_lexema() != "PROGRAM")
		{
			std::string mes = "Expected keyword <PROGRAM>. " + current_toke.get_lexema() + " met.";
			output_error(mes);
		}
		begin_node.data = current_toke.get_lexema();

		parent_node->childs.push_back(begin_node);


		comp->expected_token = Token::token_type::identifi_;

		comp->actual_node = &(parent_node->childs.back());
	}
	else if (comp->expected_token == Token::token_type::identifi_)
	{
		
		SKIP_SPACE

		comp->current_rule.push(rules::Id);
		continue_parsing(current_toke, parent_node);
		
		//end of begin parsing
		comp->current_rule.pop();

		//start of descriptions
		comp->current_rule.push(rules::Descriptions);
		comp->actual_node = syntax_tree;
	}
	else
	{
		std::string mes = "Unexpected token! Expected identifi of program. <" + current_toke.get_lexema() + "> met.";
		output_error(mes);
	}

}

void Syntactic_Analyzer::Id_parsing(tree_node* parent_node)
{

	tree_node id_node;
	id_node.rule = rules::Id;

	if (current_toke.get_type() != Token::identifi_)
	{
		std::string mes = "ID expected. " + current_toke.get_lexema() + " met.";
		output_error(mes);
	}

	id_node.data = current_toke.get_lexema();

	parent_node->childs.push_back(id_node);

	comp->current_rule.pop();

	
}

void Syntactic_Analyzer::Descriptions_parsing(tree_node* parent)
{
	tree_node descriptions_node;
	descriptions_node.rule = rules::Descriptions;

	parent->childs.push_back(descriptions_node);

	comp->expected_token = Token::token_type::key_word;
	comp->current_rule.push(rules::Descr);
	continue_parsing(current_toke, &parent->childs.back());
}

void Syntactic_Analyzer::Descr_parsing(tree_node* parent_node)
{

	++parsed_line_counter;
	tree_node descr_node;
	descr_node.rule = rules::Descr;

	parent_node->childs.push_back(descr_node);
	comp->current_rule.push(Type);

	continue_parsing(current_toke, &parent_node->childs.back());

	comp->expected_token = Token::token_type::identifi_;
	comp->current_rule.push(rules::VarList);
	comp->actual_node = &parent_node->childs.back();
}
void Syntactic_Analyzer::Type_parsing(tree_node* parent_node)
{
	tree_node type_node;
	type_node.rule = rules::Type;

	if (current_toke.get_type() != Token::key_word)
	{
		std::string mes = "Expected keyword. " + current_toke.get_lexema() + " " + " met.";
		output_error(mes);
	}

	if (current_toke.get_lexema() != "INTEGER" and current_toke.get_lexema() != "REAL")
	{
		std::string mes = "Expected <INTEGER/REAL>. " + current_toke.get_lexema() + " " + " met.";
		output_error(mes);
	}

	type_node.data = current_toke.get_lexema();

	parent_node->childs.push_back(type_node);

	//end type
	comp->current_rule.pop();
}

void Syntactic_Analyzer::VarList_parsing(tree_node* parent_node)
{

	if (comp->expected_token == Token::token_type::identifi_)
	{
		SKIP_SPACE
		
		tree_node var_list_node;
		var_list_node.rule = rules::VarList;

		parent_node->childs.push_back(var_list_node);

		comp->current_rule.push(rules::Id);
		continue_parsing(current_toke, &parent_node->childs.back());

		comp->expected_token = Token::token_type::divider_;
		comp->actual_node = &parent_node->childs.back();
	}
	else if (comp->expected_token == Token::token_type::divider_)
	{
		SKIP_SPACE

		if (current_toke.get_type() != Token::divider_) 
		{
			//end of varlist
			comp->current_rule.pop();
			
			//potential new descr
			comp->expected_token = Token::token_type::key_word;
			comp->actual_node = &syntax_tree->childs[1];

			continue_parsing(current_toke, comp->actual_node);

		}
		else if (current_toke.get_type() == comp->expected_token and current_toke.get_lexema() == ",")
		{
			//continue varlist

			comp->expected_token = Token::token_type::identifi_;
		}
		else
		{
			std::string mes = "Unexpected token! exprcted varlist ( list of indentifies ) or end of varlist";
			output_error(mes);
		}
	}
	else 
	{
		std::string mes = "Unexpected token! exprcted var ( identifi_ ) or varlist ( list of indentifies )";
		output_error(mes);
	}


	/*
	tree_node varList_node;
	varList_node.rule = "VarList:";

	varList_node.childs.push_back(Var_parsig());

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
	*/
}



Syntactic_Analyzer::tree_node Syntactic_Analyzer::Operators_parsing()
{
	/*
	tree_node operators_node;
	operators_node.rule = "Operators:";

	while (current_toke->get_type() == Token::identifi_)
	{
		operators_node.childs.push_back(Op_parsing());
	}

	return operators_node;
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::End_parsing()
{
	/*
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
	*/
	return tree_node();
}


Syntactic_Analyzer::tree_node Syntactic_Analyzer::Var_parsig()
{
	/*
	tree_node var_node;
	var_node.rule = "Var:";
	var_node.childs.push_back(Id_parsing());

	return var_node;
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Op_parsing()
{
/*
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
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Operator_parsing()
{
	/*
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
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Const_parsing()
{
	/*
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
	*/
	return tree_node();
}
Syntactic_Analyzer::tree_node Syntactic_Analyzer::Function_parsing()
{
	/*
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
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Expr_parsing()
{
	/*
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
	*/
	return tree_node();
}

Syntactic_Analyzer::tree_node Syntactic_Analyzer::Divider_parsing()
{
	/*
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
	*/
	return tree_node();
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

	if (this->rule == rules::Begin or this->rule == rules::Descr or this->rule == rules::Op or this->rule == rules::End)
	{
		show_parsed_str(*this); std::cout << '\n';
	}
}
