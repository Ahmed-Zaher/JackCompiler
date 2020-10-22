#ifndef JACKCOMPILER_VMWRITER_H
#define JACKCOMPILER_VMWRITER_H

#include "template.h"

class VMWriter {
private:
	ofstream& out;
public:
	VMWriter(ofstream& out) : out(out) {}
	void writePush(string, int);
	void writePop(string, int);
	void writeArithmetic(string);
	void writeLabel(string);
	void writeGoTo(string);
	void writeIf(string);
	void writeCall(string, int);
	void writeFunction(string, int);
	void writeReturn();

};


#endif //JACKCOMPILER_VMWRITER_H
