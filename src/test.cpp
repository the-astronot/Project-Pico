#include <iostream>
#include <fstream>
#include <string>
//#include "FileRead.h"
#include "bitf.h"
#include "Character.h"
#include "Tree.h"
#include "leaff.h"


int main() {

	bool print = true;
  std::string filename = "../test_files/tetris.txt";
	//std::cout << "ENTER FILENAME: ";
	//std::cin >> filename;

  std::ifstream rf(filename, std::ios::in);
  if(!rf) {
    std::cout << "Cannot open file!" << std::endl;
  	return 1;
  } else {
    std::cout << "File opens!" << std::endl;
  }

  char byte;
  char bits[8];
	Character* first_Char;
	Character* current_Char;
	bool create_new;
	int num_chars = 0;

  while(!(rf.eof())) {
    rf.read((char*) &byte, sizeof(byte));
		create_new = true;
		if (first_Char == NULL) {
			first_Char = new Character(byte);
		} else {
			current_Char = first_Char;
			while(true) {
				if (current_Char->getCharacter() == byte) {
					current_Char->plusOne();
					create_new = false;
					break;
				} else {
					if (current_Char->next == NULL) {break;}
					else {current_Char = current_Char->next;}
				}
			}
			if (create_new) {
				current_Char->next = new Character(byte);
			}
		}
  }

  rf.close();


	current_Char = first_Char;
	while(current_Char!=NULL) {
		current_Char = current_Char->next;
		num_chars++;
	}
	//current_Char->next = new Character();
	//current_Char->next->setNum(num_chars);
	//num_chars++;

	sort(first_Char);

	Tree* tree = new Tree(num_chars, first_Char, print);
	LeafInfo best_leaves = tree->getBestLeaves();
/*
	std::cout << "[";
	for(int i = 0; i <best_leaves.num_leaves; i++) {
		std::cout << *(best_leaves.leaves+i) << " ";
	}
	std::cout << "]" << std::endl;
*/
	leaff::assignBinary(first_Char, best_leaves);

	current_Char = first_Char;
	while (current_Char != NULL && print) {
		std::cout << current_Char->getCharacter() << ":" << current_Char->getTimesUsed() << ":" << current_Char->getBinary() << std::endl;
		current_Char = current_Char->next;
	}


/*
  char byte;
  int bit;
  char bits[8];

  std::cin >> byte;

  char hex[2];

  bitf::toHex(byte,hex);

  std::cout << "HEX: 0x" << hex[0] << hex[1] << std::endl;

  bitf::toBinary(byte,bits);

  std::cout << "BINARY: " << bits << std::endl;


  return 0;
*/
}
