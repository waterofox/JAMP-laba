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

	if (syntax->syntax_tree->childs.back().rule != rules::End)
	{
		std::string mes = "END had missed.";
		syntax->output_error(mes);
	}

	syntax->syntax_tree->show_tree(0);
	//move tree to analyse
	semant->syntax_tree = syntax->syntax_tree;
	semant->scan_code(this->syntax->syntax_tree->childs.front());
}

Compiler::Compiler()
{
	this->lexica = new Lexical_Analyzer(this);
	this->syntax = new Syntactic_Analyzer(this);
	this->semant = new Semantic_Analyzer(nullptr);
}

Compiler::~Compiler()
{
	delete lexica;
	delete syntax;
	delete semant;
}
