#ifndef JACKCOMPILER_SYMBOLTABLE_H
#define JACKCOMPILER_SYMBOLTABLE_H

#include "template.h"

class SymbolTable {
private:

	set<string> localSymbols, methods;
	map<string, int> cnt, index;
	map<string, string> kind, type;

	void removeSymbol(string);

public:
	SymbolTable();

	int kindCnt(string);

	void add(string, string, string);

	void addMethod(string);

	string kindOf(string);

	string typeOf(string);

	int indexOf(string);

	void clearLocals();

	bool exist(string);

	bool isMethod(string);
};


#endif //JACKCOMPILER_SYMBOLTABLE_H
