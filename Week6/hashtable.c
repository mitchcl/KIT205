// hashtable_wk5.c
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

HashTable create_hashtable(int n) {
	HashTable newtable;

	newtable.size = n;
	newtable.table = malloc(n * sizeof(List));

	for (int i = 0; i < n; i++) {
		newtable.table[i] = new_list();
	}

	return newtable;
}

int hash(String key, int size) {
	unsigned long h = 0; // unsigned long, so that when it wraps it's still positive
	int n = strlen(key);

	for (int i = 0; i < n; i++) {
		// multiply by 32 (<<5) and add ascii value
		h = (h << 5) + (int)(key[i]);
	}
	return h % size;
}

void hash_insert(HashTable* self, String key) {
	int index = hash(key, self->size);
	insert_at_front(&(self->table[index]), key);
}

void hash_remove(HashTable* self, String key) {
	int index = hash(key, self->size);
	delete_from_list(&(self->table[index]), key);

}

void hash_print(HashTable* self) {
	for (int i = 0; i < self->size; i++) {
		printf("%d: ", i);
		print_list(&(self->table[i]));
	}
}

void hash_destroy(HashTable* self) {
	for (int i = 0; i < self->size; i++) {
		destroy_list(&(self->table[i]));
	}
}

void hash_adhoc_test() {
	int command = 1;
	HashTable table = create_hashtable(11);
	char buffer[100];

	while (command) {
		scanf_s("%d", &command);
		switch (command) {
		case 1:
			scanf_s("%s", buffer, 100);
			hash_insert(&table, buffer);
			break;
		case 2: 
			scanf_s("%s", buffer, 100);
			hash_remove(&table, buffer);
			break;
		case 3: 
			hash_print(&table);
			break;
		}
		printf("\n");
	}
	// TODO: destroy
	hash_destroy(&table);
}

