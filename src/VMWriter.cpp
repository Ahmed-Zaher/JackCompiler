#include "VMWriter.h"

void VMWriter::writePush(string seg, int idx) {
	out << ("push " + seg + " " + to_string(idx)) << '\n';
}

void VMWriter::writePop(string seg, int idx) {
	out << ("pop " + seg + " " + to_string(idx)) << '\n';
}

void VMWriter::writeArithmetic(string op) {
	out << op << '\n';
}

void VMWriter::writeLabel(string label) {
	out << ("label " + label) << '\n';
}

void VMWriter::writeGoTo(string label) {
	out << ("goto " + label) << '\n';
}

void VMWriter::writeIf(string label) {
	out << ("if-goto " + label) << '\n';
}

void VMWriter::writeCall(string fn, int nArgs) {
	out << ("call " + fn + " " + to_string(nArgs)) << '\n';
}

void VMWriter::writeFunction(string fn, int nLocals) {
	out << ("function " + fn + " " + to_string(nLocals)) << '\n';
}

void VMWriter::writeReturn() {
	out << "return" << '\n';
}
