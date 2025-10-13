#include "Automat.h"

Automat::Automat(const std::vector<std::pair<std::string, Token::token_type>>& lexems_vec)
{
	int conditions_counter = 0;
	start_condition.num = conditions_counter;

	for (int i = 0; i < lexems_vec.size(); ++i)
	{
		condition* cond = &start_condition;
		const std::string& lexema = lexems_vec[i].first;
		for (int j = 0; j < lexema.size(); ++j)
		{
			const char& a_sign = lexema[j];

			if (cond->next_conditions.find(a_sign) != cond->next_conditions.end())
			{
				cond = cond->next_conditions[a_sign];
			}
			else
			{
				++conditions_counter;
				cond->next_conditions[a_sign] = new condition;
				cond = cond->next_conditions[a_sign];
				cond->num = conditions_counter;
			}
		}
		cond->type = lexems_vec[i].second;
	}

	std::string alp = "0123456789.";


	for (int i = 0; i < alp.size()-1; ++i)
	{
		start_condition.next_conditions[alp[i]] = &numerical_start_condition;
	}

	++conditions_counter;
	numerical_start_condition.num = conditions_counter;
	numerical_start_condition.type = Token::constant;
	for (int i = 0; i < alp.size() - 1; ++i)
	{
		numerical_start_condition.next_conditions[alp[i]] = &numerical_start_condition;
	}
	numerical_start_condition.next_conditions['.'] = &numerical_end_condiiton;

	numerical_states.first = conditions_counter;
	
	++conditions_counter;
	numerical_end_condiiton.num = conditions_counter;
	numerical_end_condiiton.type = Token::constant;
	for (int i = 0; i < alp.size() - 1; ++i)
	{
		numerical_end_condiiton.next_conditions[alp[i]] = &numerical_end_condiiton;
	}

	numerical_states.second = conditions_counter;

	++conditions_counter;
	alp = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	id_condition.num = conditions_counter;
	id_condition.type = Token::identifi_;
	for (int i = 0; i < alp.size(); ++i)
	{
		id_condition.next_conditions[alp[i]] = &id_condition;
	}
	id_state_num = conditions_counter;

}


Token Automat::operator()(const std::string& potential_lexema)
{
	condition* cond = &start_condition;

	for (int i = 0; i < potential_lexema.size(); ++i)
	{
		const char& a_sign = potential_lexema[i];
		
		if (cond->next_conditions.find(a_sign) == cond->next_conditions.end())
		{
			if (cond->num == 0)
			{
				if (a_sign == '.') { return Token(Token::not_type, potential_lexema); }
				else { cond = &id_condition; }
			}
			else if (cond->num == id_state_num or \
				     cond->num == numerical_states.first or \
					 cond->num == numerical_states.second)
			{
				return Token(Token::not_type, potential_lexema);
			}
			else
			{
				cond = &id_condition;
			}
		}
		else
		{
			cond = cond->next_conditions[a_sign];
		}
	}

	return Token(cond->type, potential_lexema);
}
