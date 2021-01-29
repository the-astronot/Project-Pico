#include "FileWrite.h"

FileWrite::FileWrite() {}

// Setters
bool FileWrite::setFileName(std::string Filename) {
	if (wf) {
		wf.close();
	}
	filename = Filename;
	wf.open(filename, std::ios::binary | std::ios::out);
	if(!wf) {
		return false;
	}
	//wf.write(file_starter, sizeof(file_starter));
	return true;
}

void FileWrite::writeBuffer(char* wbuffer, int bufsize) {
	for(int i=0; i<bufsize; i++) {
		wf.write((wbuffer+i), sizeof(char));
	}
}

void FileWrite::writeBuffer(std::string wbuffer) {
	wf << wbuffer;
}

void FileWrite::closeFile() {
	wf.close();
}
