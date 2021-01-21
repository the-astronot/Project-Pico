#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "FileRead.h"
#include "FileWrite.h"
#include "bitf.h"
#include "Character.h"
#include "Tree.h"
#include "leaff.h"


typedef unsigned int uint;


void mapping(std::vector<std::string> filenames, Character* &first_Char);
void getInputFiles();


int main() {
	std::vector<std::string> filenames;
	std::string infilename0 = "../test_files/large_corpus/E.coli";
	std::string infilename1 = "../test_files/large_corpus/bible.txt";
	std::string infilename2 = "../test_files/large_corpus/world192.txt";
	std::string outfilename = "../compressed.jrm";
	filenames.push_back(infilename1);
	filenames.push_back(infilename0);
	filenames.push_back(infilename2);

	bool print = true;
	Character* first_Char = nullptr;
	Character* current = nullptr;
	char bits[8];
	mapping(filenames, first_Char);

	/*
	bool not_file_end = true;
	bool infile_exists;
	bool outfile_exists;
	int buffersize = 32;
	char single_char;
	char* inbuffer = new char[buffersize];
	char* outbuffer = new char[buffersize*2];

  FileRead* reader = new FileRead();
	infile_exists = reader->setFileName(infilename);
	FileWrite* writer = new FileWrite();
	outfile_exists = writer->setFileName(outfilename);
	if(infile_exists && outfile_exists) {
		while(not_file_end) {
			not_file_end = reader->readBuffer();
			inbuffer = reader->getBuffer();
			for(int i=0; i<buffersize; i++) {
				std::cout << *(inbuffer + i);
			}
			writer->writeBuffer(inbuffer, buffersize);
		}
	}
	writer->closeFile();
	*/

	current = first_Char;
	while (current != nullptr && print) {
		bitf::toBinary(current->getCharacter(), bits);
		std::cout << current->getCharacter() << ":" << bits << ":" << current->getTimesUsed() << ":" << current->getBinary() << std::endl;
		current = current->next;
	}

	return 0;
}


void mapping(std::vector<std::string> filenames, Character* &first_Char) {
	bool print = false;
	Character* current_Char = nullptr;
	char byte;
	bool create_new;
	bool infile_exists;
	bool eof;
	int num_chars;
	FileRead* freqreader = new FileRead();

	for(uint i=0; i<filenames.size(); i++) {
		infile_exists = freqreader->setFileName(filenames[i]);
		std::cout << filenames[i] << std::endl;
		if(infile_exists) {
			std::cout << "File Exists" << std::endl;
			eof = false;
			while(!eof) {
				eof = !(freqreader->readChar());
				byte = freqreader->getChar();
				//std::cout << byte; //<< std::endl;
				create_new = true;
				if (first_Char == nullptr) {
					first_Char = new Character(byte);
				} else {
					current_Char = first_Char;
					while(true) {
						//std::cout << "In Loop" << std::endl;
						if (current_Char->getCharacter() == byte) {
							//std::cout << "Character Exists" << std::endl;
							current_Char->plusOne();
							create_new = false;
							break;
						} else {
							if (current_Char->next == nullptr) {break;}
							else {current_Char = current_Char->next;}
						}
					}
					if (create_new) {
						//std::cout << "New Character" << std::endl;
						current_Char->next = new Character(byte);
					}
				}
		  }

			//sort(first_Char);
			std::cout << "Finished File" << std::endl;

			current_Char = first_Char;
			num_chars = 0;
			while(current_Char!=nullptr) {
				current_Char = current_Char->next;
				num_chars++;
			}

			std::cout << num_chars << std::endl;
		}
	}

	sort(first_Char);

	Tree* tree = new Tree(num_chars, first_Char, print);
	LeafInfo best_leaves = tree->getBestLeaves();

	std::cout << "[";
	for(int i = 1; i <best_leaves.num_leaves; i++) {
		std::cout << *(best_leaves.leaves+i) << " ";
	}
	std::cout << "]" << std::endl;

	leaff::assignBinary(first_Char, best_leaves);

	//sort(first_Char);
}
