#include "leaff.h"

void leaff::assignBinary(Character* &first_Char, LeafInfo leafstruct) {
	// Define variables
	int* leaves = leafstruct.leaves;
	int num_leaves = leafstruct.num_leaves;
	Character* current_Char = first_Char;
	std::vector<int> binary_ref;
	std::string binary_str;
	bool adjusted;

	int i = 1;
	int j;
	binary_ref.push_back(0);
	while(current_Char!=NULL) {
		if(*(leaves+i) > 0) {
			adjusted = false;
			j = i;
			while(!adjusted) {
				if(binary_ref[j] == 1) {
					binary_ref[j] = 0;
					j--;
				} else {
					binary_ref[j] = 1;
				}
			}
			binary_str = convertToString(binary_ref);
			current_Char->setBinary(binary_str);
			current_Char->next = current_Char;
		} else {
			i++;
			binary_ref.push_back(0);
		}
	}

}

std::string leaff::convertToString(std::vector<int> binary_vec) {
	std::string binary_str;
	for(unsigned int i=0; i<binary_vec.size(); i++) {
		if(binary_vec[i] == 0) {
			binary_str += "0";
		} else {
			binary_str += "1";
		}
	}
	return binary_str;
}
