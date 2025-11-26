#pragma once
#include <string>
#include <vector>
#include "../LEXICA/staff/Token.h"

class Syntactic_Analyzer
{
	struct tree_node
	{
		std::string rule; 
		std::vector<tree_node> childs;
		std::string data;

		void show_tree(int deep);
		void show_parsed_str(tree_node& node);
	};

public:
	const std::vector<Token>* token_stream = nullptr;
private:
	const Token* current_toke = nullptr;
	int parsed_line_counter = 0;

	void output_error(const std::string& error);
public:
	tree_node* syntax_tree = nullptr;
	

public:
	Syntactic_Analyzer(const std::vector<Token>& token_stream);
	~Syntactic_Analyzer();

	void Program_parsing();

private:

	//MAIN PARSERS
	tree_node Begin_parsing();
	tree_node Descriptions_parsing();
	tree_node Operators_parsing();
	tree_node End_parsing();

	//DESCRIPTIONS
	tree_node Descr_parsing();
	tree_node Type_parsing();
	tree_node VarList_parsing();
	tree_node Var_parsig();
	tree_node Id_parsing();
	
	//OPERATIONS
	tree_node Op_parsing();
	tree_node Operator_parsing();
	tree_node Expr_parsing();
	tree_node Const_parsing();
	tree_node Function_parsing();

	//NE PRIDUMAL CATEGORIY
	tree_node Divider_parsing();


};

