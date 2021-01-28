#include "main.h"


std::string password,outname;
std::vector<std::string> optionnames = {"comp","decomp","display=",
																				"password=","application=",
																				"makedefault-","name="};
// Global Vars
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

	bool arg_error = false;
	std::string argstr;
	std::vector<std::string> filenames;
	fs::path file_path;
	//////////////////////////////////////////////////////////////////////////////
	std::vector<std::string> bool_str ={"Compress", "DispNone", "DispAll",
	 																		"DispBasic", "Passwd"};
	std::vector<bool> bool_options = {true,true,false,false,false};
	// Options
	//////////////////////////////////////////////////////////////////////////////

	loadDefaults(bool_options); // Start out with defaults, then replace as
															// Necessary

	for(int i=1; i<argc; i++) { // Check out all the args
		argstr = argv[i];
		if(argstr[0] == '-') { // Args
			argstr = argstr.substr(1);
			arg_error = options(argstr, bool_options);
		} else { // Files >>>--------------------------------->FIX TO HANDLE COMPLEX
			//filenames.push_back(argstr);
			file_path = argstr;
			bool fileadd = addFiles(file_path, filenames);
		}
		if (arg_error) {
			std::cout << "Error with Arg(" << i << "): -" << argstr << std::endl;
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

	for(int i=0; i<filenames.size(); i++) {
		std::cout << filenames[i] << std::endl;
	}

	if(bool_options[0]) {
		// Compress Files
		////////////////////////////////////////////////////////////////////////////
		// Check for problems with file numbers
		if(filenames.size()==0) {
			std::cout << "Error: Please specify which file(s) to compress\n";
			return 1;
		}

		//outname = std::string(fs::current_path())+"/"+outname;
		std::cout << outname << std::endl;

		bool print = bool_options[2];
		bool cont;
		Character* first_Char = nullptr;
		Character* current = nullptr;
		FileWrite* writer = new FileWrite();

		cont = compression::mapping(filenames, first_Char);
		if (cont) {
			writer->setFileName(outname);
			compression::writeHeader(writer, first_Char);
			compression::writeBody(writer, filenames, first_Char);
			return 0;
		}

		return 1;

	} else {
		// Decompress Files
		////////////////////////////////////////////////////////////////////////////
		// Check for problems with file numbers
		if(filenames.size()>1) {
			std::cout << "Error: Please decompress files one at a time\n";
			return 1;
		} else if (filenames.size() == 0) {
			std::cout << "Error: Please specify which file to decompress\n";
			return 1;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Adds files to vector
bool addFiles(fs::path pathname, std::vector<std::string> &filenames) {
	fs::directory_entry entry_path;
	std::string path_str;
	entry_path.assign(pathname);
	if(entry_path.exists()) {
		if(entry_path.is_directory()) {
			std::cout << pathname << " Is Directory" << std::endl;
			for(auto& p: fs::directory_iterator(pathname))
				addFiles(p.path(), filenames);
		} else {
			std::cout << pathname << " Is File" << std::endl;
			path_str = pathname;
			//path_str = path_str.substr(0,path_str.size());
			filenames.push_back(path_str);
		}
	} else {
		std::cout << pathname << " Doesn't Exist" << std::endl;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
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
	} else if(index == 5) { // Make Defaults
	} else if(index == 6) { // Set compressed name
		outname = option_name.substr(5);
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////
// Loads Default Options from Config File
void loadDefaults(std::vector<bool> &options) {
	std::string line;
	size_t found;
	uint index;
	bool option_found;
	FileRead* confreader = new FileRead();

	// Load Config File
	confreader->setFileName("MAKEFILE_PLACEHOLDER"); // Full Path Set when Made
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
				} else if(index == 6) { // Set Compressed Name
					outname = line.substr(5);
				}
			}
		}
	}
}
