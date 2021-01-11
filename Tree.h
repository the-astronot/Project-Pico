#ifndef __Tree_h__
#define __Tree_h__

#include <iostream>
#include <math.h>

typedef unsigned long long int ulint;

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
	Tree(int num_leaves, Character* start, bool print) {
		this->leaves = new int[num_leaves];
		this->num_leaves = num_leaves;
		this->first = start;
		this->print = print;
		this->score = 0;
		plantTree();
	}

	void plantTree() {
		int level = 1;
		int max_leaves = 2;
		std::cout << this->num_leaves << std::endl;
		while(this->num_leaves > max_leaves) {
			level++;
			max_leaves = 2*max_leaves;
		}
		for(int i=0; i<num_leaves; i++) {this->leaves[i]=0;}
		this->leaves[level-1] = max_leaves - this->num_leaves;
		this->leaves[level] = this->num_leaves - (max_leaves-this->num_leaves);
		if(this->print) {
			printTree();
		}
		calcTree();
		updateTree();
	}

	void printTree() {
		std::cout << "[";
		for(int i=1; i<this->num_leaves; i++) {
			if (i != 1) {
				std::cout << ",";
			}
			std::cout << this->leaves[i];
		}
		std::cout << "]" << std::endl;
	}

	void printBestTree() {
		std::cout << "[";
		for(int i=1; i<this->num_leaves; i++) {
			if (i != 1) {
				std::cout << ",";
			}
			std::cout << this->best_leaves[i];
		}
		std::cout << "] BEST LEAVES" << std::endl;
	}

	LeafInfo getBestLeaves() {
		struct LeafInfo best;
		best.leaves = this->best_leaves;
		best.num_leaves = this->num_leaves;
		return best;
	}

	void updateTree() {
		bool roomtogrow = true;
		while(roomtogrow){
			roomtogrow = branchTree();
		}
		printBestTree();
		std::cout << this->score << std::endl;
	}

	bool branchTree() {
		bool changed = false;
		for(int i=this->num_leaves-1; i>0; i--) {
			if(this->leaves[i] > 2) {
				this->leaves[i+1] = this->leaves[i+1]+2;
				this->leaves[i] = this->leaves[i]-3;
				this->leaves[i-1] = this->leaves[i-1]+1;
				changed = true;
				break;
			}
		}
		if (changed) {
			calcTree();
		}
		if (this->print && changed) {
			printTree();
		}
		return changed;
	}

	void calcTree() {
		ulint sum = 0;
		Character* current = first;
		for(int i=1; i<this->num_leaves; i++) {
			for(int j=0; j<this->leaves[i]; j++) {
				sum += i*current->getTimesUsed();
				//std::cout << sum << std::endl;
				current = current->next;
			}
		}
		std::cout << sum << std::endl;
		if (this->score == 0 || sum < this->score) {
			this->score = sum;
			this->best_leaves = new int[this->num_leaves];
			for(int k=0; k<this->num_leaves; k++) {
				this->best_leaves[k] = this->leaves[k];
			}
		}
	}

};

#endif
