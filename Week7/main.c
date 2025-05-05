#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
typedef char* String;

void quicksort(String* a, int first, int last) {
    if (first < last) {
        int i = first;
        int j = last - 1;
        while (i <= j) {
            while (i < last && strcmp(a[i], a[last]) <= 0) {
                i++;
            }
            while (j >= first && strcmp(a[j], a[last]) > 0) {
                j--;
            }
            if (i < j) {
                String temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
        String temp = a[i];
        a[i] = a[last];
        a[last] = temp;

        quicksort(a, first, i - 1);
        quicksort(a, i + 1, last);
    }
}

int main() {
    String* strings;
    char buffer[100];
    int n;

    printf("Enter the number of strings you wish to have: ");
    scanf_s("%d", &n);

    strings = (String*)malloc(n * sizeof(String));

    for (int i = 0; i < n; i++) {
        printf("Enter %d string: ", i + 1);
        scanf_s("%s", buffer, (unsigned)sizeof(buffer));
        strings[i] = malloc(strlen(buffer) + 1);
        strcpy_s(strings[i], strlen(buffer) + 1, buffer);
    }

    quicksort(strings, 0, n - 1);

    for (int i = 0; i < n; i++) {
        printf("%s\n", strings[i]);
        free(strings[i]);
    }
    free(strings);

    return 0;
}