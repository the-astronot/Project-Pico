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
  FileRead();

  // Setters
  bool setFileName(std::string Filename);

	// Getters
	bool getLine(std::string &line);
	std::string getFileName();
	char* getBuffer();
	char getChar();

	// Reads a character from the file
	bool readChar();

	// Reads a number of chars into the buffer
	bool readBuffer();

	// Helper: Zeros Out Buffer
	void nullify_buffer();
};


#endif
