#pragma once

#include "../SYNTAX/Syntactic_Analyzer.h"
#include "../LEXICA/staff/Hash_Tabel.h"

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
	//data checkers

	Hash_Tabel<64> INTEGER_vars;
	Hash_Tabel<64> REAL_vars;
	std::string PROGRAM_id;
	/*
			std::set<std::string> INTEGER_vars;
			std::set<std::string> REAL_vars;
	*/

	//recursy datacheckers 
	Hash_Tabel<64>* current_descr = nullptr;
	var_types current_op_type;
	std::stack<var_types> current_function_arg_type;
	bool checking_function = false;

	bool is_correct = true;

public:
	Semantic_Analyzer(Syntactic_Analyzer::tree_node* tree);
	~Semantic_Analyzer();

	void scan_code(const Syntactic_Analyzer::tree_node& actual_node);
	std::string get_notation_descrs(const Syntactic_Analyzer::tree_node& node, int& varlist_size, const std::string& str);
	std::string get_notation_operators(const Syntactic_Analyzer::tree_node& node, const std::string& str);
	std::string get_notation_expr(const Syntactic_Analyzer::tree_node& node, const std::string& str);
};

