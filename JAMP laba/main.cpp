#include "src/LEXICA/Lexical_Analyzer.h"


int main()
{
	Hash_Tabel<512> tabel;
	Lexical_Analyzer analyzer;
	analyzer.scan_code(tabel, "input.txt");



	return 0;
}