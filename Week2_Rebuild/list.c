#include "list.h"
#include <stdio.h>
#include <stdlib.h>

// Function to create and return a new empty list.
List new_list() {
    List list;
    list.head = NULL;
    return list;
}

// Function to print the contents of a list.
void print_list(List* self) {
    ListNodePtr current = self->head;
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// Function to insert a new node with given data at the front of the list.
void insert_at_front(List* self, int data) {
    ListNodePtr new_node = (ListNodePtr)malloc(sizeof(struct listNode));
    new_node->data = data;
    new_node->next = self->head;
    self->head = new_node;
}

// Function to insert a new node with given data in the list in ascending order.
void insert_in_order(List* self, int data) {
    ListNodePtr new_node = (ListNodePtr)malloc(sizeof(struct listNode));
    new_node->data = data;
    new_node->next = NULL;

    if (self->head == NULL || self->head->data >= data) {
        new_node->next = self->head;
        self->head = new_node;
    }
    else {
        ListNodePtr current = self->head;
        while (current->next != NULL && current->next->data < data) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

// Function to delete the first occurrence of a node with given data from the list.
void delete_list(List* self, int data) {
    ListNodePtr current = self->head;
    ListNodePtr previous = NULL;

    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) return;

    if (previous == NULL) {
        self->head = current->next;
    }
    else {
        previous->next = current->next;
    }

    free(current);
}

// Function to free the memory allocated to a list and all its nodes.
void destroy_list(List* self) {
    ListNodePtr current = self->head;
    ListNodePtr next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    self->head = NULL;
}

// Function to reverse a list and return a new list.
List reverse(List* self) {
    List reversed_list = new_list();
    ListNodePtr current = self->head;

    while (current != NULL) {
        insert_at_front(&reversed_list, current->data);
        current = current->next;
    }

    return reversed_list;
}

// Function to merge two ordered lists into a new ordered list.
List merge(List* list1, List* list2) {
    List merged_list = new_list();
    ListNodePtr current1 = list1->head;
    ListNodePtr current2 = list2->head;
    ListNodePtr* last_ptr = &merged_list.head;

    while (current1 != NULL && current2 != NULL) {
        if (current1->data <= current2->data) {
            *last_ptr = current1;
            current1 = current1->next;
        }
        else {
            *last_ptr = current2;
            current2 = current2->next;
        }
        last_ptr = &(*last_ptr)->next;
    }

    *last_ptr = (current1 != NULL) ? current1 : current2;

    return merged_list;
}

// Helper function to insert a new node into the list.
void option_insert(List* self) {
    int data;
    printf("Enter an integer to insert: ");
    scanf_s("%d", &data);
    insert_at_front(self, data);
}

// Helper function to delete a node from the list.
void option_delete(List* self) {
    int data;
    printf("Enter an integer to delete: ");
    scanf_s("%d", &data);
    delete_list(self, data);
}

// Helper function to print the list.
void option_print(List* self) {
    print_list(self);
}

// Function to test the list implementation.
void list_test() {
    // Implementation of test cases
}

// Function for ad-hoc testing of the list implementation.
void list_adhoc_test() {
    List my_list = new_list();
    int quit = 0;

    while (!quit) {
        int option;
        printf("Select an option:\n");
        printf("0: Quit\n");
        printf("1: Insert\n");
        printf("2: Delete\n");
        printf("3: Print\n");
        printf("4: Reverse\n");
        printf("Enter your choice: ");
        scanf_s("%d", &option);

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
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    }

    destroy_list(&my_list);

    // Test the merge function separately
    List list1 = new_list();
    List list2 = new_list();

    insert_in_order(&list1, 1);
    insert_in_order(&list1, 3);
    insert_in_order(&list1, 5);

    insert_in_order(&list2, 2);
    insert_in_order(&list2, 4);
    insert_in_order(&list2, 6);

    List merged_list = merge(&list1, &list2);
    printf("Merged list: ");
    print_list(&merged_list);

    destroy_list(&list1);
    destroy_list(&list2);
    destroy_list(&merged_list);
}