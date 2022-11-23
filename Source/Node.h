#ifndef NODE_H
#define NODE_H

//#include "childPtr.h"

#include <vector>
#include <string>

using namespace std;



class Node {
	

	public:
	
		struct childPtr{
			Node* ptr;
			string attrib;
			int attribNum;
			double weight;
		};
		
		Node(Node* parent, vector <vector <string>> attributes, vector<vector<string>> examples, string splitAttrib);
		Node(Node* parent, vector <vector <string>> attributes, vector<vector<string>> examples);
		Node(Node* n);
		Node(string leafAttrib);
		
		void addChild(Node* child, string attrib, int attribNum, double weight);
		void setSplitAttrib(string splitAttrib) {this->splitAttrib = splitAttrib;}
		
		vector<vector<string>> getAttributes() {return attributes;}
		vector<vector<string>> getExamples() {return examples;}
		vector<string>         getchildConcreteAttributes() {return attributes[childAttribIndex];}
		int getChildAttribIndex(){return childAttribIndex;}
		vector<childPtr>       getChildren() {return children;}
		string				   getSplitAttrib() {return splitAttrib;}
		string				   getleafAttrib() {return leafAttrib;}
		Node*				   getParent() {return parent;}
		bool			       isLeaf() {return isleaf;}
		
		Node* makeChildNode(int attributeIndex, int concreteAttributeIndex);
		
		void  makeChildren(int attributeIndex);
		
	private:
		
		vector<vector<string>> makeChildExamples(int attributeIndex, int concreteAttributeIndex);
		
		
		vector <vector <string>> attributes;
		//int parentAttribIndex; // Parent uses this attribute index to divide into children
		int childAttribIndex; // Uses this attribute index to divide into children
		vector <vector <string>> examples;
		vector <childPtr> children;
		//string assignedConcreteAttribute; // Same as attrib childPtr pointing to this node
		Node* parent;
		bool isleaf = false;
		string leafAttrib;
		string splitAttrib;
};

#endif