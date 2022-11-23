#include "Assignment11.h"
#include "Parser.h"
#include <math.h>
#include <iostream>
#include <cstdio>


using namespace std;

void printVector(vector<string> v) {
	for(int i = 0; i < v.size(); i++) {
		cout << " j: " << i << " " << v[i];
	}
	
	cout << endl;
}

void printVector2D(vector<vector<string>> v) {
	for(int i = 0; i < v.size(); i++) {
		printVector(v[i]);
	}
	cout << endl;
	
}

int main() {
	Parser* p = new Parser("restaurant.csv");
	vector<vector<string>> attributes = p->getAttributesList();
	vector<vector<string>> examples   = p->getExamples();
	
	Node* rootNode = learnDecisionTree(examples, attributes, {});
	outputTree(rootNode, 0, "", -1);	
	
	return 0;
}


void outputTree(Node* rootNode, int depth, string attrib, int attribNum) {
	
	for(int i = 0; i < depth; i++) {
		cout << "- ";
	}
	cout << attrib;
	
	
	if (rootNode->isLeaf()) {
		cout << " => " << rootNode->getleafAttrib() << endl;
		return;
	}
	cout << " | Split on attrib: " << rootNode->getChildren()[0].attribNum << endl;
	
	vector<Node::childPtr> children = rootNode->getChildren();
	
	for(int i = 0; i < children.size(); i++) {
		outputTree(children[i].ptr, depth+1, children[i].attrib, children[i].attribNum);
	}
	
}

bool attributesEmpty(vector<vector<string>> v) {
	for(int i = 0; i < v.size(); i++) {
		if (v[i][0].compare("") != 0) {
			return false;
		}
	}
	return true;
}


Node* learnDecisionTree(vector<vector<string>> examples, vector<vector<string>> attributes, vector<vector<string>> parentExamples) {
	
	if (examples.empty()) {
		return pluralityValue(parentExamples);
	} else if (uniformClassification(examples)) {
		return new Node(examples[0][examples[0].size() - 1]);
	} else if (attributesEmpty(attributes)) {
		return pluralityValue(examples);
	} else {
		Node* recursiveRoot = new Node(NULL, attributes, examples);
		int mostImportantAttrib = importance(recursiveRoot);
		vector<vector<string>> childrenAttribs = removeElementAt(attributes, mostImportantAttrib);
		vector<vector<string>> childExamples;
		
		for(int i = 0; i < attributes[mostImportantAttrib].size(); i++) {
			
			childExamples = isolateExamples(examples, attributes, mostImportantAttrib, i);
			recursiveRoot->addChild(learnDecisionTree(childExamples, childrenAttribs, examples), attributes[mostImportantAttrib][i], mostImportantAttrib, 1);
		}
		
		return recursiveRoot;
	}
		
	return 0x0;
}

vector<vector<string>> removeElementAt(vector<vector<string>> v, int i) {
	
	vector<vector<string>> returnVect;
	for(int j = 0; j < v.size(); j++){
		if (j != i) {
			returnVect.push_back(v[j]);
		} else {
			returnVect.push_back({""});
		}
	}
	return returnVect;
	
}


vector<vector<string>> isolateExamples(vector<vector<string>> examples, vector<vector<string>> attributes, int attribute, int concreteAttrib) {
	
	int numExamples = examples.size();
	int maxAttrib = examples[0].size() - 1;
	string attrib = attributes[attribute][concreteAttrib];
	//cout << "attrib: " << attrib << endl;
	//cout << "maxAttrib: " << maxAttrib << endl;
	
	vector<vector<string>> returnExamples;
	for(int i = 0; i < numExamples; i++) {
		if (examples[i][attribute].compare(attrib) == 0) {
			returnExamples.push_back(examples[i]);
		}
	}
	//cout << "returnVect.size() " << returnExamples.size() << endl;
	return returnExamples;
}

Node* pluralityValue(vector<vector<string>> v) {
	
	struct attribCount{
		string attrib;
		int count = 0;
	};
	
	vector<attribCount> attribCounts;
	string mostFreqOutcome = "";
	
	bool contains = false;
	for(int i = 0; i < v.size(); i++) {
		contains = false;
		for(int j = 0; j < attribCounts.size(); j++) {
			if (attribCounts[j].attrib.compare(v[i][v[i].size()-1]) == 0) {
				attribCounts[j].count++;
				contains = true;
				break;
			}
		}
		if (!contains) {
			attribCounts.push_back({v[i][v[i].size()-1], 1});
		}
	}
	int maxVal   = -1;
	int maxIndex = -1;
	for(int i = 0; i < attribCounts.size(); i++) {
		if (attribCounts[i].count > maxVal) {
			maxVal = attribCounts[i].count;
			maxIndex = i;
		}
	}
	
	Node* n = new Node(attribCounts[maxIndex].attrib);
	return n;

}


int importance(Node n) {
	int biggestGainIndex = -1;
	double biggestGain = -1;
	
	int numAttributes = n.getAttributes().size();
	for(int i = 0; i < numAttributes - 1; i++) {
		if (n.getAttributes()[i][0].compare("") != 0) {
			Node* newNode = new Node(n);
			newNode->makeChildren(i);
			int ig = infoGain(&n, newNode);
			
			if (ig > biggestGain) {
				biggestGain = ig;
				biggestGainIndex = i;
			}
		}
	}
	return biggestGainIndex;
}


bool uniformClassification(vector<vector<string>> examples) {
	string classification = examples[0][examples[0].size()-1];
	int numExamples = examples.size();
	for(int i = 0; i < numExamples; i++) {
		if (examples[i][examples[i].size()-1].compare(classification) != 0) {
			return false;
		}
	}
	
	return true;
}


double infoGain(Node origNode, Node* possibleSplitNode) {
	// Note: expand children of possibleSplitNode before calling
	
	
	int rem = remainder(possibleSplitNode);
	return boolEntropy(&origNode) - rem;
}


// Entropy
double boolEntropy (Node* n) {
	struct attribCountPair {
		string attrib;
		int count;
	};
	vector<attribCountPair> attribCounts;
	vector<vector<string>> examples = n->getExamples();
	int examplesSize = examples.size();
	if (examples.size() > 0) {
		int exampleMaxAttrib = examples[0].size() - 1;
		double total = 0;
		bool contains = false;
		
		// For current node, get likelihoods of each final outcome
		for(int i = 0; i < examplesSize; i++) {
			string currAttrib = examples[i][exampleMaxAttrib];
			contains = false;
			for(int j = 0; j < attribCounts.size(); j++) {
				if (attribCounts[j].attrib.compare(currAttrib) == 0) {
					attribCounts[j].count++;
					contains = true;
					break;
				}
			}
			if (!contains) {
				attribCounts.push_back({currAttrib, 1});
			}
		}
		
		for(int i = 0; i < attribCounts.size(); i++) {
			
			double prob = (double) attribCounts[i].count / (double) examplesSize;
			total += prob*(log(1.0/prob)/ log(2.0));
		}
	

		double prob = 0;
		if (attribCounts[0].attrib.compare("Yes") == 0) {
			prob = (double) attribCounts[0].count / (double) examplesSize;
		} else{
			prob = 1 - (double) attribCounts[0].count / (double) examplesSize;
		}
		int prob1 = 0;
		int prob2 = 0;
		if (prob != 0.0) {
			prob1 = prob*(log((double) (1.0/prob))/ log(2.0));
		}
		if (prob != 0.0) {
			prob2 = (1-prob)*(log((double) (1.0/(1-prob)))/ log(2.0));
		}
		total = prob1 + prob2;
		return total;
	} else {
		return 0;
	}
}


double remainder(Node* n) {
	double total = 0;
	vector<Node::childPtr> nChildren = n->getChildren();
	int numChildren = nChildren.size();
	int numParentExamples = n->getExamples().size();
	
	for(int i = 0; i < numChildren; i++) {
		total += ((double) nChildren[i].ptr->getExamples().size() / (double) n->getExamples().size())*boolEntropy(nChildren[i].ptr);
	}
	return total;
}