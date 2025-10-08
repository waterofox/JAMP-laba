#include "src/LEXICA/staff/Hash_Tabel.h"

int main()
{

	Hash_Tabel<32> tabel;

	Token token1(Token::token_type::key_word, "int");
	Token token2(Token::token_type::variant_, "jopa");
	Token token3(Token::token_type::constant, "12");
	Token token4(Token::token_type::divider_, ";");

	Token token5(Token::token_type::key_word, "int");
	Token token6(Token::token_type::opErator, "*");
	Token token7(Token::token_type::variant_, "pointer");
	Token token8(Token::token_type::opErator, "=");
	Token token9(Token::token_type::key_word, "nullptr");

	tabel.add(token1);
	tabel.add(token2);
	tabel.add(token3);
	tabel.add(token4);
	tabel.add(token5);
	tabel.add(token6);
	tabel.add(token7);
	tabel.add(token8);
	tabel.add(token9);

	std::cout << tabel;

	Hash_Tabel<32>::Hash_Key key;
	key = tabel.find(token9);

	Token test(tabel[key]);

	return 0;
}