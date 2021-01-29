#ifndef __compress_h__
#define __compress_h__

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

namespace compression {

	typedef unsigned int uint;

	void getInputFiles();
	void addCharacter(Character* &first_Char, char byte);
	bool mapping(std::vector<std::string> filenames, Character* &first_Char);
	void writeHeader(FileWrite* &headerwriter, Character* first_Char);
	void writeBody(FileWrite* &writer, std::vector<std::string> filenames,
		 							Character* first_Char);

}

#endif
