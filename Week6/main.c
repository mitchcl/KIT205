#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    List my_list = new_list();
    insert_at_front(&my_list, "Hello");
    insert_at_front(&my_list, "World");
    print_list(&my_list);
    delete_from_list(&my_list, "Hello");
    print_list(&my_list);
    destroy_list(&my_list);
    return 0;
}
