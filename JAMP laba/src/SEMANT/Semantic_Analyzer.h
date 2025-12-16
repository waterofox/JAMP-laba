#pragma once

#include "../SYNTAX/Syntactic_Analyzer.h"
#include <set>

class Compiler;

class Semantic_Analyzer
{
	friend Compiler;
	enum var_types
	{
		INTEGER, REAL
	};

	Syntactic_Analyzer::tree_node* syntax_tree = nullptr;
private:
	std::set<std::string> INTEGER_vars;
	std::set<std::string> REAL_vars;

	std::string PROGRAM_id;

	std::set<std::string>* current_descr = nullptr;
	var_types current_op_type;
	std::stack<var_types> current_function_arg_type;
	bool checking_function = false;

	

public:
	Semantic_Analyzer(Syntactic_Analyzer::tree_node* tree);
	~Semantic_Analyzer();

	void scan_code(const Syntactic_Analyzer::tree_node& actual_node);
};

