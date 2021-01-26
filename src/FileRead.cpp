#include "FileRead.h"

FileRead::FileRead() {}

// Setters
bool FileRead::setFileName(std::string Filename) {
	filename = Filename;
	if(rf) {rf.close();}
	rf.open(filename, std::ios::in);
	if(!rf) {
		return false;
	} else {
		return true;
	}
}

// Getters
bool FileRead::getLine(std::string &line) {
	if(getline(rf,line)) {
		return true;
	}
	rf.close();
	return false;
}
std::string FileRead::getFileName() {return filename;}
char* FileRead::getBuffer() {return buffer;}
char FileRead::getChar() {return byte;}

// Reads a character from the file
bool FileRead::readChar() {
	bool keep_going = false;
	if(!(rf.eof())) {
		rf.read((char*) &byte, sizeof(byte));
		keep_going = true;
	} else {
		rf.close();
	}
	return keep_going;
}

// Reads a number of chars into the buffer
bool FileRead::readBuffer() {
	bool keep_going = false;
	nullify_buffer();
	if(!(rf.eof())) {
		rf.read(buffer, sizeof(buffer));
		keep_going = true;
	} else {
		rf.close();
	}
	return keep_going;
}

// Helper: Zeros Out Buffer
void FileRead::nullify_buffer() {
	for(int i=0; i<32; i++) {
		buffer[i] = '\0';
	}
}
