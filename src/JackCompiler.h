#ifndef JACKCOMPILER_JACKCOMPILER_H
#define JACKCOMPILER_JACKCOMPILER_H

#include "template.h"

class JackCompiler {
public:
	static void compile(string);
private:
	static void compileFile(string, string, ofstream&);
};


#endif //P10_JACKCOMPILER_H
