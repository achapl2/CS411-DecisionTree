#include "Node.h"
#include <iostream>

using namespace std;


Node::Node(Node* parent, vector <vector <string>> attributes, vector<vector<string>> examples, string splitAttrib) {
	this->splitAttrib = splitAttrib;
	this->attributes = attributes;
	this->examples = examples;	
	this->parent = parent;
}

Node::Node(Node* parent, vector <vector <string>> attributes, vector<vector<string>> examples) {
	this->attributes = attributes;
	this->examples = examples;	
	this->parent = parent;
}

Node::Node(Node* n) {
	this->attributes = n->attributes;
	this->examples = n->examples;	
	this->parent = n->parent;
}

Node::Node(string leafAttrib) {
	this->isleaf = true;
	this->leafAttrib = leafAttrib;
}

void Node::addChild(Node* child, string attrib, int attribNum, double weight) {	
	children.push_back({child, attrib, attribNum, weight});
}


void Node::makeChildren(int attributeIndex){
	this->childAttribIndex = attributeIndex;
	for(int i = 0; i < attributes[attributeIndex].size(); i++) {
		addChild(makeChildNode(attributeIndex, i), attributes[attributeIndex][i], attributeIndex, 1);
	}
}


Node* Node::makeChildNode(int attributeIndex, int concreteAttributeIndex) {
	
	vector<vector<string>> childAttributes;
	string childConcreteAttribute = this->getAttributes()[attributeIndex][concreteAttributeIndex];
	for (int i = 0; i < this->getAttributes().size(); i++) {
		if (i != attributeIndex) {
			childAttributes.push_back(this->getAttributes()[i]);
		} else {
		childAttributes.push_back({""});
		}
	}
	
	vector<vector<string>> childExamples = makeChildExamples(attributeIndex, concreteAttributeIndex);
	Node* child = new Node(this, childAttributes, childExamples, childConcreteAttribute);
	return child;
	
}

vector<vector<string>> Node::makeChildExamples(int attributeIndex, int concreteAttributeIndex) {
	vector<vector<string>> examplesVector;
	
	int numParentExamples = this->examples.size();
	int exampleSize = this->examples[0].size();
	for(int i = 0; i < numParentExamples; i++) {
		if (this->examples[i][attributeIndex].compare(this->attributes[attributeIndex][concreteAttributeIndex]) == 0) {
			examplesVector.push_back(examples[i]);
		}
	}
	
	return examplesVector;
	
}