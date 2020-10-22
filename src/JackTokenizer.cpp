#include "JackTokenizer.h"
#include "template.h"


bool JackTokenizer::isIdentifier(string str) {

	if (!(str[0] == '_' || isalpha(str[0])))
		return false;

	for (int i = 1; i < str.length(); i++) {
		if (!(str[i] == '_' || isalnum(str[i])))
			return false;

	}
	return true;
}

bool JackTokenizer::isInteger(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

bool JackTokenizer::isKeyword(string str) { return present(keywords, str); }

bool JackTokenizer::isSymbol(string str) { return present(symbols, str); }

vector<string> JackTokenizer::getClasses(string str) {
	vector<string> ret;

	if (isSymbol(str))
		ret.push_back("symbol");

	if (isInteger(str))
		ret.push_back("integerConstant");

	if (isKeyword(str))
		ret.push_back("keyword");

	if (isIdentifier(str))
		ret.push_back("identifier");

	return ret;
}

string JackTokenizer::read(const string &fileName) {
	string ret = "";
	ifstream ifs(fileName);
	char c = ifs.get();

	while (ifs.good()) {
		ret += c;
		c = ifs.get();
	}

	ifs.close();
	return ret;
}

vector<pair<string, string>> JackTokenizer::tokenize(const string &fileName) {

	string src = read(fileName);


	vector<pair<string, string>> tokens;

	vector<string> candidateClasses, nextCandidateClasses;

	string lexeme = "";

	for (int i = 0; i < src.size(); ++i) {

		// removing white space
		if (present(whitespace, src[i])) {
			if (!lexeme.empty()) {
				tokens.push_back(make_pair(lexeme, candidateClasses[0]));
				lexeme = "";
			}
			continue;
		}

		// removing /* ... */ comments
		if (i + 1 < src.size() && src[i] == '/' && src[i + 1] == '*') {
			bool good = false;

			for (int j = i + 2; j < src.size(); ++j)
				if (j + 1 < src.size() && src[j] == '*' && src[j + 1] == '/') {
//					tokens.push_back(make_pair(src.substr(i, j - i + 2), "commment"));
					i = j + 1;
					good = true;
					break;
				}
			continue;
		}

		// removing line comments
		if (i + 1 < src.size() && src[i] == '/' && src[i + 1] == '/') {

			for (int j = i + 2; j < src.size(); ++j)
				if (src[j] == '\n') {
//					tokens.push_back(make_pair(src.substr(i, j - i), "commment"));
					i = j;
					break;
				}

			continue;
		}

		// handling strings
		if (src[i] == '\"') {
			bool good = false;

			for (int j = i + 1; j < src.size(); ++j)
				if (src[j] == '\"') {
					tokens.push_back(make_pair(src.substr(i + 1, j - i - 1), "stringConstant"));
					i = j;
					good = true;
					break;
				}
			continue;
		}

		lexeme += src[i];

		// check whether we have a complete valid token

		candidateClasses = getClasses(lexeme);

		if (i + 1 == src.size())
			tokens.push_back(make_pair(lexeme, candidateClasses[0]));
		else {

			lexeme += src[i + 1];
			nextCandidateClasses = getClasses(lexeme);
			lexeme.pop_back();

			if (!candidateClasses.empty() && nextCandidateClasses.empty()) {
				tokens.push_back(make_pair(lexeme, candidateClasses[0]));
				lexeme = "";
			}
		}
	}
	return tokens;
}
