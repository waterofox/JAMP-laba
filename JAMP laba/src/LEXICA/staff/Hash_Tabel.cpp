#include "Hash_Tabel.h"


template <size_t reserved_place>
Hash_Tabel<reserved_place>::Hash_Tabel<reserved_place>()
{
	this->data = new Hash_Node * [data_categories];
	for (int i = 0; i < data_categories; ++i)
	{
		this->data[i] = new Hash_Node[reserved_place];
	}
}

template<size_t reserved_place>
Hash_Tabel<reserved_place>::~Hash_Tabel()
{
	for (int i = 0; i < data_categories;++i)
	{
		for (int j = 0; j < reserved_place; ++i)
		{
			Hash_Node* actual_node = data[i][j];

			Token_Node* actual_token = actual_node->first_token;
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

template<size_t reserved_place>
void Hash_Tabel<reserved_place>::add(const Token& token)
{
	Hash_Key key = hash(token);
	
	Hash_Node* category = data[key.category];
	
	Token_Node* move = category[key.index].first_token;
	while (move->move_rigth != nullptr)
	{
		move = move->move_rigth;
	}
	move->move_rigth = new Token_Node;
	move->move_rigth->move_left = move;
	move = move->move_rigth;
	move->token = new Token(token);
}

template<size_t reserved_place>
Token& Hash_Tabel<reserved_place>::operator[](const Hash_Key& key)
{
	Hash_Node* cell= data[key.category][key.index];
	Token_Node* move = cell->first_token;

	for (int i = 0; i < key.second_index;++i)
	{
		move = move->move_rigth;
	}

	return *(move->token);
}
