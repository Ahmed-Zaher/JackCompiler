#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	localSymbols.clear();
	methods.clear();
	cnt.clear();
	index.clear();
	kind.clear();
	type.clear();
}

int SymbolTable::kindCnt(string kindName) {
	return cnt[kindName];
}

void SymbolTable::add(string symbolName, string symbolType, string symbolKind) {
	index[symbolName] = cnt[symbolKind]++;
	type[symbolName] = symbolType;
	kind[symbolName] = symbolKind;
	if (symbolKind == "local" || symbolKind == "argument")
		localSymbols.insert(symbolName);
}

string SymbolTable::kindOf(string symbolName) {
	return kind[symbolName];
}

string SymbolTable::typeOf(string symbolName) {
	return type[symbolName];
}

int SymbolTable::indexOf(string symbolName) {
	return index[symbolName];
}

void SymbolTable::clearLocals() {
	for (auto &symbol : localSymbols)
		removeSymbol(symbol);
	localSymbols.clear();
}

void SymbolTable::removeSymbol(string symbolName) {
	--cnt[kind[symbolName]];
	type.erase(symbolName);
	kind.erase(symbolName);
	index.erase(symbolName);
}

bool SymbolTable::exist(string symbolName) {
	return present(index, symbolName);
}

bool SymbolTable::isMethod(string subroutineName) {
	return present(methods, subroutineName);
}

void SymbolTable::addMethod(string methodName) {
	methods.insert(methodName);
}
