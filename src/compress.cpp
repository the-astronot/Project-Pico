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


void getInputFiles();
void mapping(std::vector<std::string> filenames, Character* &first_Char);
void writeHeader(FileWrite* &headerwriter, Character* first_Char);
void writeBody(FileWrite* &writer, std::vector<std::string> filenames,
	 							Character* first_Char);


int main() {
	std::vector<std::string> filenames;
	std::string infilename0 = "../test_files/large_corpus/E.coli";
	std::string infilename1 = "../test_files/large_corpus/bible.txt";
	std::string infilename2 = "../test_files/large_corpus/world192.txt";
	std::string outfilename = "../compressed.pico";
	filenames.push_back(infilename0);
	filenames.push_back(infilename1);
	filenames.push_back(infilename2);

	bool print = true;
	Character* first_Char = nullptr;
	Character* current = nullptr;
	FileWrite* writer = new FileWrite();

	mapping(filenames, first_Char);
	writer->setFileName(outfilename);
	writeHeader(writer, first_Char);
	writeBody(writer, filenames, first_Char);

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

	/*
	current = first_Char;
	while (current != nullptr && print) {
		if (current->getCharacter() != '\0'){
			bitf::toBinary(current->getCharacter(), bits);
			std::cout << current->getCharacter();
			std::cout << ":" << bits;
		} else {
			if(current->isDelimiter()) {
				std::cout << "Del" << ":";
				std::cout << "......";
			}
			if(current->isEndofFile()) {
				std::cout << "EOF" << ":";
				std::cout << "......";
			}
			if(current->isEndofTitle()) {
				std::cout << "EOT" << ":";
				std::cout << "......";
			}
			if(current->isEndofCompFile()) {
				std::cout << "EOCF" << ":";
				std::cout << ".....";
			}
		}
		std::cout << ":" << current->getTimesUsed();
		std::cout << ":" << current->getBinary() << std::endl;
		current = current->next;
	}
	*/
	return 0;
}


// Code to Assign Binary Strings to All Chaaracters in the Files
void mapping(std::vector<std::string> filenames, Character* &first_Char) {
	bool print = false;
	Character* current_Char = nullptr;
	char byte;
	bool create_new;
	bool infile_exists;
	bool eof;
	int num_chars;
	int files_added = 0;
	FileRead* freqreader = new FileRead();

	for(uint i=0; i<filenames.size(); i++) {
		infile_exists = freqreader->setFileName(filenames[i]);
		std::cout << filenames[i] << std::endl;
		if(infile_exists) {
			std::cout << "File Exists" << std::endl;
			eof = false;
			files_added++;
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
		}
	}
	// Count em up
	current_Char = first_Char;
	num_chars = 1;
	while(current_Char->next!=nullptr) {
		current_Char = current_Char->next;
		num_chars++;
	}

	// Add End of Compressed File Character
	current_Char->next = new Character(false,false,true,false);
	current_Char = current_Char->next;
	num_chars++;
	// Add End of Title Characters
	current_Char->next = new Character(false,false,false,true);
	current_Char = current_Char->next;
	current_Char->setNum(files_added);
	num_chars++;
	// Add End of Individual File Characters
	current_Char->next = new Character(false,true,false,false);
	current_Char = current_Char->next;
	current_Char->setNum(files_added);
	num_chars++;
	//Add Delimiter
	current_Char->next = new Character(true,false,false,false);
	current_Char = current_Char->next;
	current_Char->setNum(num_chars+2);
	num_chars++;

	std::cout << num_chars << std::endl;

	sort(first_Char);

	// Establish and Improve Tree, Assign Binary Strings
	Tree* tree = new Tree(num_chars, first_Char, print);
	LeafInfo best_leaves = tree->getBestLeaves();
/*
	std::cout << "[";
	for(int i = 1; i <best_leaves.num_leaves; i++) {
		std::cout << *(best_leaves.leaves+i) << " ";
	}
	std::cout << "]" << std::endl;
*/
	leaff::assignBinary(first_Char, best_leaves);
}


// Code to Write the Dictionary-Defining Header of the File
void writeHeader(FileWrite* &headerwriter, Character* first_Char) {
	Character* current_Char = first_Char;
	Character* del = nullptr;
	int del_size;
	int buffer_size = 32;
	std::string strbuffer = "1";
	std::string delimiter;
	char* buffer_array = new char[buffer_size];
	int byte_index, bit_index, str_index, max_bytes;
	bool skip_ascii;

	// Find the Delimiter
	while(current_Char!=nullptr && del==nullptr) {
		if(current_Char->isDelimiter()) {del = current_Char;}
		current_Char = current_Char->next;
	}
	// Set up the Header with the Delimiter
	del_size = (del->getBinary()).size();
	delimiter = del->getBinary();
	for(int i=0; i<del_size; i++) {
		strbuffer += "0";
	}
	strbuffer += "1";
	strbuffer += delimiter;

	current_Char = first_Char;
	while(current_Char!=nullptr) {
		if(!(current_Char->isDelimiter())) {
			skip_ascii = false;
			strbuffer += "0"; // Is not end of header
			// Check for non chars
			if(current_Char->isEndofTitle()) {
				skip_ascii = true;
				strbuffer += "1";
			} else {strbuffer += "0";}
			if(current_Char->isEndofFile()) {
				skip_ascii = true;
				strbuffer += "1";
			} else {strbuffer += "0";}
			if(current_Char->isEndofCompFile()) {
				skip_ascii = true;
				strbuffer += "1";
			} else {strbuffer += "0";}

			// Do the character thing
			if(!skip_ascii) {
				strbuffer += bitf::toBinaryString(current_Char->getCharacter());
			}
			strbuffer += current_Char->getBinary();
			strbuffer += delimiter;
		}
		current_Char = current_Char->next;
		if (current_Char==nullptr) {strbuffer += "1000";} //End of Header Indicator

		// Check if filled buffer -> write to file
		if (strbuffer.size() >= buffer_size*8) {
			for(byte_index=0;byte_index<buffer_size;byte_index++) {
				for(bit_index=0;bit_index<8;bit_index++) {
					str_index = byte_index*8+bit_index;
					if(strbuffer[str_index] == '1') {
						bitf::set(buffer_array[byte_index],7-bit_index);
					} else {
						bitf::unset(buffer_array[byte_index],7-bit_index);
					}
				}
			}
			headerwriter->writeBuffer(buffer_array, buffer_size);
			strbuffer = strbuffer.substr(buffer_size*8);
			std::cout << strbuffer << std::endl;
		}
	}
	// Finish up
	// Fill out trailing byte with zeroes, append to file
	max_bytes = strbuffer.size()/8 + 1;
	std::cout << strbuffer.size() << ":" << max_bytes << std::endl;
	std::cout << strbuffer << std::endl;
	for(byte_index=0;byte_index<max_bytes;byte_index++) {
		for(bit_index=0;bit_index<8;bit_index++) {
			str_index = byte_index*8+bit_index;
			if(str_index>=strbuffer.size()) {
				bitf::unset(buffer_array[byte_index],7-bit_index);
			} else if(strbuffer[str_index] == '1') {
				bitf::set(buffer_array[byte_index],7-bit_index);
			} else {
				bitf::unset(buffer_array[byte_index],7-bit_index);
			}
		}
	}
	headerwriter->writeBuffer(buffer_array, max_bytes);
}


// Writes the Body of the File
void writeBody(FileWrite* &writer, std::vector<std::string> filenames,
	 							Character* first_Char) {

	Character* current_Char;
	Character* title, *eoF, *eoCF;
	FileRead* reader = new FileRead();
	bool file_exists, eof;
	std::string filename;
	std::string strbuffer;
	uint buffer_size = 32;
	Char* buffer_array = new Char[buffer_size];

	// Find the EoT, EoF,EoCF
	current_Char = first_Char;
	while(current_Char!=nullptr) {
		if(current_Char->isEndofTitle()) {title = current_Char;}
		if(current_Char->isEndofFile()) {eoF = current_Char;}
		if(current_Char->isEndofCompFile()) {eoCF = current_Char;}
		current_Char = current_Char->next;
	}

	// Read each file in
	for(uint i=0; i<filenames.size(); i++) {
		filename = filenames[i];
		file_exists = reader->setFileName(filename);
		if(file_exists) {
			eof = false;
			// Read in filename
			for(uint j=0; j<filename.size(); j++) {
				current_Char =
			}
			while(!eof) {
				eof = !(reader->readChar());
				byte = reader->getChar();

		  }
		}
	}


	headerwriter->closeFile();
}
