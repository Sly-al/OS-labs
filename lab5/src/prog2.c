#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

const int NUMBEROFLYB = 2;
const char* NAMES[] = {"./libdyn1.so", "./libdyn2.so"};

int main(){

    int n = 0;
    void* handle;
    handle  = dlopen(NAMES[n], RTLD_LAZY);
    char*(*Translation)(long);
    int*(*Sort)(int*, unsigned long);
    
    if (!handle){
        printf("dlopen error\n");
        return -1;
    }

    Translation = dlsym(handle, "Translation");
    Sort = dlsym(handle, "Sort"); 
    int t;
    printf("0 - switch library 1 - translation, 2 - sort of array\n");

    while(scanf("%d", &t) != EOF){  

        switch (t) {
            case 0:{

                n = (n + 1) % NUMBEROFLYB;

                if (dlclose(handle ) != 0){
                    perror("dlclose error");
                    return -1;
                }

                if (!(handle  = dlopen(NAMES[n], RTLD_LAZY))){
                    printf("dlopen error\n");
                    return -1;
                }

                Translation = dlsym(handle , "Translation");
                Sort = dlsym(handle , "Sort");
                printf("Swithced\n");
                break;

            }
            case 1:{

                printf("Enter x = ");
                int x;
                scanf("%d", &x);
                char* result = (*Translation)(x);
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
                int* ans = (*Sort)(arr, size);

                for (unsigned long i = 0; i < size; ++i){
                    printf("%d ", ans[i]);
                }

                printf("\n");
                free(ans);
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