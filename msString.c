#include "msString.h"

struct msStringStruct {
    long length;
    char data[];
};

msString msSetString(char *str) {
    msString newString = malloc(sizeof(long) + strlen(str) + 1);

    if (newString == NULL) {
        msError("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    *(long *)newString = strlen(str);
    strcpy(newString + sizeof(long), str);

    return newString;
}

char *msGetString(msString str) {
    return ((struct msStringStruct *)str)->data;
}

void msCopy(msString *dest, msString src) {
    *dest = malloc(sizeof(struct msStringStruct) + ((struct msStringStruct *)src)->length + 1);
    if (*dest == NULL) {
        msError("Memory allocation error");
    }

    ((struct msStringStruct *)*dest)->length = ((struct msStringStruct *)src)->length;
    strcpy(((struct msStringStruct *)*dest)->data, ((struct msStringStruct *)src)->data);
}

void msConcatenate(msString *dest, msString src) {
    size_t dest_len = *(long *)*dest;
    size_t src_len = *(long *)src;

    size_t new_len = dest_len + src_len;

    *dest = realloc(*dest, sizeof(long) + new_len + 1);

    if (*dest == NULL) {
        msError("Memory reallocation error");
    }

    *(long *)*dest = new_len;

    strcat((char *)*dest + sizeof(long), (char *)src + sizeof(long));
}

long int msLength(msString str) {
    return ((struct msStringStruct *)str)->length;
}

int msCompare(msString str1, msString str2) {
    if (((struct msStringStruct *)str1)->length != ((struct msStringStruct *)str2)->length) {
        return 1;
    }
    return strcmp(((struct msStringStruct *)str1)->data, ((struct msStringStruct *)str2)->data);
}

int msCompareString(msString str1, char *str2) {
    size_t msStringLength = *((long *)str1);
    char *msStringData = (char *)str1 + sizeof(long);

    return strcmp(msStringData, str2) == 0 ? 0 : 1;
}

static void msError(char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}