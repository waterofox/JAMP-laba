#include "Semantic_Analyzer.h"
#include "../Compiler.h"

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
		}
		//exit rec
		std::cout << "Your code is fully correct!\n";
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
		if (INTEGER_vars.find(defined_id) == INTEGER_vars.end() and \
			REAL_vars.find(defined_id) == REAL_vars.end())
		{
			current_descr->insert(defined_id);
		}
		else
		{
			std::string mes = "Var is already defined! ";
			mes += "id: " + defined_id;

			std::cout << mes << '\n';
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
		
		if (INTEGER_vars.find(var_node.data) != INTEGER_vars.end())
		{
			current_op_type = INTEGER;

			//go to expr
			scan_code(actual_node.childs.back());
		}
		else if (REAL_vars.find(var_node.data) != REAL_vars.end())
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
				if (INTEGER_vars.find(expr_var) != INTEGER_vars.end())
				{
					if (current_expr_type != INTEGER)
					{
						std::string mes = "Incorrect Type! ";
						mes += "id: " + expr_var;

						std::cout << mes << '\n';
					}
				}
				else if (REAL_vars.find(expr_var) != REAL_vars.end())
				{
					if (current_expr_type != REAL)
					{
						std::string mes = "Incorrect Type! ";
						mes += "id: " + expr_var;

						std::cout << mes << '\n';
					}
				}
				else
				{
					std::string mes = "Var is undefined! ";
					mes += "id: " + expr_var;

					std::cout << mes << '\n';
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
