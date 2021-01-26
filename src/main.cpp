#include "main.h"

namespace fs = std::filesystem;

std::string password;
std::vector<std::string> optionnames = {"comp","decomp","display=",
																				"password=","application=",
																				"makedefault-"};

int main(int argc, char *argv[]) {

	bool arg_error = false;
	std::string argstr;
	std::vector<std::string> filenames;
	std::vector<std::string> bool_str ={"Compress", "DispNone", "DispAll",
	 																		"DispBasic", "Passwd"};
	std::vector<bool> bool_options = {true,true,false,false,false};

	loadDefaults(bool_options);

	for(int i=1; i<argc; i++) {
		argstr = argv[i];
		if(argstr[0] == '-') {
			// Args
			argstr = argstr.substr(1);
			arg_error = options(argstr, bool_options);
		} else {
			// Files
			filenames.push_back(argstr);
		}
		if (arg_error) {
			std::cout << "One or more unknown args" << std::endl;
			return 1;
		}
	}

	// Begin Operations
	for(int i=0; i<bool_options.size(); i++) {
		std::cout << bool_str[i] << " : ";
		if(bool_options[i]) {
			std::cout << "True\n";
		} else {
			std::cout << "False\n";
		}
	}

	//std::cout << "Current path is " << fs::current_path() << '\n';
	return 0;
}


// Configures Command Line options
bool options(std::string option_name, std::vector<bool> &options) {

	size_t found;
	uint index;
	bool option_exists;

	option_exists = false;
	//std::cout << "OPTION NAME: " << option_name << std::endl;
	for(uint i=0; i<optionnames.size(); i++) {
		//std::cout << "Checking: " << optionnames[i];
		found = option_name.find(optionnames[i]);
		if(found == 0) {
			option_exists = true;
			index = i;
			break;
		}
	}

	if(!option_exists) {
		return true;
		//std::cout << "No option exists" << std::endl;
	}

	// All the Options
	if(index == 0) { // Compressing
		options[0] = true;
		//std::cout << "Compressing" << '\n';
	} else if(index == 1) { // Decompressing
		options[0] = false;
		//std::cout << "Decompressing" << '\n';
	} else if(index == 2) { // Display
		option_name = option_name.substr(8);
		if(option_name == "none") {
			options[1] = true;
			options[2] = false;
			options[3] = false;
		} else if (option_name == "all") {
			options[1] = false;
			options[2] = true;
			options[3] = false;
		} else if(option_name == "basic") {
			options[1] = false;
			options[2] = false;
			options[3] = true;
		}
		//std::cout << "Display Stuff" << std::endl;
	} else if(index == 3) { // Password
		password = option_name.substr(9);
		if(password == "" || password == "none") {
			options[4] = false;
		} else {
			options[4] = true;
		}
		//std::cout << "Password Stuff" << std::endl;
	} else if(index == 5) {
		// Make Defaults
	}

	return false;
}


// Loads Default Options from Config File
void loadDefaults(std::vector<bool> &options) {
	std::string line;
	size_t found;
	uint index;
	bool option_found;
	FileRead* confreader = new FileRead();

	// Load Config File
	confreader->setFileName("config.txt");
	while(confreader->getLine(line)) {
		if(line[0] != '#') {
			option_found = false;
			index = 0;
			for(int i=0; i<optionnames.size(); i++) {
				found = line.find(optionnames[i]);
				if(found == 0) {
					index = i;
					option_found = true;
					break;
				}
			}
			if(option_found) {
				if(index == 2) { // Display
					line = line.substr(8);
					if(line == "none") {
						options[1] = true;
						options[2] = false;
						options[3] = false;
					} else if (line == "all") {
						options[1] = false;
						options[2] = true;
						options[3] = false;
					} else if(line == "basic") {
						options[1] = false;
						options[2] = false;
						options[3] = true;
					}
				} else if(index == 3) { // Password
					password = line.substr(9);
					if(password == "" || password == "none") {
						options[4] = false;
					} else {
						options[4] = true;
					}
				} else if(index == 4) { // Compression/Decompression
					line = line.substr(12);
					for(int j=0; j<2; j++) {
						found = line.find(optionnames[j]);
						if(found == 0) {
							if(j == 0) {
								options[0] = true;
							} else if (j == 1){
								options[0] = false;
							}
						}
					}
				}
			}
		}
	}
}
