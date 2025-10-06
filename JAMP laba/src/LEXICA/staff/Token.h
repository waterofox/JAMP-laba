#pragma once
#include <iostream>
#include <string>
class Token
{
public:
	enum class token_type
	{
		key_word,
		function,
		variant_,
		opErator,
		constant,
		divider_,
	};
private:
	token_type type;
	std::string lexema;
public:
	Token(const token_type& type, const std::string& lexema);

	const token_type& get_type();
	const std::string& get_lexema();
};	

