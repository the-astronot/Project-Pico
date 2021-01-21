#include "leaff.h"

void leaff::assignBinary(Character* &first_Char, LeafInfo leafstruct) {
	// Define variables
	int* leaves = leafstruct.leaves;
	int num_leaves = leafstruct.num_leaves;
	Character* current_Char = first_Char;
	std::vector<int> binary_ref;
	std::string binary_str;
	bool isfirstchar = true;
	bool started_tree = false;

	int i = 0;
	binary_ref.push_back(0);
	while(current_Char!=nullptr && i < num_leaves) {
		//std::cout << "MADE IT IN THE LOOP" << std::endl;
		if(*(leaves+i+1) > 0) {
			started_tree = true;
			increase(binary_ref,i,isfirstchar);
			*(leaves+i+1) = *(leaves+i+1) - 1;
			binary_str = convertToString(binary_ref);
			//std::cout << binary_str << std::endl;
			current_Char->setBinary(binary_str);
			//std::cout << current_Char->getBinary() << std::endl;
			current_Char = current_Char->next;
		} else {
			if (started_tree) {
				increase(binary_ref,i,isfirstchar);
			}
			i++;
			binary_ref.push_back(0);
			isfirstchar = true;
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

void leaff::increase(std::vector<int> &binary_vec, int i, bool &isfirstchar) {
	bool adjusted = false;
	int j = i;
	while(!adjusted) {
		if(binary_vec[j] == 1) {
			binary_vec[j] = 0;
			j--;
		} else {
			if (isfirstchar) {
				isfirstchar = false;
			} else {
				binary_vec[j] = 1;
			}
			adjusted = true;
		}
	}
}
