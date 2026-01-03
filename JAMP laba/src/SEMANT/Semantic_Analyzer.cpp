#include "Semantic_Analyzer.h"
#include "../Compiler.h"
#include <string>

#define CONTINUE_EXPR

Semantic_Analyzer::Semantic_Analyzer(Syntactic_Analyzer::tree_node* tree)
{
	this->syntax_tree = tree;
}

Semantic_Analyzer::~Semantic_Analyzer()
{
}

void Semantic_Analyzer::scan_code(const Syntactic_Analyzer::tree_node& actual_node)
{

	if (actual_node.rule == rules::Begin)
	{
		this->PROGRAM_id = actual_node.childs.back().data;
		scan_code(syntax_tree->childs[1]); //Descriptions
	}
	else if (actual_node.rule == rules::End)
	{
		if (this->PROGRAM_id != actual_node.childs.back().data)
		{
			std::string mes = "End of undefined program! ";
			mes += "<END " + actual_node.childs.back().data;

			std::cout << mes << '\n';

			is_correct = false;
		}
		//exit rec
		return;

	}
	else if (actual_node.rule == rules::Descriptions)
	{
		//go to Descrs
		for (int i = 0; i < actual_node.childs.size(); ++i)
		{
			scan_code(actual_node.childs[i]);
		}

		//go to Operators
		scan_code(syntax_tree->childs[2]);

	}
	else if (actual_node.rule == rules::Descr)
	{
		if (actual_node.childs.front().data == "INTEGER")
		{
			current_descr = &INTEGER_vars;
		}
		else
		{
			current_descr = &REAL_vars;
		}
		
		//varlist start 
		scan_code(actual_node.childs.back());
	}
	else if (actual_node.rule == rules::VarList)
	{
		//add id
		std::string defined_id = actual_node.childs.front().data;
		
		Token check_token(Token::token_type::identifi_,defined_id);
		if (INTEGER_vars.find(check_token).category == Token::token_type::not_found and \
			REAL_vars.find(check_token).category == Token::token_type::not_found)
		{
			current_descr->add(check_token);
		}
		else
		{
			std::string mes = "Var is already defined! ";
			mes += "id: " + defined_id;

			std::cout << mes << '\n';
	
			is_correct = false;
		}

		if (actual_node.childs.size() == 1) 
		{
			//end of varlist
			return;
		}
		
		scan_code(actual_node.childs.back());

	}
	else if (actual_node.rule == rules::Operators)
	{
		for (int i = 0; i < actual_node.childs.size(); ++i)
		{
			//go to Op
			scan_code(actual_node.childs[i]);
		}

		//got to end
		scan_code(syntax_tree->childs[3]);
	}
	else if (actual_node.rule == rules::Op)
	{
		//check var
		const Syntactic_Analyzer::tree_node& var_node = actual_node.childs.front();
		
		Token check_token(Token::token_type::identifi_, var_node.data);

		if (INTEGER_vars.find(check_token).category != Token::token_type::not_found)
		{
			current_op_type = INTEGER;

			//go to expr
			scan_code(actual_node.childs.back());
		}
		else if (REAL_vars.find(check_token).category != Token::token_type::not_found)
		{
			current_op_type = REAL;

			//go to expr
			scan_code(actual_node.childs.back());
		}
		else
		{
			std::string mes = "Var is undefined! ";
			mes += "id: " + var_node.data;

			std::cout << mes << '\n';

			is_correct = false;
		}
	}
	else if (actual_node.rule == rules::Expr)
	{
		//checking all vars and consts in Expr

		var_types current_expr_type;
		if (checking_function) { current_expr_type = current_function_arg_type.top(); }
		else { current_expr_type = current_op_type; }

		for (int i = 0; i < actual_node.childs.size(); ++i)
		{
			const Syntactic_Analyzer::tree_node& pod_expr_node = actual_node.childs[i];
			if (pod_expr_node.rule == rules::Expr) 
			{
				//continue checking
				scan_code(pod_expr_node);
			}
			else if (pod_expr_node.rule == rules::Id)
			{
				const std::string& expr_var = pod_expr_node.data;

				Token check_token(Token::token_type::identifi_, expr_var);

				if (INTEGER_vars.find(check_token).category != Token::token_type::not_found)
				{
					if (current_expr_type != INTEGER)
					{
						std::string mes = "Incorrect Type! ";
						mes += "id: " + expr_var;

						std::cout << mes << '\n';

						is_correct = false;
					}
				}
				else if (REAL_vars.find(check_token).category != Token::token_type::not_found)
				{
					if (current_expr_type != REAL)
					{
						std::string mes = "Incorrect Type! ";
						mes += "id: " + expr_var;

						std::cout << mes << '\n';

						is_correct = false;
					}
				}
				else
				{
					std::string mes = "Var is undefined! ";
					mes += "id: " + expr_var;

					std::cout << mes << '\n';

					is_correct = false;
				}
			}
			else if (pod_expr_node.rule == rules::Const)
			{
				const std::string& expr_const = pod_expr_node.data;
				if (std::find(expr_const.begin(), expr_const.end(), '.') == expr_const.end())
				{
					//integer
					if (current_expr_type != INTEGER)
					{
						std::string mes = "Incorrect Type! ";
						mes += "const: " + expr_const;

						std::cout << mes << '\n';

						is_correct = false;
					}
				}
				else
				{
					//real
					if (current_expr_type != REAL)
					{
						std::string mes = "Incorrect Type! ";
						mes += "const: " + expr_const;

						std::cout << mes << '\n';

						is_correct = false;
					}
				}
			}
			else if (pod_expr_node.rule == Function)
			{
				var_types current_type;
				if (checking_function) { current_type = current_function_arg_type.top(); }
				else { current_type = current_op_type; }

				const std::string& func = pod_expr_node.data;
				if (func == "ITOR")
				{
					//itor retunr real
					if (current_type != REAL)
					{
						std::string mes = "Incorrect returned Type! ";
						mes += "function: " + func;

						std::cout << mes << '\n';

						is_correct = false;
					}

					//check expr of funtion
					current_function_arg_type.push(INTEGER);
					checking_function = true;
					scan_code(pod_expr_node.childs.back()); //expr
					current_function_arg_type.pop();
					checking_function = false;
				}
				else
				{
					//rtoi retunr integer
					if (current_type != INTEGER)
					{
						std::string mes = "Incorrect returned Type! ";
						mes += "function: " + func;

						std::cout << mes << '\n';

						is_correct = false;
					}

					//check expr of funtion
					current_function_arg_type.push(REAL);
					checking_function = true;
					scan_code(pod_expr_node.childs.back()); //expr
					current_function_arg_type.pop();
					checking_function = false;
				}
			}
		}
	}
}

std::string Semantic_Analyzer::get_notation_descrs(const Syntactic_Analyzer::tree_node& node, int& varlist_size,const std::string& str)
{
	if (node.rule == Descriptions)
	{
		std::string notation_descrs = "";

		for (int i = 0; i < node.childs.size(); ++i)
		{
			varlist_size = 0;
			std::string notation_decr = "";
			std::string rememberred_type;

			const Syntactic_Analyzer::tree_node& Descr = node.childs[i];
			
			rememberred_type = Descr.childs.front().data;

			//buld varlist

			notation_descrs = notation_descrs + rememberred_type + ' ' + get_notation_descrs(Descr.childs.back(), varlist_size, notation_decr) + ' ';
			notation_descrs = notation_descrs +  std::to_string(varlist_size + 1) + " DECL\n";
		}

		return str + notation_descrs;
	}
	else if (node.rule == VarList)
	{
		++varlist_size;
		std::string varlist_notation = "";

		varlist_notation += node.childs.front().data;
		if (node.childs.size() == 1) { return varlist_notation; }
		else
		{
			return varlist_notation + ' ' + get_notation_descrs(node.childs.back(), varlist_size, str);
		}
	}
}

std::string Semantic_Analyzer::get_notation_expr(const Syntactic_Analyzer::tree_node& node, const std::string& str)
{
	std::string notation_expr = "";
	
	if (node.childs.front().rule == rules::Id or node.childs.front().rule == rules::Const)
	{
		const Syntactic_Analyzer::tree_node& simp_node = node.childs.front();
		
		notation_expr += simp_node.data;

		if (node.childs.size() == 1) { return notation_expr; }
		else
		{
			if (node.childs[1].data == "+")
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " PLUS";
			}
			else
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " MINUS";
			}
		}
	}
	else if (node.childs.front().rule == rules::Divider)
	{
		notation_expr += get_notation_expr(node.childs[1], notation_expr);
		if (node.childs.size() == 3) { return notation_expr; } // ( EXPR )
		else
		{
			if (node.childs[1].data == "+")
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " PLUS";
			}
			else
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " MINUS";
			}
		}
	}
	else if (node.childs.front().rule == rules::Function)
	{
		const Syntactic_Analyzer::tree_node& function = node.childs.front();
		notation_expr += function.data;
		notation_expr = notation_expr + ' ' + get_notation_expr(function.childs.front(),notation_expr) + " 2 CALL";

		if (node.childs.size() == 1) { return notation_expr; }
		else
		{
			if (node.childs[1].data == "+")
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " PLUS";
			}
			else
			{
				return notation_expr + ' ' + get_notation_expr(node.childs.back(), notation_expr) + " MINUS";
			}
		}
	}


	return notation_expr;
}

std::string Semantic_Analyzer::get_notation_operators(const Syntactic_Analyzer::tree_node& node, const std::string& str)
{
	std::string notation_operators = "";
	for (int i = 0; i < node.childs.size(); ++i)
	{
		std::string notation_op = "";

		const Syntactic_Analyzer::tree_node& Op = node.childs[i];

		std::string expr_buffer = "";
		notation_operators = notation_operators + Op.childs.front().data + ' ' + get_notation_expr(Op.childs.back(), expr_buffer) + " ASSIGN\n";
	}

	return notation_operators;
}

