#include "decompress.h"


// Global vars for decompression
std::map<std::string, char> bin_map;
std::map<std::string, char>::iterator it;
std::vector<std::string> special_chars = {"EOH", "EOT", "EOF", "EOCF"};


bool decompress::checkEndianness(FileRead* &endianreader) {
	std::string pic;
	for(int i=0; i<3; i++) {
		//std::cout << pic << std::endl;
		endianreader->readChar();
		pic += endianreader->getChar();
	}
	//std::cout << pic << std::endl;
	if (pic == "PIC") {
		return true;
	}
	std::cout << "Different" << std::endl;//>>>------------------------------->FIX
	return false;
}


bool decompress::constructDict(FileRead* &headerreader, bool endianness) {
	std::string str_buffer = "";
	int del_size = 0;
	char letter;
	std::string binary;
	bool is_special;
	int special_index;
	bool cont = true;

	//special_chars = {"EOH", "EOT", "EOF", "EOCF"};

	while(cont) { //
		is_special = false;
		while(str_buffer.size()<4) {
			readinchar(headerreader, str_buffer);
		}

		for(special_index=0; special_index<4; special_index++) { // Special Char Detector
			if(str_buffer[special_index] == '1') {
				is_special = true;
				if(special_index==0) {
					cont = false;
					if(str_buffer.size()==4) {
						readinchar(headerreader, str_buffer); // Offset
					}
				}
				break;
			}
		}
		if(!cont) {break;} // If end of header, ditch it
		//std::cout << "PREFIX: " << str_buffer.substr(0,4) << std::endl;
		str_buffer = str_buffer.substr(4);
		if(!is_special) { // Get Binary Byte Char
			while(str_buffer.size()<8) {
				readinchar(headerreader, str_buffer);
			}
			// Make Char
			for(int i=0; i<8; i++) {
				if(str_buffer[i] == '1') {
					bitf::set(letter, 7-i);
				} else {
					bitf::unset(letter, 7-i);
				}
			}
			//std::cout << "CHARACTER: " << str_buffer.substr(0,8) << std::endl;
			str_buffer = str_buffer.substr(8);
		}

		del_size = 0;
		while(del_size == 0) { // DETERMINE BINARY STRING LENGTH
			for(int i=1; i<str_buffer.size(); i++) {
				if(str_buffer[i] == '1') {
					del_size = i-1;
					//std::cout << "SPACER: " << str_buffer.substr(0,i) << std::endl;
					str_buffer = str_buffer.substr(i+1);
					break;
				}
			}
			if(del_size == 0) {
				readinchar(headerreader, str_buffer);
			}
		}

		while(str_buffer.size()<del_size+1) { // GET BINARY STRING
			readinchar(headerreader, str_buffer);
		}
		binary = str_buffer.substr(0,del_size+1);
		str_buffer = str_buffer.substr(del_size+1);

		if(is_special) {
			//std::cout << "SPECIAL\n";
			special_chars[special_index] = binary;
		} else {
			//std::cout << "LETTER: " << letter << std::endl;
			bin_map[binary] = letter;
		}
		//std::cout << "BINARY: " << binary << std::endl;
		//std::cout << "----------------------------------------------\n";

		//break;
	}

	//std::cout << "End Byte: " << str_buffer << std::endl;
	/*
	for (const auto& [key, value] : bin_map) { // print map
      std::cout << key << " = " << value << "; ";
	}
	std::cout << std::endl;

	for(int i=0; i<4; i++) {
		std::cout << special_chars[i] << " : ";
	}
	std::cout << std::endl;
	*/

	return true;
}


void decompress::decompressFiles(FileRead* &reader) {
	bool cont = true;
	bool finished, title, not_end;
	std::string name, str_buffer, body;
	char* array_buffer = new char[32];
	char byte;
	int i,j;
	FileWrite* writer = new FileWrite();
	title = true; // Body starts with first title

	while(cont) {

		not_end = reader->readBuffer();
		if(!not_end) {
			std::cout << "Nothing to read" << '\n';
			cont=false;
		} else { // Read in 32 bytes
			array_buffer = reader->getBuffer();
			for(i=0; i<32; i++) {
				byte = array_buffer[i];
				str_buffer += bitf::toBinaryString(byte);
			}
		}

		finished = false;
		while(!finished) { // If not finished reading compressed file, keep reading
			finished = true;
			for(i=1; i<str_buffer.size(); i++) {
				it = bin_map.find(str_buffer.substr(0,i));
				if(it!=bin_map.end()) {
					if(title) {
						name += it->second;
					} else {
						body += it->second;
					}
					str_buffer = str_buffer.substr(i);
					finished = false;
					break;
				}
				if(str_buffer.substr(0,i)==special_chars[1]) {
					title = false;
					str_buffer = str_buffer.substr(i);
					finished = false;
					break;
				} else if (str_buffer.substr(0,i)==special_chars[2]) { // End of file
					writeToFile(writer, name, body);
					//std::cout << "Wrote to File: " << name << std::endl;
					name = "";
					body = "";
					title = true;
					str_buffer = str_buffer.substr(i);
					finished = false;
					break;
				} else if (str_buffer.substr(0,i)==special_chars[3]) {
					cont = false;
					break;
				}
			}
		}
	}
}


void decompress::readinchar(FileRead* &headerreader, std::string &str_buffer) {
	char char_buffer;

	if(headerreader->readChar()) {
		char_buffer = headerreader->getChar();
		str_buffer += bitf::toBinaryString(char_buffer);
	} else {
		str_buffer = "";
	}
}

void decompress::writeToFile(FileWrite* &writer, std::string name,
	 																							std::string body) {
	fs::path path;
	fs::directory_entry entry_path;
	for(int i=0; i< name.size(); i++) {
		if(name[i] == '/') {
			path = name.substr(0,i);
			entry_path.assign(path);
			if(!(entry_path.exists())) { // Check if path exists
				fs::create_directory(path);
			}
		}
	}
	writer->setFileName(name);
	// Gets rid of extra newlines at end of file
	while(body[body.size()-1]=='\n' && body[body.size()-2]=='\n') {
		body = body.substr(0,body.size()-1);
	}
	writer->writeBuffer(body);
	writer->closeFile();
}
