#ifndef __FileRead_h__
#define __FileRead_h__

#include <iostream>
#include <fstream>
#include <string>

typedef unsigned int uint;

class FileRead {
private:
  std::ifstream rf;
  std::string filename;
	bool compress;
	char byte;
	//const int buffer_size = 32;
	char buffer[32];


public:

  // Initializes Class
  FileRead(){}

  // Setters
  bool setFileName(std::string Filename) {
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
	std::string getFileName() {return filename;}
	char* getBuffer() {return buffer;}
	char getChar() {return byte;}

	// Reads a character from the file
	bool readChar() {
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
	bool readBuffer() {
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
	void nullify_buffer() {
		for(int i=0; i<32; i++) {
			buffer[i] = '\0';
		}
	}
};


#endif
