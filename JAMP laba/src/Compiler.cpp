#include "Compiler.h"

void Compiler::delegate_token(const Token& token)
{
	//delegate to syntax analyzer
	syntax->continue_parsing(token,actual_node);
}

void Compiler::compile_file(const std::string& file_path)
{
	expected_token = Token::token_type::key_word;
	current_rule.push(rules::Program);
	lexica->scan_code(tokens_table,file_path);

	syntax->syntax_tree->show_tree(0);
}

Compiler::Compiler()
{
	this->lexica = new Lexical_Analyzer(this);
	this->syntax = new Syntactic_Analyzer(this);
}

Compiler::~Compiler()
{
	delete lexica;
	delete syntax;
}
