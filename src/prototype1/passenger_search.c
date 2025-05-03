#define _GNU_SOURCE  // For strcasestr function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../airline_types.h"
#include "passenger_management.h"

// Search for a passenger by name in linked list
Passenger* find_passenger_by_name(LL_Node* head, const char* name) {
    LL_Node* current = head;
    
    while (current != NULL) {
        // Case-insensitive substring search
        if (strcasestr(current->data.name, name) != NULL) {
            return &(current->data);
        }
        current = current->next;
    }
    
    return NULL;
}
