#ifndef __FileWrite_h__
#define __FileWrite_h__

#include <iostream>
#include <fstream>
#include <string>


class FileWrite {
private:
	std::ofstream wf;
	std::string filename;
	//const char file_starter[3] = {'P','I','C'};

public:

	FileWrite();

	// Setters
	bool setFileName(std::string Filename);

	void writeBuffer(char* wbuffer, int bufsize);
	void writeBuffer(std::string wbuffer);

	void closeFile();
};

#endif
