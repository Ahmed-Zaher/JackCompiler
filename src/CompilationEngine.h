#ifndef JACKCOMPILER_COMPILATIONENGINE_H
#define JACKCOMPILER_COMPILATIONENGINE_H

#include "template.h"
#include "SymbolTable.h"
#include "VMWriter.h"

class CompilationEngine {
private:
	int ptr, labelCnt;
	string className, curToken, curTokenClass;
	vector<pair<string, string>> tokens;
	SymbolTable symbols;
	ofstream &out;
	VMWriter vmWriter;

	void compileClass();

	void compileClassVarDec();

	void compileSubroutineDec();

	void compileParameterList(string);

	void compileSubroutineBody(string, string);

	int compileVarDec();

	void compileStatements();

	void compileLet();

	void compileIf();

	void compileWhile();

	void compileDo();

	void compileReturn();

	void compileExpression();

	void advance();

	void compileTerm();

	int compileExpressionList();

	void compileSubroutineCall();

	void popSymbol(string symbol);

	void pushSymbol(string symbol);

	void pushString(string);

	string peek(int);

	string getMemSeg(string);

	string getNextLabel() { return "___label_" + to_string(labelCnt++); }

public:
	CompilationEngine(vector<pair<string, string>>, ofstream &);

	void generate() {
		ptr = 0;
		curToken = tokens[0].first, curTokenClass = tokens[0].second;
		compileClass();
	}

};


#endif //JACKCOMPILER_COMPILATIONENGINE_H
