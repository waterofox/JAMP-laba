#pragma once
#include <string>
#include <vector>
#include <stack>
#include "../LEXICA/staff/Token.h"

class Compiler;
enum rules;

class Syntactic_Analyzer
{
	friend Compiler;
	struct tree_node
	{
		rules rule; 
		std::vector<tree_node> childs;
		std::string data;

		void show_tree(int deep);
		void show_parsed_str(tree_node& node);
	};

private:
	Token current_toke;
	int parsed_line_counter = 0;

	void output_error(const std::string& error);

	Compiler* comp = nullptr;
	std::stack<tree_node*> parenthesis;
public:
	tree_node* syntax_tree = nullptr;
	

public:
	Syntactic_Analyzer(Compiler* comp);
	~Syntactic_Analyzer();

	void continue_parsing(const Token& token_to_parse, tree_node* parent_node);
	
	void Program_parsing();

private:

	//MAIN PARSERS
	void Begin_parsing(tree_node* parent_node);
	void Descriptions_parsing(tree_node* parent);
	void Operators_parsing(tree_node* parent);
	tree_node End_parsing();

	//DESCRIPTIONS
	void Descr_parsing(tree_node* parent_node);
	void Type_parsing(tree_node* parent_node);
	void VarList_parsing(tree_node* parent_node);
	tree_node Var_parsig();
	void Id_parsing(tree_node* parent_node);
	
	//OPERATIONS
	void Op_parsing(tree_node* parent_node);
	void Operator_parsing(tree_node* parent_node);
	void Expr_parsing(tree_node* paretn_node);
	void Const_parsing(tree_node* parent_node);
	tree_node Function_parsing();

	//NE PRIDUMAL CATEGORIY
	void Divider_parsing(tree_node* parent_node);


};

