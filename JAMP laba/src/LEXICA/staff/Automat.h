#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>

#include "Token.h"

class Automat
{
	struct condition
	{
		int num;
		std::map<char, condition*> next_conditions;
		Token::token_type type = Token::not_type;

		~condition()
		{
			for (auto& el : next_conditions)
			{
				if (el.second == this) { el.second = nullptr; }
				else 
				{
					if (el.second != nullptr)
					{
						delete el.second;
						el.second = nullptr;
					}
				}
			}
		}
	};

	condition start_condition;
	
	//usefull conditions
	condition numerical_start_condition;
	condition numerical_end_condiiton;
	condition id_condition;
	condition help;
	condition end_help;

	//id of usefull conditions
	int id_state_num;
	std::pair<int,int> numerical_states;


public:
	Automat(const std::vector<std::pair<std::string,Token::token_type>>& lexems_vec);
	~Automat();
	Token operator() (const std::string& potential_lexema);
};

