#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lab5.h"

int Compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
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
        str[p] = '0' + x % 3;
        x = x / 3;
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
    qsort(newArray, size, sizeof(int), Compare);
    return newArray;
}