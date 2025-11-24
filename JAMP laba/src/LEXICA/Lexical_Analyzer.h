#pragma once

#include <fstream>

#include "staff/Automat.h"
#include "staff/Hash_Tabel.h"

class Lexical_Analyzer
{
	Automat* dka = nullptr;
	std::vector<Token>* token_stream = nullptr;

public:
	Lexical_Analyzer(std::vector<Token>& token_stream);
	~Lexical_Analyzer();

	void scan_code(Hash_Tabel<512>& tabel, const std::string& code_url);
};

