#include "Compiler.h"
#include <Windows.h>

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
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(h, 3);
	std::cout << "\n----------------------Semantic analyze start----------------------\n";
	SetConsoleTextAttribute(h, 6);

	semant->syntax_tree = syntax->syntax_tree;
	semant->scan_code(this->syntax->syntax_tree->childs.front());

	

	if (semant->is_correct)
	{
		SetConsoleTextAttribute(h, 2);
		std::cout << "Your code is fully correct!\n";
		SetConsoleTextAttribute(h, 7);

		std::string polish_n = "";
		int buffer = 0;

		polish_n = polish_n + syntax->syntax_tree->childs.front().childs.front().data + ' ' + syntax->syntax_tree->childs.front().data + '\n';
		polish_n = semant->get_notation_descrs(syntax->syntax_tree->childs[1],buffer,polish_n);
		polish_n += semant->get_notation_operators(syntax->syntax_tree->childs[2], polish_n);
		polish_n = polish_n + syntax->syntax_tree->childs.back().childs.front().data + ' ' + syntax->syntax_tree->childs.back().data + '\n';

		std::cout << polish_n;
	}
	else
	{
		SetConsoleTextAttribute(h, 4);
		std::cout << "Your code have some issues!\n";
		SetConsoleTextAttribute(h, 7);

	}

	 
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
