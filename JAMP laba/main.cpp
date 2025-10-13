#include "src/LEXICA/staff/Automat.h"

#define att std::pair<std::string,Token::token_type>

int main()
{
	Automat test_auto(std::vector<std::pair<std::string, Token::token_type>>{
		att("PROGRAM", Token::key_word),
			att("END", Token::key_word),
			att("PROGRAM", Token::key_word),
			att("INTIGER", Token::key_word),
			att("REAL", Token::key_word),
			att("=", Token::opErator),
			att("+", Token::opErator),
			att("-", Token::opErator),
			att("ITOR", Token::function),
			att("RTOI", Token::function),
	});

	return 0;
}