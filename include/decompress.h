#ifndef __decompress_h__
#define __decompress_h__

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include "FileRead.h"
#include "FileWrite.h"
#include "bitf.h"
#include "Character.h"
#include "leaff.h"

namespace fs = std::filesystem;

namespace decompress {

	typedef unsigned int uint;

	// Main functions (Called by MAIN)
	bool checkEndianness(FileRead* &endianreader);
	bool constructDict(FileRead* &headerreader, bool endianness);
	void decompressFiles(FileRead* &reader);

	// Helper functions (Called within file)
	void readinchar(FileRead* &headerreader, std::string &str_buffer);
	void writeToFile(FileWrite* &writer, std::string name, std::string body);

}

#endif
