#include "Token.h"

Token::Token(const token_type& type, const std::string& lexema)
{
	this->type = type;
	this->lexema = lexema;
}

const Token::token_type& Token::get_type()
{
	return this->type;
}

const std::string& Token::get_lexema()
{
	return this->lexema;
}
