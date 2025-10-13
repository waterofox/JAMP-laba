#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "Token.h"

class Automat
{
	struct condition
	{
		int num;
		std::map<char, condition*> next_conditions;
		Token::token_type type = Token::not_type;
	};
	
	condition start_condition;
	
	condition numerical_start_condition;
	condition numerical_end_condiiton;
	condition id_condition;

	int id_state_num;
	//std::set<int> ended_states;
	std::pair<int,int> numerical_states;

public:
	Automat(const std::vector<std::pair<std::string,Token::token_type>>& lexems_vec);
	Token operator() (const std::string& potential_lexema);
};

