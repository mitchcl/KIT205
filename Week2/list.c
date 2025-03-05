#include <stdio.h>
#include <stdlib.h>
#include "list.h"


// Function to create and return a new empty list.
List new_list() {
	List temp;
	temp.head = NULL;
	return temp;
}

// Function to print the contents of a list.
void print_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		printf("%d", current->data);
		current = current->next;

		if (current != NULL)
			printf(", ");
	}
	printf("\n");
}

// Function to insert a new node with given data at the front of the list.
void insert_at_front(List* self, int data) {
	ListNodePtr new_node = malloc(sizeof * new_node);

	new_node->data = data;
	new_node->next = self->head;
	self->head = new_node;
}

// Function to insert a new node with given data in the list in ascending order.
void insert_in_order(List* self, int data) {
	ListNodePtr current = self->head;
	ListNodePtr prev = NULL;

	ListNodePtr new_node = malloc(sizeof * new_node);
	new_node->data = data;
	new_node->next = NULL;

	while (current != NULL && current->data < data) {
		prev = current;
		current = current->next;
	}

	if (current == self->head) { // at front
		new_node->next = current;
		self->head = new_node;
	}
	else {                     // middle
		new_node->next = current;
		prev->next = new_node;
	}
}

// Function to delete the first occurrence of a node with given data from the list.
void delete_list(List* self, int data) {
	ListNodePtr current = self->head;
	ListNodePtr prev = NULL;

	while (current != NULL) {
		if (current->data == data) {
			if (prev == NULL) {        // front of list
				self->head = current->next;
				free(current);
				current = self->head;
			}
			else {                    // middle of list
				prev->next = current->next;
				free(current);
				current = prev->next;
			}
		}
		else {
			prev = current;
			current = current->next;
		}
	}
}

// Function to free the memory allocated to a list and all its nodes.
void destroy_list(List* self) {
	ListNodePtr current = self->head;
	while (current != NULL) {
		ListNodePtr to_free = current;
		current = current->next;
		free(to_free);
	}
	self->head = NULL;
}

List reverse(List* self) {
    List reversed_list = new_list();
    ListNodePtr current = self->head;
    while (current != NULL) {
        insert_at_front(&reversed_list, current->data);
        current = current->next;
    }
    return reversed_list;
}

List merge(List* list1, List* list2) {
    List merged_list = new_list();
    ListNodePtr current1 = list1->head;
    ListNodePtr current2 = list2->head;

    while (current1 != NULL && current2 != NULL) {
        if (current1->data < current2->data) {
            insert_in_order(&merged_list, current1->data);
            current1 = current1->next;
        } else {
            insert_in_order(&merged_list, current2->data);
            current2 = current2->next;
        }
    }

    while (current1 != NULL) {
        insert_in_order(&merged_list, current1->data);
        current1 = current1->next;
    }

    while (current2 != NULL) {
        insert_in_order(&merged_list, current2->data);
        current2 = current2->next;
    }

    return merged_list;
}

void option_insert(List* my_list) {
    int data;
    printf("Enter a number to insert: ");
    scanf("%d", &data);
    insert_at_front(my_list, data);
}

void option_delete(List* my_list) {
    int data;
    printf("Enter a number to delete: ");
    scanf("%d", &data);
    delete_list(my_list, data);
}

void option_print(List* my_list) {
    printf("List contents: ");
    print_list(my_list);
}

void list_adhoc_test() {
    List my_list = new_list();
    int quit = 0;

    while (!quit) {
        int option;
        printf("Menu:\n");
        printf("0: Quit\n");
        printf("1: Insert\n");
        printf("2: Delete\n");
        printf("3: Print\n");
        printf("4: Reverse\n");
        printf("5: Merge\n");
        printf("Enter your option: ");
        scanf("%d", &option);

        switch (option) {
            case 0:
                quit = 1;
                break;
            case 1:
                option_insert(&my_list);
                break;
            case 2:
                option_delete(&my_list);
                break;
            case 3:
                option_print(&my_list);
                break;
            case 4: {
                List reversed_list = reverse(&my_list);
                printf("Reversed list: ");
                print_list(&reversed_list);
                destroy_list(&reversed_list);
                break;
            }
            case 5: {
                List list2 = new_list();
                printf("Enter elements for the second list (end with 0):\n");
                int data;
                while (1) {
                    scanf("%d", &data);
                    if (data == 0) break;
                    insert_in_order(&list2, data);
                }
                List merged_list = merge(&my_list, &list2);
                printf("Merged list: ");
                print_list(&merged_list);
                destroy_list(&list2);
                destroy_list(&merged_list);
                break;
            }
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    destroy_list(&my_list);
}

void list_test() {
    List test_list = new_list();
    printf("Testing insert_at_front... \n");
    insert_at_front(&test_list, 5);
    insert_at_front(&test_list, 3);
    insert_at_front(&test_list, 7);
    insert_at_front(&test_list, 2);
    insert_at_front(&test_list, 0);
    printf("Expected: 0, 2, 7, 3, 5 \n");
    printf(" Result: ");
    print_list(&test_list);
}