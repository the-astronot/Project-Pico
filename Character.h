
#ifndef __Character_h__
#define __Character_h__

#include <string>
#include "bitf.h"

class Character {
private:
	char character;
	int num_used;
	std::string binary;

public:

	Character *next;

	// Constructor
	Character(char inCharacter) {
		this->character = inCharacter;
		this->num_used = 1;
	}

	//Accessors
	int getTimesUsed() {return this->num_used;}
	char getCharacter() {return this->character;}
	std::string getBinary() {return this->binary;}

	// Setters
	void plusOne() {this->num_used++;}
	void setBinary(std::string binary) {this->binary = binary;}
	//void reset() {this->num_used = 0;}
};

void sort(Character* &first) {
	Character* current = first;
	Character* temp;
	Character* previous;
	while(current->next!=NULL) {
		if (current->getTimesUsed() >= current->next->getTimesUsed()) {
			previous = current;
			current = current->next;
		} else {
			temp = current->next;
			if (previous != NULL) {
				previous->next = current->next;
			} else {
					first = current->next;
			}
			current->next = temp->next;
			temp->next = current;
			current = first;
			previous = NULL;
		}
	}
}


#endif
