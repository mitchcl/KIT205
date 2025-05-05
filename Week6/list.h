// list_wk5.h
#pragma once

//TODO: change data type to char*
//Hint: use following typedef to make code clearer

typedef struct listNode {
	char* data;
	struct listNode* next;
} ListNode, *ListNodePtr;

typedef struct list {
	ListNodePtr head;
} List;


List new_list();

void print_list(List* self);

void insert_at_front(List* self, const char* data);

void delete_from_list(List* self, const char* data);

void destroy_list(List* self);

void list_test();