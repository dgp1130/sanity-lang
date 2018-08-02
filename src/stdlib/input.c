#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1024

/**
 * Read all of stdin and return it as a string.
 */
char* read() {
    char* buffer = (char*) malloc(MAX_SIZE * sizeof(char));
    buffer[0] = '\0';

    int i = 0;
    char c;
    while ((c = getchar()) != EOF) buffer[i++] = c;

    return buffer;
}

/**
 * Read an integer from stdin and return it.
 */
int readInt() {
    int value;
    scanf("%d", &value);
    return value;
}