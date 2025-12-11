#include "Token.h"

Token::Token(const token_type& type, const std::string& lexema)
{
	this->type = type;
	this->lexema = lexema;
}

Token::Token()
{

}

const Token::token_type& Token::get_type() const
{
	return this->type;
}

const std::string& Token::get_lexema() const
{
	return this->lexema;
}

bool Token::operator==(const Token& token) const
{
	return (this->type == token.type) and (this->lexema == token.lexema);
}
