#include "Node.h"
vector<vector<string>> isolateExamples(vector<vector<string>> examples, vector<vector<string>> attributes, int attribute, int concreteAttrib);
vector<vector<string>> removeElementAt(vector<vector<string>> v, int i);
Node* learnDecisionTree(vector<vector<string>> examples, vector<vector<string>> attributes, vector<vector<string>> parentExamples);
Node* pluralityValue(vector<vector<string>> parentExamples);

bool   uniformClassification(vector<vector<string>> examples);
double infoGain(Node parent, Node* child);
void outputTree(Node* rootNode, int depth, string attrib, int attribNum);
double boolEntropy (Node* n);
int    importance(Node n);
double remainder(Node* n);
