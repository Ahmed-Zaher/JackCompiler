#include "CompilationEngine.h"
#include "template.h"

void CompilationEngine::compileClass() {
	advance();
	className = curToken;
	advance();
	advance();

	while (curToken == "static" || curToken == "field")
		compileClassVarDec();

	while (curToken == "constructor" || curToken == "function" || curToken == "method")
		compileSubroutineDec();

	advance();
}

void CompilationEngine::compileClassVarDec() {
	string type = peek(1), kind = curToken;
	symbols.add(peek(2), type, kind);
	advance();
	advance();
	advance();

	while (curToken == ",") {
		advance();
		symbols.add(curToken, type, kind);
		advance();
	}
	advance();
}

void CompilationEngine::compileSubroutineDec() {
	symbols.clearLocals();
	string subroutineType = curToken;
	advance();
	advance();
	string subroutineName = curToken;
	advance();
	advance();
	compileParameterList(subroutineType);
	advance();
	compileSubroutineBody(subroutineName, subroutineType);
}

void CompilationEngine::compileParameterList(string subroutineType) {
	if (subroutineType == "method")
		symbols.add("this", className, "argument");

	if (curToken != ")") {
		symbols.add(peek(1), curToken, "argument");
		advance();
		advance();

		while (curToken == ",") {
			advance();
			symbols.add(peek(1), curToken, "argument");
			advance();
			advance();
		}
	}
}

void CompilationEngine::compileSubroutineBody(string subroutineName, string subroutineType) {
	advance();
	int nLocals = 0;

	while (curToken == "var")
		nLocals += compileVarDec();

	vmWriter.writeFunction(className + "." + subroutineName, nLocals);

	if (subroutineType == "constructor") {
		vmWriter.writePush("constant", symbols.kindCnt("field"));
		vmWriter.writeCall("Memory.alloc", 1);
		vmWriter.writePop("pointer", 0);
	}
	if (subroutineType == "method") {
		vmWriter.writePush("argument", 0);
		vmWriter.writePop("pointer", 0);
	}
	compileStatements();
	advance();

}

int CompilationEngine::compileVarDec() {
	int ret = 1;
	string type = peek(1), kind = "local";
	symbols.add(peek(2), type, kind);
	advance();
	advance();
	advance();

	while (curToken == ",") {
		advance();
		symbols.add(curToken, type, kind);
		advance();
		++ret;
	}
	advance();
	return ret;
}

void CompilationEngine::compileStatements() {
	while (curToken == "let" || curToken == "if" || curToken == "while" || curToken == "do" || curToken == "return") {
		if (curToken == "let")
			compileLet();
		if (curToken == "if")
			compileIf();
		if (curToken == "while")
			compileWhile();
		if (curToken == "do")
			compileDo();
		if (curToken == "return")
			compileReturn();
	}
}

void CompilationEngine::compileLet() {
	advance();
	string varName = curToken;
	advance();

	if (curToken == "[") {  // deal with assignment to a location within an array
		pushSymbol(varName);
		advance();
		compileExpression();
		advance();
		vmWriter.writeArithmetic("add");
		advance();
		compileExpression();
		advance();
		vmWriter.writePop("temp", 0);
		vmWriter.writePop("pointer", 1);
		vmWriter.writePush("temp", 0);
		vmWriter.writePop("that", 0);
	} else {
		advance();
		compileExpression();
		advance();
		popSymbol(varName);
	}
}

void CompilationEngine::compileIf() {
	string L1 = getNextLabel(), L2 = getNextLabel();
	advance();
	advance();
	compileExpression();
	advance();
	advance();
	vmWriter.writeArithmetic("not");
	vmWriter.writeIf(L1);
	compileStatements();
	advance();
	vmWriter.writeGoTo(L2);
	vmWriter.writeLabel(L1);

	if (curToken == "else") {
		advance();
		advance();
		compileStatements();
		advance();
	}
	vmWriter.writeLabel(L2);
}

void CompilationEngine::compileWhile() {
	string L1 = getNextLabel(), L2 = getNextLabel();
	advance();
	advance();
	vmWriter.writeLabel(L1);
	compileExpression();
	vmWriter.writeArithmetic("not");
	vmWriter.writeIf(L2);
	advance();
	advance();
	compileStatements();
	advance();
	vmWriter.writeGoTo(L1);
	vmWriter.writeLabel(L2);
}

void CompilationEngine::compileDo() {
	advance();
	compileSubroutineCall();
	advance();
	vmWriter.writePop("temp", 1);
}

void CompilationEngine::compileSubroutineCall() {
	if (peek(1) == "(") {   // call to a subroutine within the class
		string fnName = className + "." + curToken;
		int nArgs = 0;

		if (symbols.isMethod(curToken)) {
			++nArgs;
			vmWriter.writePush("pointer", 0);
		}
		advance();
		advance();
		nArgs += compileExpressionList();
		advance();
		vmWriter.writeCall(fnName, nArgs);
	} else {
		string callerName = curToken;
		advance();
		advance();
		string fnName;
		int nArgs = 0;
		if (symbols.exist(callerName)) {   // method call on an object
			++nArgs;
			pushSymbol(callerName);
			fnName = symbols.typeOf(callerName) + "." + curToken;
		} else
			fnName = callerName + "." + curToken;

		advance();
		advance();
		nArgs += compileExpressionList();
		advance();
		vmWriter.writeCall(fnName, nArgs);
	}
}

void CompilationEngine::compileReturn() {
	bool isVoid = true;
	advance();

	if (curToken != ";")
		compileExpression(), isVoid = false;

	advance();

	if (isVoid)
		vmWriter.writePush("constant", 0);

	vmWriter.writeReturn();
}

void CompilationEngine::compileExpression() {
	compileTerm();
	while (string("+-*/&|<>=").find(curToken) != string::npos) {
		string op = curToken;
		advance();
		compileTerm();

		if (op == "+")
			vmWriter.writeArithmetic("add");
		if (op == "-")
			vmWriter.writeArithmetic("sub");
		if (op == "*")
			vmWriter.writeCall("Math.multiply", 2);
		if (op == "/")
			vmWriter.writeCall("Math.divide", 2);
		if (op == "&")
			vmWriter.writeArithmetic("and");
		if (op == "|")
			vmWriter.writeArithmetic("or");
		if (op == "<")
			vmWriter.writeArithmetic("lt");
		if (op == ">")
			vmWriter.writeArithmetic("gt");
		if (op == "=")
			vmWriter.writeArithmetic("eq");
	}
}

void CompilationEngine::compileTerm() {
	if (curToken == "(") {
		advance();
		compileExpression();
		advance();
	} else if (curToken == "-" || curToken == "~") {
		string unaryOp = curToken;
		advance();
		compileTerm();
		if (unaryOp == "-")
			vmWriter.writeArithmetic("neg");
		else
			vmWriter.writeArithmetic("not");
	} else if (peek(1) == "(" || peek(1) == ".") {
		compileSubroutineCall();
	} else if (peek(1) == "[") {
		string arrayName = curToken;
		string memSeg = symbols.kindOf(arrayName);
		int idx = symbols.indexOf(arrayName);
		if (memSeg == "field")
			memSeg = "this";
		vmWriter.writePush(memSeg, idx);
		advance();
		advance();
		compileExpression();
		advance();
		vmWriter.writeArithmetic("add");
		vmWriter.writePop("pointer", 1);
		vmWriter.writePush("that", 0);
	} else {

		if (curTokenClass == "integerConstant")
			vmWriter.writePush("constant", stoi(curToken));
		else if (curToken == "true") {
			vmWriter.writePush("constant", 1);
			vmWriter.writeArithmetic("neg");
		} else if (curToken == "false" || curToken == "null")
			vmWriter.writePush("constant", 0);
		else if (curToken == "this")
			vmWriter.writePush("pointer", 0);
		else if (symbols.exist(curToken))
			pushSymbol(curToken);
		else    // stringConstant
			pushString(curToken);
		advance();

	}
}

void CompilationEngine::pushString(string str) {
	vmWriter.writePush("constant", str.size());
	vmWriter.writeCall("String.new", 1);
	vmWriter.writePop("temp", 2);

	for (auto &c : str) {
		vmWriter.writePush("temp", 2);
		vmWriter.writePush("constant", int(c));
		vmWriter.writeCall("String.appendChar", 2);
		vmWriter.writePop("temp", 2);
	}
	vmWriter.writePush("temp", 2);
}

int CompilationEngine::compileExpressionList() {
	int ret = 0;
	if (curToken != ")") {
		compileExpression();
		++ret;
		while (curToken == ",") {
			advance();
			compileExpression();
			++ret;
		}
	}
	return ret;
}

void CompilationEngine::advance() {
	if (++ptr < tokens.size())
		curToken = tokens[ptr].first, curTokenClass = tokens[ptr].second;
}

CompilationEngine::CompilationEngine(vector<pair<string, string>> tokens, ofstream &out)
		: tokens(tokens), out(out), symbols(SymbolTable()), vmWriter(VMWriter(out)), labelCnt(0) {

	for (int i = 0; i < tokens.size(); ++i)
		if (tokens[i].first == "method")
			symbols.addMethod(tokens[i + 2].first);
}

string CompilationEngine::peek(int i) {
	return tokens[ptr + i].first;
}

void CompilationEngine::popSymbol(string symbol) {
	vmWriter.writePop(getMemSeg(symbol), symbols.indexOf(symbol));
}

void CompilationEngine::pushSymbol(string symbol) {
	vmWriter.writePush(getMemSeg(symbol), symbols.indexOf(symbol));
}

string CompilationEngine::getMemSeg(string symbol)  {
	string memSeg = symbols.kindOf(symbol);
	if (memSeg == "field")
		memSeg = "this";
	return memSeg;
}

