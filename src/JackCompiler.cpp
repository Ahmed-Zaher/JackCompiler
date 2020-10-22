#include "template.h"
#include "JackCompiler.h"
#include "JackTokenizer.h"
#include "CompilationEngine.h"
#include <dirent.h>

void JackCompiler::compileFile(string directory, string fileName, ofstream& out) {
	db(directory, fileName);
	string inputFile = directory + fileName + ".jack";
	JackTokenizer tokenizer;
	CompilationEngine codeGenerator(tokenizer.tokenize(inputFile), out);
	codeGenerator.generate();
}

void JackCompiler::compile(string arg) {
	while (arg.back() == '/')
		arg.pop_back();

	string path = "";
	vector<string> fileNames;
	string outFile;
	if (arg.size() >= 5 && arg.substr(arg.size() - 5) == ".jack") { // we are given a file
		for (int i = arg.size() - 6; i >= 0; --i) {
			if (!isalnum(arg[i])) {
				fileNames.push_back(arg.substr(i + 1));
				path = arg.substr(0, i + 1);
				break;
			}
		}
		if (path.empty())
			fileNames.push_back(arg);
		for (int i = 0; i < 5; ++i)
			fileNames.back().pop_back();
	} else { // we are given a directory
		path = arg;
		if (isalnum(path.back()))
			path += '/';
		{
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir((arg).c_str())) != NULL) {
				while ((ent = readdir(dir)) != NULL) {
					string fileName = ent->d_name;
					if (fileName.size() > 5 && fileName.substr(fileName.size() - 5) == ".jack")
						fileNames.push_back(fileName.substr(0, fileName.size() - 5));
				}
				closedir(dir);
			} else {
				cout << "Error opening file\n";
				exit(0);
			}
		}
	}

	for (auto &fileName : fileNames) {
		ofstream out(path + fileName + ".vm");
		compileFile(path, fileName, out);
	}
}

int main(int arc, char *argv[]) {
#ifdef LOCAL
	string s;
//	getline(cin, s);
	s = "D:\\courses\\current\\nand2tetris\\nand2tetris\\projects\\11\\Pong\\";
#else
	string s = argv[1];
#endif

	JackCompiler::compile(s);
	return 0;
}