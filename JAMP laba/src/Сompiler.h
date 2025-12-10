#pragma once

#include "LEXICA/Lexical_Analyzer.h"
#include "SYNTAX/Syntactic_Analyzer.h"

class Compiler;

using connection = void(Compiler::*)(const Token&);

enum rules
{
	//var 12
	Program,Begin,End,Descriptions,Operators,Descr, Type, VarList, Op, Expr, Id, Const,
	
	//sub rules
	Divider, Operator,
};

class Ñompiler
{
private:
	//usefull fields
	Token::token_type expected_token = Token::token_type::not_type;
	std::stack<rules> current_rule;

	//Lex&Syn analyzers connection
public:
	void delegate_token(const Token& token);
private:
	Lexical_Analyzer*   lexica = nullptr;
	Syntactic_Analyzer* syntax = nullptr;

public:
	Ñompiler();
	~Ñompiler();

};

