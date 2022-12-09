typedef char*(*TranslationType)(long);
typedef void(*SortType)(int*, unsigned long);        

char* Translation(long x);
void Sort(int* array, unsigned long size);