#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "msString.h"

void reverseAndWriteToFile(FILE *inputFile, FILE *outputFile) {
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);

    char *buffer = (char *)malloc(fileSize + 1);

    if (buffer == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, fileSize, inputFile);
    buffer[fileSize] = '\0';

    long i, j;
    for (i = 0, j = fileSize - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    if (fprintf(outputFile, "%s", buffer + 1) < 0) {
        perror("Error writing to file");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    free(buffer);
}

void printBytes(void *ptr, int numBytes) {
    unsigned char *bytePtr = (unsigned char *)ptr;

    printf("Starting at memory address %p:\n", ptr);

    int i;
    for (i = 0; i < numBytes; i++) {
        printf("%03d: %d\n", i + 1, *bytePtr);
        bytePtr++;
    }
}

int main(int argc, char *argv[]) {

    unsigned char myArray[] = {10, 20, 30, 145};

    printBytes(myArray, 4);

    if (argc != 3) {
      fprintf(stderr, "Usage: %s fileIn fileOut\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");

    if (inputFile == NULL || outputFile == NULL) {
      perror("Error opening files");
      exit(EXIT_FAILURE);
    }

    reverseAndWriteToFile(inputFile, outputFile);

    fclose(inputFile);
    fclose(outputFile);
  
    msString ms = msSetString("Hello");
    msString ms2 = msSetString(" World!");
    msString mscopy = NULL;

    printf("String |%s| is %d characters long (%p).\n",
           msGetString(ms), msLength(ms), (void *)ms);

    msCopy(&mscopy, ms);

    printf("Copied string |%s| is %d characters long (%p).\n",
           msGetString(mscopy), msLength(mscopy), (void *)mscopy);

    printf("Compare ms with mscopy: %d\n", msCompare(ms, mscopy));
    printf("Compare ms with ms2: %d\n", msCompare(ms, ms2));
    printf("Compare ms with Hello: %d\n", msCompareString(ms, "Hello"));
    printf("Compare ms with HelloX: %d\n", msCompareString(ms, "HelloX"));
    printf("Compare ms with Hella: %d\n", msCompareString(ms, "Hella"));

    msConcatenate(&mscopy, ms2);

    printf("Concatenated string |%s| is %d characters long (%p).\n",
           msGetString(mscopy), msLength(mscopy), (void *)mscopy);

    free(ms);
    free(ms2);
    free(mscopy);

    return 0;
}