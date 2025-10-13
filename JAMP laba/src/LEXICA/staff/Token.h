#pragma once
#include <iostream>
#include <string>
class Token
{
public:
	enum token_type
	{
		key_word,
		identifi_,
		opErator,
		constant,
		divider_,
		function,

		not_type,
		not_found,
	};
private:
	token_type type;
	std::string lexema;
public:
	Token(const token_type& type, const std::string& lexema);

	const token_type& get_type() const;
	const std::string& get_lexema() const;

	bool operator==(const Token& token) const;
};	

