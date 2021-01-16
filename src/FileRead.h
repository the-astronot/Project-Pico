#include <iostream>
#include <fstream>
#include <string>

#ifndef __FileRead_h__
#define __FileRead_h__

typedef unsigned int uint;

class FileRead {
private:
  ifstream Read;
  std::string filename;



public:

  // Initializes Class
  FileRead();

  // Setters
  void setFileName(std:string filename) {this.filename = filename};

  // Return Hex from File
  std::string ReadOutHex();
}


#endif
