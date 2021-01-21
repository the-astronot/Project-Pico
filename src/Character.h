
#ifndef __Character_h__
#define __Character_h__

#include <string>
//#include "bitf.h"

class Character {
private:
	char character;
	int num_used;
	std::string binary;

public:

	Character *next;

	// Constructor
	Character(char inCharacter) {
		character = inCharacter;
		num_used = 2;
		next = nullptr;
	}

	//Accessors
	int getTimesUsed() {return num_used;}
	char getCharacter() {return character;}
	std::string getBinary() {return binary;}

	// Setters
	void plusOne() {num_used++;}
	void setNum(int num) {num_used = num;}
	void setBinary(std::string Binary) {binary = Binary;}
	//void reset() {this->num_used = 0;}
};

void sort(Character* &first);

#endif
