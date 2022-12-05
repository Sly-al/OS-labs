#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lab5.h"

void IntSwap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void CharSwap(char* a, char* b){
    char temp = *a;
    *a = *b;
    *b = temp;
}

char* Translation(long x){
    char* str = malloc(sizeof(int)*8);
    int p = 0;

    while(x > 0){
        str[p] = '0' + x % 2;
        x = x / 2;
        p++;
    }

    str[p] = '\0';
    int i = 0;

    while(i < p){
        p--;
        CharSwap(str + i, str + p);
        i++;
        
    }

    return str;
}

int* Sort(int* array, unsigned long size) {
    int* newArray;
    newArray = (int*)malloc(sizeof(int)* size);
    memcpy(newArray, array, sizeof(int)*size);

    for (unsigned long i = 0; i < size; ++i) {
        for (unsigned long j = 0; j < size - 1; ++j) {
            if (newArray[j] > newArray[j + 1]) {
                IntSwap(newArray + j, newArray + j + 1);
            }
        }
    }

    return newArray;
}
