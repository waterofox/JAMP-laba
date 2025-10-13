#include "Lexical_Analyzer.h"

#define att std::pair<std::string,Token::token_type>

Lexical_Analyzer::Lexical_Analyzer()
{
	dka = new Automat(std::vector<std::pair<std::string, Token::token_type>>{
		att("PROGRAM", Token::key_word),
			att("END", Token::key_word),
			att("PROGRAM", Token::key_word),
			att("INTGER", Token::key_word),
			att("REAL", Token::key_word),
			att("=", Token::opErator),
			att("+", Token::opErator),
			att("-", Token::opErator),
			att("ITOR", Token::function),
			att("RTOI", Token::function),
	});
}

void Lexical_Analyzer::scan_code(Hash_Tabel<512>& tabel, const std::string& code_url)
{
	std::set<char> dividers = { ' ',',','(',')', '\0','\n','\r',};
	
	std::ofstream output("resualt.txt");
	std::ofstream log("log.txt");

	std::ifstream source(code_url);
	
	if (!source.is_open())
	{

		log << "source not found\n";
		log.close();
		output.close();
		source.close();

		return;
	}
	char sign;
	std::string lexema_buffer = "";

	while (source.get(sign))
	{
		if(dividers.count(sign) == 0)
		{
			lexema_buffer += sign;
			
		}
		else
		{
			std::string div_buf = "";
			div_buf += sign;
			Token new_token(Token::divider_,div_buf);
			tabel.add(new_token);

			if (lexema_buffer == "") { continue; }

			log << "LEXEMA: " << lexema_buffer << '\n';
			Token new_token2 = (*dka)(lexema_buffer);
			lexema_buffer = "";
			if (new_token2.get_type() == Token::not_type)
			{
				log << "INCORRECT LEXEMA\n\n";
			}
			else
			{
				log << '\n';
				tabel.add(new_token2);
			}
		}
	}
	log.close();
	source.close();
	output << tabel;
	output.close();
}
