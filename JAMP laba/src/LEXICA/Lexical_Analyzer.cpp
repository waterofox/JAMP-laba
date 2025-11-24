#include "Lexical_Analyzer.h"



#define att std::pair<std::string,Token::token_type>

Lexical_Analyzer::Lexical_Analyzer(std::vector<Token>& token_stream)
{
	dka = new Automat(std::vector<std::pair<std::string, Token::token_type>>{
		att("PROGRAM", Token::key_word),
			att("END", Token::key_word),
			att("PROGRAM", Token::key_word),
			att("INTEGER", Token::key_word),
			att("REAL", Token::key_word),
			att("=", Token::opErator),
			att("+", Token::opErator),
			att("-\0", Token::opErator),
			att("ITOR", Token::function),
			att("RTOI", Token::function),
	});

	this->token_stream = &token_stream;
}

Lexical_Analyzer::~Lexical_Analyzer()
{
	delete dka;
}

void Lexical_Analyzer::scan_code(Hash_Tabel<512>& tabel, const std::string& code_url)
{
	std::set<char> dividers = { ' ',',','(',')', '\0','\n','\r'};
	
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
		lexema_buffer += sign;

		if(dividers.count(sign) != 0)
		{
			if (lexema_buffer == "") { continue; }

			char divider_tmp = lexema_buffer.back();

			lexema_buffer.pop_back();

			log << "LEXEMA: " << lexema_buffer << '\n';
			Token new_token = (*dka)(lexema_buffer);

			lexema_buffer = "";

			if (new_token.get_type() == Token::not_type)
			{
				log << "INCORRECT LEXEMA\n\n";
			}
			else
			{
				log << '\n';
				tabel.add(new_token);
				token_stream->push_back(new_token);
			}

			if (divider_tmp != '\n' and divider_tmp != '\r' and divider_tmp != '\0')
			{
				std::string tmp = ""; tmp += divider_tmp;
				tabel.add(Token(Token::divider_, tmp));
				token_stream->push_back(Token(Token::divider_, tmp));
			}
			lexema_buffer = "";

		}
	}
	if (lexema_buffer != "")
	{
		char divider_tmp = lexema_buffer.back();

		log << "LEXEMA: " << lexema_buffer << '\n';
		Token new_token = (*dka)(lexema_buffer);

		lexema_buffer = "";

		if (new_token.get_type() == Token::not_type)
		{
			log << "INCORRECT LEXEMA\n\n";
		}
		else
		{
			log << '\n';
			tabel.add(new_token);
			token_stream->push_back(new_token);
		}
		lexema_buffer = "";
	}



	log.close();
	source.close();
	output << tabel;
	output.close();
}
