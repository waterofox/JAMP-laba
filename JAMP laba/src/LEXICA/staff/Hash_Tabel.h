#pragma once
#include "Token.h"

#define data_categories 6

template <size_t reserved_place>
class Hash_Tabel
{
private:
	struct Token_Node
	{
		Token* token = nullptr; //out token

		Token* move_rigth = nullptr; //next token
		Token* move_left  = nullptr; //prev token
	};
	struct Hash_Node
	{
		Token_Node* first_token= nullptr;
	};
	struct Hash_Key
	{
		Token::token_type category;
		size_t index;
		size_t second_index = -1;
	};
	
	static Hash_Key hash(const Token& token) //token hashing
	{
		Hash_Key key;
		return key;
	}
private:
	Hash_Node** data;
public:

	Hash_Tabel();
	~Hash_Tabel();

	void add (const Token& token);
	Hash_Key find(const Token& token)
	{
		Hash_Key key = hash(token);

		Hash_Node* category = data[key.category];

		Token_Node* move = category[key.index].first_token;
		int count = 0;
		while (move->move_rigth != nullptr)
		{
			if (token == *(move->token)) { key.second_index = count;  return key; }
			move = move->move_rigth;
			++count;
		}

		key.category = Token::token_type::not_found;
		return key;
	}

	Token& operator[] (const Hash_Key& key);
};


