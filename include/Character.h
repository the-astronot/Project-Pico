
#ifndef __Character_h__
#define __Character_h__

#include <string>
//#include "bitf.h"

class Character {
private:
	char character;
	int num_used;
	std::string binary;
	bool delimiter;
	bool endoffile;
	bool endofcompfile;
	bool endoftitle;

public:

	Character *next;

	// Constructors
	Character(char inCharacter) {
		character = inCharacter;
		num_used = 2;
		next = nullptr;
		delimiter = false;
		endoffile = false;
		endofcompfile = false;
		endoftitle = false;
	}

	Character(bool Delimiter, bool EndOfFile,
		 				bool EndOfCompFile, bool EndOfTitle) {
		character = '\0';
		num_used = 1;
		next = nullptr;
		delimiter = Delimiter;
		endoffile = EndOfFile;
		endofcompfile = EndOfCompFile;
		endoftitle = EndOfTitle;
	}

	//Accessors
	int getTimesUsed() {return num_used;}
	char getCharacter() {return character;}
	std::string getBinary() {return binary;}
	bool isDelimiter() {return delimiter;}
	bool isEndofFile() {return endoffile;}
	bool isEndofCompFile() {return endofcompfile;}
	bool isEndofTitle() {return endoftitle;}

	// Setters
	void plusOne() {num_used++;}
	void setNum(int num) {num_used = num;}
	void setBinary(std::string Binary) {binary = Binary;}
};

void sort(Character* &first);

#endif
