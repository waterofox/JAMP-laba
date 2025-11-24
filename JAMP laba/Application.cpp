#include "Application.h"

void Application::scan_code(const std::string& path)
{
	this->lex_obj->scan_code(token_hash_table, path);
	
	std::cout << "lexical analyzer end.\n";

	this->syntax_obj->Program_parsing();
	this->syntax_obj->syntax_tree->show_tree(0);
}


Application::Application()
{
	this->lex_obj = new Lexical_Analyzer(this->token_stream);
	this->syntax_obj = new Syntactic_Analyzer(this->token_stream);
}

Application::~Application()
{
	delete this->lex_obj;
	delete this->syntax_obj;
}
