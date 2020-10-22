#ifndef JACKCOMPILER_JACKTOKENIZER_H
#define JACKCOMPILER_JACKTOKENIZER_H

#include "template.h"

class JackTokenizer {
private:

	const set<string> keywords{"class", "constructor", "function",
	                           "method", "field", "static", "var", "int",
	                           "char", "boolean", "void", "true", "false",
	                           "null", "this", "let", "do", "if", "else",
	                           "while", "return"};
	const set<string> symbols{"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*",
	                          "/", "&", "|", "<", ">", "=", "~"};
	const set<char> whitespace{' ', '\n', '\t', 13};

	bool isIdentifier(string);

	bool isInteger(string);

	bool isKeyword(string);

	bool isSymbol(string);

	vector<string> getClasses(string);

	string read(const string &);

public:
	vector<pair<string, string>> tokenize(const string &nameOfFile);
};


#endif //JACKCOMPILER_JACKTOKENIZER_H
