#ifndef __Node_h__
#define __Node_h__

#include <string>
#include <iostream>
#include "Character.h"

class Node {
private:
	std::string binary;
	int length;

public:
	Node* root;
	Node* left;
	Node* right;

	Node() {
		length = 0;
		this->binary = "";
	}

	Node(Node* Parent, bool left) {
		this->root = Parent;
		this->length = this->root->getLength() + 1;
		this->binary = this->root->getBinary();
		if (left) {
			this->binary = this->binary + "0";
		} else {
			this->binary = this->binary + "1";
		}

		std::cout << this->length << "->>" << this->binary << std::endl;
	}

	int getLength() {return this->length;}

	std::string getBinary() {return this->binary;}

};
/*
void assignBinary(Character* &firstChar, LeafInfo leaf_struct) {
	int* leaf_plot = leaf_struct.leaves;
	int num_leaves = leaf_struct.num_leaves;
	Character* currentChar = firstChar;
	Node *rootNode, *currentNode, *prevNode;
	std::cout << "Start" << std::endl;
	rootNode = new Node(NULL, false);
	currentNode = rootNode;
	while(currentChar != NULL && currentNode != NULL) {
		if(prevNode == currentNode->root) {
			// Branch to left
			std::cout << "LEFT" << std::endl;
			currentNode->left = new Node(currentNode,true);
			prevNode = currentNode;
			currentNode = currentNode->left;
		} else if (prevNode == currentNode->left) {
			// Branch to right
			std::cout << "RIGHT" << std::endl;
			currentNode->right = new Node(currentNode, false);
			prevNode = currentNode;
			currentNode = currentNode->right;
		} else {
			std::cout << "UP" << std::endl;
			// Climb back up a branch
			prevNode = currentNode;
			currentNode = currentNode->root;
		}
		std::cout << "Made it" << std::endl;
		// Check if Leaf
		if (*(leaf_plot+currentNode->getLength()) > 0) {
			std::cout << "YEP" <<std::endl;
			*(leaf_plot+currentNode->getLength());
			currentChar->setBinary(currentNode->getBinary());
			prevNode = currentNode;
			currentNode = currentNode->root;
			currentChar = currentChar->next;
		}
		std::cout << currentNode->getLength() << std::endl;
	}
}
*/

#endif
