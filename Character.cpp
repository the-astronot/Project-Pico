#include "Character.h"

void sort(Character* &first) {
	Character* current = first;
	Character* temp;
	Character* previous;
	while(current->next!=NULL) {
		if (current->getTimesUsed() >= current->next->getTimesUsed()) {
			previous = current;
			current = current->next;
		} else {
			temp = current->next;
			if (previous != NULL) {
				previous->next = current->next;
			} else {
					first = current->next;
			}
			current->next = temp->next;
			temp->next = current;
			current = first;
			previous = NULL;
		}
	}
}
