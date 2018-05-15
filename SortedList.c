//NAME: Christopher Ngai
//EMAIL: cngai1223@gmail.com
//ID: 404795904

//SortedList.c
#include "SortedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element){
	//if either SortedList_t structs are invalid
	if (list == NULL || element == NULL){
		return;
	}

	//*list points to head of list
	SortedList_t* curr = list->next;

	//iterate through linked list until element key less than curr key
	while (curr != list){
		if (strcmp(element->key, curr->key) <= 0){
			break;
		}

		curr = curr->next;
	}

	if (opt_yield & INSERT_YIELD){
		sched_yield();
	}

	//critical section
	//redirect pointers to add element
	element->prev = curr->prev;
	element->next = curr;
	curr->prev->next = element;
	curr->prev = element;
}

int SortedList_delete(SortedListElement_t *element){
	//check if element is valid
	if (element == NULL){
		return 1;
	}

	//check to make sure next->prev and prev->next point to element node
	if (element->next->prev == element->prev->next){
		if (opt_yield & DELETE_YIELD){
			sched_yield();
		}

		//redirect pointers to delete element
		element->prev->next = element->next;
		element->next->prev = element->prev;

		return 0;
	}

	return 1;
}

SortedList_t *SortedList_lookup(SortedList_t *list, const char *key){
	//check if list is valid and key
	if (list == NULL || key == NULL){
		return NULL;
	}

	SortedList_t* curr = list->next;

	while (curr != list){
		//if keys match
		if (strcmp(curr->key, key) == 0){
			return curr;
		}

		if (opt_yield & LOOKUP_YIELD){
			sched_yield();
		}

		curr = curr->next;
	}

	//if element not found
	return NULL;
}

int SortedList_length(SortedList_t *list){
	//check if list is valid
	if (list == NULL){
		return -1;
	}

	int count = 0;
	SortedList_t* curr = list->next;

	//iterate through linked list
	while (curr != list){
		//check all prev/next pointers
		if (curr->prev->next != curr || curr->next->prev != curr){
			return -1;
		}

		count++;

		if (opt_yield * LOOKUP_YIELD){
			sched_yield();
		}

		curr = curr->next;
	}

	return count;
}