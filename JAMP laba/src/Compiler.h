#pragma once

#include "LEXICA/Lexical_Analyzer.h"
#include "SYNTAX/Syntactic_Analyzer.h"

enum rules
{
	//var 12
	Program,Begin,End,Descriptions,Operators,Descr, Type, VarList, Op, Expr, Id, Const,
	
	//sub rules
	Divider, Operator,  Function
};

class Compiler
{
	friend Syntactic_Analyzer;
private:
	//usefull fields
	Token::token_type expected_token = Token::token_type::not_type;
	std::stack<rules> current_rule;
	Syntactic_Analyzer::tree_node* actual_node = nullptr;
	
	Hash_Tabel<512> tokens_table;

	//Lex&Syn analyzers connection
public:
	void delegate_token(const Token& token);
private:
	Lexical_Analyzer*   lexica = nullptr;
	Syntactic_Analyzer* syntax = nullptr;

public:
	void compile_file(const std::string& file_path);

	Compiler();
	~Compiler();

};

