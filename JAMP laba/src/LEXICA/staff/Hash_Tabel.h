#pragma once
#include "Token.h"
#include <fstream>

#define data_categories 6

template <size_t reserved_place>
class Hash_Tabel
{
	friend std::ofstream& operator<<(std::ofstream& stream, const Hash_Tabel& tabel)
	{
		for (int i = 0; i < data_categories; ++i) 
		{
			switch (i)
			{
			case Token::token_type::constant: {std::cout << "constanta: "; } break;
			case Token::token_type::divider_: {std::cout << "divider: "; } break;
			case Token::token_type::function: {std::cout << "function: "; } break;
			case Token::token_type::key_word: {std::cout << "key word: "; } break;
			case Token::token_type::opErator: {std::cout << "operator: "; } break;
			case Token::token_type::identifi_: {std::cout << "identifi: "; } break;

			default:
				break;
			}
			std::cout << std::endl;

			for (int j = 0; j < reserved_place; ++j)
			{
				Hash_Node& cell = tabel.data[i][j];
				if (cell.first_token == nullptr) { continue; }

				Token_Node* actual_token = cell.first_token;

				std::cout << '\t' << j <<  ": " << actual_token->token->get_lexema() << ' ';

				while (actual_token->move_rigth != nullptr)
				{
					actual_token = actual_token->move_rigth;
					std::cout << '\t' << actual_token->token->get_lexema() << ' ';
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		return stream;
	}

private:
	struct Token_Node
	{
		Token* token = nullptr; //out token

		Token_Node* move_rigth = nullptr; //next token
		Token_Node* move_left  = nullptr; //prev token
	};
	struct Hash_Node
	{
		Token_Node* first_token= nullptr;
	};
public:
	struct Hash_Key
	{
		Token::token_type category;
		size_t index;
		size_t second_index = -1;
	};
private:
	
	static Hash_Key hash(const Token& token) //token hashing
	{
		Hash_Key key;
		//DJB2 HASH

		size_t hash = 5381; //seed

		for (int i = 0; i < token.get_lexema().size(); ++i)
		{
			const std::string& lexema = token.get_lexema();
			hash = hash * 33 + int(lexema[i]);
		}
		
		key.category = token.get_type();
		key.index = hash % reserved_place;

		return key;
	}
private:
	Hash_Node** data;
public:

	Hash_Tabel()
	{
		this->data = new Hash_Node * [data_categories];
		for (int i = 0; i < data_categories; ++i)
		{
			this->data[i] = new Hash_Node[reserved_place];
		}
	}
	~Hash_Tabel()
	{
		for (int i = 0; i < data_categories; ++i)
		{
			for (int j = 0; j < reserved_place; ++j)
			{
				Hash_Node& actual_node = data[i][j];

				Token_Node* actual_token = actual_node.first_token;
				if (actual_token == nullptr) { continue; }
				while (actual_token->move_rigth != nullptr)
				{
					actual_token = actual_token->move_rigth;
				}
				while (actual_token->move_left != nullptr)
				{
					//delete data
					delete actual_token->token;
					actual_token->move_rigth = nullptr;

					actual_token = actual_token->move_left;
					actual_token->move_rigth->move_left = nullptr;

					delete actual_token->move_rigth;
					actual_token->move_rigth = nullptr;
				}
				delete actual_token->token;
			}
			delete[] data[i];
		}
		delete[] data;
	}

	void add(const Token& token)
	{


		Hash_Key key = this->find(token);//hash(token);

		if (key.category != Token::token_type::not_found) { return; }

		key = hash(token);

		Hash_Node& cell = data[key.category][key.index];

		if (cell.first_token == nullptr) 
		{
			cell.first_token = new Token_Node;
		}
		Token_Node* actual_token = cell.first_token;

		while (actual_token->move_rigth != nullptr)
		{
			actual_token = actual_token->move_rigth;
		}

		if (actual_token->token != nullptr)
		{
			actual_token->move_rigth = new Token_Node;
			actual_token->move_rigth->move_left = actual_token;

			actual_token = actual_token->move_rigth;
			actual_token->token = new Token(token);
		}
		else
		{
			actual_token->token = new Token(token);
		}
	}
	Hash_Key find(const Token& token)
	{
		Hash_Key key = hash(token);
		
		Hash_Node& cell = data[key.category][key.index];

		if (cell.first_token == nullptr) { key.category = Token::token_type::not_found; return key; }

		Token_Node* actual_token = cell.first_token;
		if (*(actual_token->token) == token) { key.second_index = 0; return key; }
		size_t count = 0;
		while (actual_token->move_rigth != nullptr)
		{
			++count;
			actual_token = actual_token->move_rigth;

			if (*(actual_token->token) == token) { key.second_index = count; return key; }
		}

		key.category = Token::token_type::not_found; return key;

	}

	Token& operator[] (const Hash_Key& key)
	{
		Hash_Node& cell = data[key.category][key.index];
		Token_Node* move = cell.first_token;

		for (int i = 0; i < key.second_index; ++i)
		{
			move = move->move_rigth;
		}

		return *(move->token);
	}
};


