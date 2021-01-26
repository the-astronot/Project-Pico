#ifndef __main_h__
#define __main_h__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "FileRead.h"
#include "FileWrite.h"
#include "compress.h"
//#include "decompress.h"


bool options(std::string option_name, std::vector<bool> &options);
void loadDefaults(std::vector<bool> &options);

#endif
