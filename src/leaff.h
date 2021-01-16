#ifndef __leaff_h__
#define __leaff_h__

#include <iostream>
#include <vector>
#include <string>
#include "Character.h"
#include "Tree.h"


// Number of functions intended to assign binary strings to characters
namespace leaff {
	
	 void assignBinary(Character* &first_Char, LeafInfo leafstruct);
	 std::string convertToString(std::vector<int> binary_vec);
	 void increase(std::vector<int> &binary_vec, int i, bool &isfirstchar);
}

#endif
