#include <iostream>
#include <fstream>
#include <string>


class FileWrite {
private:
	std::ofstream wf;
	std::string filename;
	const char file_starter[3] = {'J','R','M'};

public:

	FileWrite() {}

	// Setters
	bool setFileName(std::string Filename) {
		if (wf) {
			wf.close();
		}
		filename = Filename;
		wf.open(filename, std::ios::binary);
		if(!wf) {
	  	return false;
	  }
		wf.write(file_starter, sizeof(file_starter));
		return true;
	}

	void writeBuffer(char* wbuffer, int bufsize) {
		for(int i=0; i<bufsize; i++) {
			if (*(wbuffer+i) != '\0') {
				wf.write((wbuffer+i), sizeof(char));
			}
		}
	}

	void closeFile() {
		wf.close();
	}
};
