#pragma once

#include "src/LEXICA/Lexical_Analyzer.h"
#include "src/SYNTAX/Syntactic_Analyzer.h"
class Application
{
	Lexical_Analyzer* lex_obj = nullptr;
	Syntactic_Analyzer* syntax_obj = nullptr;

	std::vector<Token> token_stream;
	Hash_Tabel<512> token_hash_table;

public:
	void scan_code(const std::string& path);


	Application();
	~Application();

};

/*
*/