#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <lab5.h>

int main(){

    int numberOfLyb = 2;
    char* names[] = {"./libdyn1.so", "./libdyn2.so"};

    int n = 0;
    void* handle;
    handle  = dlopen(names[n], RTLD_LAZY);
    
    if (!handle){
        printf("dlopen error\n");
        return -1;
    }

    TranslationType trans;
    SortType sort;
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
        trans = (TranslationType)dlsym(handle, "Translation"); 
        sort = (SortType)dlsym(handle, "Sort");
    #pragma GCC diagnostic pop

    int t;
    printf("0 - switch library 1 - translation, 2 - sort of array\n");

    while(scanf("%d", &t) != EOF){  

        switch (t) {
            case 0:{

                n = (n + 1) % numberOfLyb;

                if (dlclose(handle ) != 0){
                    perror("dlclose error");
                    return -1;
                }

                if (!(handle  = dlopen(names[n], RTLD_LAZY))){
                    printf("dlopen error\n");
                    return -1;
                }

                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wpedantic"
                    trans = (TranslationType)dlsym(handle, "Translation"); 
                    sort = (SortType)dlsym(handle, "Sort");
                #pragma GCC diagnostic pop
                printf("Swithced\n");
                break;

            }
            case 1:{

                printf("Enter x = ");
                int x;
                scanf("%d", &x);
                char* result = (*trans)(x);
                printf("Translation is %s\n", result);
                free(result);
                break;

            }
            case 2:{

                printf("Enter size of array = ");
                unsigned long size;
                scanf("%lu", &size);
                int* arr = malloc(size * sizeof(int));
                printf("Start input:\n");

                for (unsigned long i = 0; i < size; ++i){
                    scanf("%d", arr + i);
                }

                printf("Sorted: \n");
                (*sort)(arr, size);

                for (unsigned long i = 0; i < size; ++i){
                    printf("%d ", arr[i]);
                }

                printf("\n");
                free(arr);
                break;

            }
            default:{

                printf("0 - switch library 1 - translation, 2 - sort of array\n");
                break;

            }
        }

    }
    return 0;

}