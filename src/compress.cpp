#include "compress.h"

/*
int main() {
	std::vector<std::string> filenames;
	std::string infilename0 = "test_files/large_corpus/E.coli";
	std::string infilename1 = "test_files/large_corpus/bible.txt";
	std::string infilename2 = "test_files/large_corpus/world192.txt";
	std::string outfilename = "large_corpus.pico";
	//std::string infilename0 = "test_files/my_tests/basic.txt";
	//std::string infilename1 = "test_files/my_tests/hamlet.txt";
	//std::string infilename2 = "test_files/my_tests/shakespeare.txt";
	//std::string infilename3 = "test_files/my_tests/tetris.txt";
	//std::string outfilename = "compressed_files/my_tests.pico";
	filenames.push_back(infilename0);
	filenames.push_back(infilename1);
	filenames.push_back(infilename2);
	//filenames.push_back(infilename3);

	bool print = true;
	bool cont;
	Character* first_Char = nullptr;
	Character* current = nullptr;
	FileWrite* writer = new FileWrite();

	cont = mapping(filenames, first_Char);
	if (cont) {
		writer->setFileName(outfilename);
		writeHeader(writer, first_Char);
		writeBody(writer, filenames, first_Char);
		return 0;
	}

	return 1;
}
*/

// Add Character
void compression::addCharacter(Character* &first_Char, char byte) {
	bool create_new;
	Character* current_Char;

	create_new = true;
	if (first_Char == nullptr) {
		first_Char = new Character(byte);
	} else {
		current_Char = first_Char;
		while(true) {
			if (current_Char->getCharacter() == byte) {
				current_Char->plusOne();
				create_new = false;
				break;
			} else {
				if (current_Char->next == nullptr) {break;}
				else {current_Char = current_Char->next;}
			}
		}
		if (create_new) {
			current_Char->next = new Character(byte);
		}
	}
}


// Code to Assign Binary Strings to All Chaaracters in the Files
bool compression::mapping(std::vector<std::string> filenames, Character* &first_Char) {
	bool print = false;
	Character* current_Char = nullptr;
	char byte;
	bool create_new, infile_exists, eof;
	int num_chars = 1;
	int files_added = 0;
	std::string filename;
	FileRead* freqreader = new FileRead();

	for(uint i=0; i<filenames.size(); i++) {
		filename = filenames[i];
		infile_exists = freqreader->setFileName(filename);
		if(infile_exists) {
			for(uint j=0; j<filename.size(); j++) {
				addCharacter(first_Char, filename[j]);
			}
			eof = false;
			files_added++;
			while(!eof) {
				eof = !(freqreader->readChar());
				byte = freqreader->getChar();
				addCharacter(first_Char, byte);
		  }
		}
	}
	// Count em up
	current_Char = first_Char;
	if(current_Char!=nullptr) {
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

		sort(first_Char);

		// Establish and Improve Tree, Assign Binary Strings
		Tree* tree = new Tree(num_chars, first_Char, print);
		LeafInfo best_leaves = tree->getBestLeaves();

		leaff::assignBinary(first_Char, best_leaves);
	}
	if(first_Char==nullptr) {return false;}
	return true;
}


// Code to Write the Dictionary-Defining Header of the File
void compression::writeHeader(FileWrite* &headerwriter, Character* first_Char) {
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
		}
	}
	// Finish up
	// Fill out trailing byte with zeroes, append to file
	max_bytes = strbuffer.size()/8 + 1;
	for(byte_index=0;byte_index<max_bytes;byte_index++) {
		for(bit_index=0;bit_index<8;bit_index++) {
			str_index = byte_index*8+bit_index;
			if(byte_index == max_bytes-1 && bit_index == 7) {
				bitf::set(buffer_array[byte_index],7-bit_index); //
			} else if(str_index>=strbuffer.size()) {
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
void compression::writeBody(FileWrite* &writer, std::vector<std::string> filenames,
	 							Character* first_Char) {

	Character* current_Char;
	Character* title, *eoF, *eoCF;
	FileRead* reader = new FileRead();
	bool file_exists, eof;
	std::string filename;
	std::string strbuffer;
	uint buffer_size = 32;
	int byte_index, bit_index, str_index, max_bytes;
	char byte;
	char* buffer_array = new char[buffer_size];

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
				current_Char = first_Char;
				while(current_Char!=nullptr) {
					if(current_Char->getCharacter() == filename[j]) {
						strbuffer += current_Char->getBinary();
						break;
					}
					current_Char = current_Char->next;
				}
			}
			strbuffer += title->getBinary();
			while(!eof) {
				eof = !(reader->readChar());
				byte = reader->getChar();
				current_Char = first_Char;
				while(current_Char!=nullptr) {
					if(current_Char->getCharacter() == byte) {
						strbuffer += current_Char->getBinary();
						break;
					}
					current_Char = current_Char->next;
				}
				if(strbuffer.size()>=buffer_size*8) {
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
					writer->writeBuffer(buffer_array, buffer_size);
					strbuffer = strbuffer.substr(buffer_size*8);
				}
		  }
			strbuffer += eoF->getBinary();
		}
	}
	strbuffer += eoCF->getBinary();
	// Clean Up
	max_bytes = strbuffer.size()/8 + 1;
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

	writer->closeFile();
}
