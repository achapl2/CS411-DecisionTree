#include "string.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Parser { 
	public:
		Parser(string filePath);
		string getFile(string filePath);
		string getNthAttrib(string attribs, int i);
		vector <vector <string>> getExamples();
		vector <vector <string>> getAttributesList();
		
		
	
	private:
		vector <vector <string>> attributesList = {{"DEFAULT"}};
		vector <string> exampleToAttribVector(string line);
		vector <vector <string>> exampleList = {{"DEFAULT"}};
		void   parseAttributesLine(string line);
		void   parseAttributesList();	
		void   parseExamples();
		void   parseLines();
		
		string myStrTok(string str, char delim);
		
		vector<string> lines = {"DEFAULT"};
		string fileStr;
};
