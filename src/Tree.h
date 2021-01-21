#ifndef __Tree_h__
#define __Tree_h__

#include <iostream>
#include <math.h>

typedef unsigned long long int ulint;

//
struct LeafInfo{
	int* leaves;
	int num_leaves;
};

class Tree {
private:
	int* leaves;
	int* best_leaves;
	int num_leaves;
	ulint score;
	Character* first;
	bool print;

public:
	Tree(int Num_Leaves, Character* start, bool Print) {
		leaves = new int[Num_Leaves];
		num_leaves = Num_Leaves;
		first = start;
		print = Print;
		score = 0;
		plantTree();
	}

	void plantTree() {
		int level = 1;
		int max_leaves = 2;
		std::cout << num_leaves << std::endl;
		while(num_leaves > max_leaves) {
			level++;
			max_leaves = 2*max_leaves;
		}
		for(int i=0; i<num_leaves; i++) {leaves[i]=0;}
		leaves[level-1] = max_leaves - num_leaves;
		leaves[level] = num_leaves - (max_leaves-num_leaves);
		if(print) {
			printTree();
		}
		calcTree();
		updateTree();
	}

	void printTree() {
		std::cout << "[";
		for(int i=1; i<num_leaves; i++) {
			if (i != 1) {
				std::cout << ",";
			}
			std::cout << leaves[i];
		}
		std::cout << "]" << std::endl;
	}

	void printBestTree() {
		std::cout << "[";
		for(int i=1; i<num_leaves; i++) {
			if (i != 1) {
				std::cout << ",";
			}
			std::cout << best_leaves[i];
		}
		std::cout << "] BEST LEAVES" << std::endl;
	}

	LeafInfo getBestLeaves() {
		struct LeafInfo best;
		best.leaves = best_leaves;
		best.num_leaves = num_leaves;
		return best;
	}

	void updateTree() {
		bool roomtogrow = true;
		while(roomtogrow){
			roomtogrow = _revbranchTree();
		}
		if (print) {
			printBestTree();
		}
		std::cout << score << std::endl;
	}

	bool branchTree() {
		bool changed = false;
		for(int i=num_leaves-1; i>0; i--) {
			if(leaves[i] > 2) {
				leaves[i+1] = leaves[i+1]+2;
				leaves[i] = leaves[i]-3;
				leaves[i-1] = leaves[i-1]+1;
				changed = true;
				break;
			}
		}
		if (changed) {
			calcTree();
		}
		if (print && changed) {
			printTree();
		}
		return changed;
	}


	// Working on this one
	bool _revbranchTree() {
		bool changed = false;
		int best_array[num_leaves];
		int current_array[num_leaves];
		ulint best_sum;
		ulint current_sum;
		for(int i=1; i<num_leaves; i++) {
			if(leaves[i] > 2) {
				for(int j=0; j<num_leaves; j++) {
					current_array[j] = leaves[j];
				}
				current_array[i+1] = current_array[i+1]+2;
				current_array[i] = current_array[i]-3;
				current_array[i-1] = current_array[i-1]+1;
				current_sum = _revcalcTree(current_array);
				if(!changed || current_sum < best_sum) {
					best_sum = current_sum;
					for(int k=0; k<num_leaves; k++) {
						best_array[k] = current_array[k];
					}
				}
				changed = true;
			}
		}
		if (changed) {
			for(int j=0; j<num_leaves; j++) {
				leaves[j] = best_array[j];
			}
		}
		if (changed) {
			calcTree();
		}
		if (print && changed) {
			printTree();
		}
		return changed;
	}

	void calcTree() {
		ulint sum = 0;
		Character* current = first;
		for(int i=1; i<num_leaves; i++) {
			for(int j=0; j<leaves[i]; j++) {
				sum += i*current->getTimesUsed();
				//std::cout << sum << std::endl;
				current = current->next;
			}
		}
		if (print) {std::cout << sum << std::endl;}

		if (score == 0 || sum < score) {
			score = sum;
			best_leaves = new int[num_leaves];
			for(int k=0; k<num_leaves; k++) {
				best_leaves[k] = leaves[k];
			}
		}
	}

	ulint _revcalcTree(int c_leaves[]) {
		ulint sum = 0;
		Character* current = first;
		for(int i=1; i<num_leaves; i++) {
			for(int j=0; j<c_leaves[i]; j++) {
				sum += i*current->getTimesUsed();
				current = current->next;
			}
		}
		//if (this->print) {std::cout << sum << std::endl;}

		return sum;
	}

};

#endif
