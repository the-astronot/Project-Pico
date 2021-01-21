#include "Character.h"

void sort(Character* &first) {
	Character* current = first;
	Character* temp = nullptr;
	Character* previous = nullptr;
	while(current->next!=nullptr) {
		if (current->getTimesUsed() >= current->next->getTimesUsed()) {
			previous = current;
			current = current->next;
		} else {
			temp = current->next;
			if (previous != nullptr) {
				previous->next = current->next;
			} else {
				first = current->next;
			}
			current->next = temp->next;
			temp->next = current;
			current = first;
			previous = nullptr;
		}
	}
}
