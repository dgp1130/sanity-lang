#include <stdlib.h>
#include <stdio.h>

#define MAX_LENGTH 50

/**
 * Take the given integer and return it as a string. This is necessary to print a number from Sanity, since it currently
 * interprets integers as characters and prints the associated char code. Note, this function is very bad because it has
 * a finite buffer length which can be overflowed and leaks memory by allocating a string and returning it to Sanity
 * code, yet Sanity does not have the capability to free it.
 */
char* stringify(int num) {
    char* buffer = (char*) malloc(MAX_LENGTH * sizeof(char));
    sprintf(buffer, "%d", num);
    return buffer;
}