// list_wk5.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

//TODO: change data type to char*

List new_list() {
	List temp;
	temp.head = NULL;
	return temp;
}

// after changing to char*:
// -- need to change printf for string
void print_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		printf("%s", current->data);
		current = current->next;

		if (current != NULL) {
			printf(", ");
		}
	}
	printf("\n");
}

// after changing to char*:
// -- check how long data parameter is (strlen)
// -- allocate just enough memory for newNode->data (malloc)
// -- copy string from parameter into newNode->data (strcpy)
void insert_at_front(List* self, const char* data) {
	ListNodePtr new_node = malloc(sizeof * new_node);
	new_node->data = malloc(strlen(data) +1);
	strcpy(new_node->data, data);

	new_node->next = self->head;
	self->head = new_node;
}

// after changing to char*:
// -- change test in if statement to string compare (strcmp)
// -- free current->data (memory allocated for string) before freeing current
void delete_from_list(List* self, const char* data) {
	ListNodePtr current = self->head;
	ListNodePtr prev = NULL;

	while (current != NULL) {
		if (strcmp(current->data, data) == 0) {
			if (prev == NULL) {        // front of list
				self->head = current->next;
			}
			else {
				prev->next = current->next;
			}
			free(current->data);
			free(current);
			current = (prev == NULL) ? self->head : prev->next;
		}
		else {                    // middle of list
			prev = current;
			current = current->next;
		}
	}
}

// after changing to char*:
// -- need to free memory allocated for string before freeing node
void destroy_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		ListNodePtr to_free = current;
		current = current->next;
		free(to_free->data);
		free(to_free);
	}
	self->head = NULL;
}

void list_test() {
	List test_list = new_list();

	printf("Testing insert_at_front...\n");
	insert_at_front(&test_list, "test5");
	insert_at_front(&test_list, "test4");
	insert_at_front(&test_list, "test3");
	insert_at_front(&test_list, "test2");
	insert_at_front(&test_list, "test1");

	printf("Expected: test1, test2, test3, test4, test5\n");
	printf("  Result: ");
	print_list(&test_list);

	printf("\nTesting delete_from_list...\n");
	delete_from_list(&test_list, "test4");
	printf("Expected: test1, test2, test3, test5\n");
	printf("  Result: ");
	print_list(&test_list);

	printf("\nTesting destroy_list...\n");
	destroy_list(&test_list);
	printf("List should be empty now.\n");
	printf("  Result: ");
	print_list(&test_list);
}